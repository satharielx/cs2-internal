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
    client.put<uintptr_t>(dwLocalPlayerController,0);
    auto independent=game_state::ResolveSnapshot(client.addr(),0);
    assert(independent.in_game && independent.pawn==pawn.addr() && !independent.controller);
    client.put<uintptr_t>(dwLocalPlayerPawn,1);
    auto stale=game_state::ResolveSnapshot(client.addr(),system.addr());
    assert(!stale.in_game && !stale.pawn);
    assert(std::strstr(stale.status,"dwLocalPlayerPawn"));
    game_state::Publish(direct);
    assert(game_state::GetSnapshot().in_game);
    game_state::Publish(stale);
    assert(!game_state::IsInGame() && !game_state::GetLocalPawnRaw());
    game_state::Stop();
    assert(!game_state::GetEntityList() && !game_state::GetLocalController());
    std::cout << "PASS: direct pawn-global read, independence from controller/handles/entity table, invalid pawn rejection, and state clearing\n";
}
