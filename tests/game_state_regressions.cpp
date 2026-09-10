#include <cassert>
#include <vector>
#include <iostream>
#include "../manager/core/game_state.cpp"

namespace interfaces { CGameEntitySystem* GameEntitySystem() { return nullptr; } }
namespace sdk {
std::uint8_t* find_pattern(const char*, const char*) { return nullptr; }
std::uint8_t* resolve_absolute_rip_address(std::uint8_t*, std::size_t, std::size_t) { return nullptr; }
}
namespace error_logger {
ErrorLogger& ErrorLogger::Get() { static ErrorLogger logger; return logger; }
void ErrorLogger::Log(const char*, const char*, int) {}
}
struct Buffer {
    std::vector<unsigned char> bytes;
    explicit Buffer(size_t size) : bytes(size) {}
    uintptr_t addr() { return reinterpret_cast<uintptr_t>(bytes.data()); }
    template<class T> void put(size_t offset,T value) {
        assert(offset+sizeof(T)<=bytes.size());
        std::memcpy(bytes.data()+offset,&value,sizeof(value));
    }
};
static uintptr_t returned_pawn = 0;
static unsigned getter_calls = 0;
static uintptr_t __fastcall TestPawnGetter(int slot) {
    assert(slot == -1);
    ++getter_calls;
    return returned_pawn;
}
int main() {
    assert(!game_state::ResolveSnapshot(0,0).in_game);
    using namespace cs2_dumper::offsets::client_dll;
    Buffer client((std::max)(dwLocalPlayerPawn,dwLocalPlayerController)+sizeof(uintptr_t));
    Buffer system(0x100), controller(0x1000), pawn(0x100);
    client.put(dwLocalPlayerController,controller.addr());
    // A controller alone must not substitute for the direct pawn global.
    auto missing=game_state::ResolveSnapshot(client.addr(),system.addr());
    assert(!missing.in_game && !missing.pawn);
    client.put(dwLocalPlayerPawn,pawn.addr());
    // No identity, controller flag, handle, or entity table is needed.
    auto direct=game_state::ResolveSnapshot(client.addr(),system.addr());
    assert(direct.in_game && direct.pawn==pawn.addr());
    Buffer replacement_controller(0x1000);
    uintptr_t controller_slot=replacement_controller.addr();
    auto resolved=game_state::ResolveSnapshot(client.addr(),system.addr(),nullptr,
        reinterpret_cast<uintptr_t>(&controller_slot));
    assert(resolved.controller==replacement_controller.addr());
    controller_slot=0;
    auto cleared=game_state::ResolveSnapshot(client.addr(),system.addr(),nullptr,
        reinterpret_cast<uintptr_t>(&controller_slot));
    assert(cleared.in_game && !cleared.controller); // Do not revive the stale offset value.
    client.put<uintptr_t>(dwLocalPlayerController,0);
    auto independent=game_state::ResolveSnapshot(client.addr(),0);
    assert(independent.in_game && independent.pawn==pawn.addr() && !independent.controller);
    auto primary=game_state::ResolveSnapshot(client.addr(),0,&TestPawnGetter);
    assert(primary.pawn==pawn.addr() && getter_calls==0);
    client.put<uintptr_t>(dwLocalPlayerPawn,1);
    returned_pawn=pawn.addr();
    auto fallback=game_state::ResolveSnapshot(client.addr(),0,&TestPawnGetter);
    assert(fallback.in_game && fallback.pawn==pawn.addr() && getter_calls==1);
    assert(std::strstr(fallback.status,"GetLocalPlayerPawn"));
    returned_pawn=0;
    assert(!game_state::ResolveSnapshot(client.addr(),0,&TestPawnGetter).in_game);
    auto stale=game_state::ResolveSnapshot(client.addr(),system.addr());
    assert(!stale.in_game && !stale.pawn);
    assert(std::strstr(stale.status,"dwLocalPlayerPawn"));
    game_state::Publish(direct);
    assert(game_state::GetSnapshot().in_game);
    game_state::Publish(stale);
    assert(!game_state::IsInGame() && !game_state::GetLocalPawnRaw());
    game_state::Stop();
    assert(!game_state::GetEntityList() && !game_state::GetLocalController());
    std::cout << "PASS: direct pawn read, getter fallback, controller-slot precedence/clearing, invalid pawn rejection, and state clearing\n";
}
