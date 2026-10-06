#pragma once
#include <atomic>
#include <algorithm>
#include <chrono>
#include <memory>
#include <array>
#include <sstream>
#include <iomanip>
#include <string>

namespace aim_profiler {
    using Clock=std::chrono::steady_clock;
    enum Stage { Callback, Settings, Aim, Scan, Geometry, EngineAimed, EngineUnchanged, Skins, Knife, Loadout, KnifeModel, KnifeMaterials, OtherMaterials, RegionQuery, ProcessRead, ProcessWrite, LocalRead, EntityLookup, EngineLookup, HandleValidation, PlayerEnumeration, BoneRead, EyeRead, WinnerValidation, VisibilityTrace, Count };
    inline constexpr const char* names[]={"Whole callback", "Settings lock attempt", "Aim (includes scan/geometry)", "Target scan", "Geometry rebuild", "Engine: aimed", "Engine: unchanged", "All skin updates", "Knife update", "Inventory scan", "Knife model rebuild", "Knife material rebuild", "Other material rebuild", "VirtualQuery", "ReadProcessMemory", "WriteProcessMemory", "Validated local read", "Entity lookup", "Engine entity lookup", "Handle validation", "Controller enumeration", "Bone read", "Eye read", "Winner validation", "Visibility trace"};
    enum Event { SkipBusy, SkipStopping, SkipMenu, SkipDisabled, Evaluated, CacheHit, CacheMiss, MemoryFail, RegionHit, RegionMiss, RegionFull, DeadWinner, Applied, NoWrite, NoTarget, EventCount };
    inline constexpr const char* eventNames[]={"skip: settings busy","skip: stopping","skip: menu/console","skip: aim disabled","aim evaluated","target cache hit","target cache miss","memory operation failed","region cache hit","region cache miss","region cache full","dead/replaced winner", "angle writes applied", "evaluations without a write", "no candidate passed filters"};
    inline constexpr unsigned long long bucketNs[]={1000,5000,10000,50000,100000,500000,1000000,5000000,10000000,20000000,50000000,100000000,500000000};
    struct Counter {
        std::atomic<unsigned long long> calls{0}, total_ns{0}, peak_ns{0};
        std::atomic<unsigned long long> self_ns{0};
        std::array<std::atomic<unsigned long long>,14> buckets{};
        void Add(unsigned long long ns, unsigned long long self=0) {
            self_ns.fetch_add(self,std::memory_order_relaxed);
            unsigned bucket=0; while(bucket<13 && ns>bucketNs[bucket]) ++bucket;
            buckets[bucket].fetch_add(1,std::memory_order_relaxed);
            total_ns.fetch_add(ns,std::memory_order_relaxed);
            calls.fetch_add(1,std::memory_order_relaxed);
            auto peak=peak_ns.load(std::memory_order_relaxed);
            while(ns>peak && !peak_ns.compare_exchange_weak(peak,ns,std::memory_order_relaxed)) {}
        }
    };
    struct Capture {
        Clock::time_point begin=Clock::now()+std::chrono::seconds(3);
        Clock::time_point end=begin+std::chrono::seconds(10);
        std::array<Counter,Count> counters;
        std::array<std::array<std::atomic<unsigned long long>,Count>,Count> byRootNs{};
        std::string settings;
        std::atomic<unsigned> pending{0};
        std::array<std::atomic<unsigned long long>,EventCount> events{};
        std::array<std::array<std::atomic<unsigned long long>,EventCount>,Count> attributed{};
    };
    inline std::atomic<std::shared_ptr<Capture>> current;
    inline std::atomic<long long> deadline{0};
    inline void Start(std::string settings={}) {
        auto capture=std::make_shared<Capture>();
        capture->settings=std::move(settings);
        const auto end=std::chrono::duration_cast<std::chrono::nanoseconds>(capture->end.time_since_epoch()).count();
        current.store(capture); deadline.store(end);
    }
    struct Scope;
    inline thread_local Scope* activeScope=nullptr;
    struct Scope {
        std::shared_ptr<Capture> capture;
        Clock::time_point begin;
        Stage stage;
        Scope* parent=nullptr;
        unsigned long long child_ns=0;
        explicit Scope(Stage value):stage(value) {
            if(activeScope) {
                capture=activeScope->capture; parent=activeScope;
                begin=Clock::now(); activeScope=this;
                capture->pending.fetch_add(1,std::memory_order_relaxed); return;
            }
            if(value>=RegionQuery) return; // Detailed probes belong to a captured parent only.
            auto limit=deadline.load(std::memory_order_relaxed);
            if(!limit) return;
            const auto now=Clock::now();
            if(std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count()>=limit) {
                deadline.compare_exchange_strong(limit,0); return;
            }
            auto session=current.load();
            if(!session) return;
            if(now<session->begin || now>=session->end) return;
            capture=std::move(session); begin=now; activeScope=this;
            capture->pending.fetch_add(1,std::memory_order_relaxed);
        }
        ~Scope() {
            if(!capture) return;
            const auto elapsed=static_cast<unsigned long long>(std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now()-begin).count());
            capture->counters[stage].Add(elapsed,elapsed>(child_ns)?elapsed-child_ns:0);
            auto* root=this; while(root->parent) root=root->parent;
            capture->byRootNs[root->stage][stage].fetch_add(elapsed,std::memory_order_relaxed);
            if(parent) parent->child_ns+=elapsed;
            activeScope=parent;
            capture->pending.fetch_sub(1,std::memory_order_relaxed);
        }
        Scope(const Scope&)=delete;
        Scope& operator=(const Scope&)=delete;
    };
    inline void Mark(Event event) {
        if(!activeScope) return;
        activeScope->capture->events[event].fetch_add(1,std::memory_order_relaxed);
        // Attribute to the outermost stage: separates callback and skin traffic.
        auto* root=activeScope; while(root->parent) root=root->parent;
        root->capture->attributed[root->stage][event].fetch_add(1,std::memory_order_relaxed);
    }
    inline double Percentile(const Counter& c,double fraction) {
        const auto calls=c.calls.load(); if(!calls) return 0;
        const auto target=static_cast<unsigned long long>(calls*fraction+.999999);
        unsigned long long seen=0;
        for(unsigned i=0;i<14;++i) {
            seen+=c.buckets[i].load();
            if(seen>=target) return i<13?bucketNs[i]/1e6:c.peak_ns.load()/1e6;
        }
        return c.peak_ns.load()/1e6;
    }
    inline std::string Report(const Capture& capture) {
        std::ostringstream out; out<<std::fixed<<std::setprecision(3);
        const auto now=Clock::now();
        const double seconds=(std::max)(0.0,(std::min)(10.0,std::chrono::duration<double>(now-capture.begin).count()));
        out<<"Profiler v3 | built "<<__DATE__<<" "<<__TIME__<<" | hook-registry / live targets / wall-trace build\n";
        out<<"Settings at capture request: "<<capture.settings<<"\n";
        out<<"Aim / skin profile: "<<seconds<<" seconds, in-flight scopes "<<capture.pending.load()<<"\n";
        out<<"Self time excludes instrumented children. Percentiles are histogram bucket upper bounds. Capture adds timing overhead; no per-call logging.\n";
        out<<"Inclusive timings; nested rows overlap. Aggregate thread time, not frame time.\n";
        for(int i=0;i<Count;++i) {
            const auto& c=capture.counters[i]; const auto calls=c.calls.load();
            const double ms=c.total_ns.load()/1e6;
            out<<names[i]<<": "<<calls<<" calls, "<<(seconds>0?calls/seconds:0)<<"/s; "
               <<ms<<" ms total, "<<(calls?ms/calls:0)<<" ms avg, "<<c.peak_ns.load()/1e6<<" ms peak; "<<c.self_ns.load()/1e6<<" ms self; p50 <= "<<Percentile(c,.5)<<" ms, p95 <= "<<Percentile(c,.95)<<" ms, p99 <= "<<Percentile(c,.99)<<" ms\n";
        }
        out<<"Memory/lookup attribution (inclusive ms):\n";
        for(int i=RegionQuery;i<Count;++i)
            out<<names[i]<<": callback="<<capture.byRootNs[Callback][i].load()/1e6
               <<", skins="<<capture.byRootNs[Skins][i].load()/1e6<<"\n";
        for(int e=0;e<EventCount;++e) {
            out<<eventNames[e]<<": "<<capture.events[e].load()<<" (callback "<<capture.attributed[Callback][e].load()<<", skins "<<capture.attributed[Skins][e].load()<<")\n";
        }
        return out.str();
    }
}
