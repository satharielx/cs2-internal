#include "../manager/core/menu_mouse.h"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    menu_mouse::State state;
    bool current = true;
    std::vector<bool> changes;
    auto read = [&] { return current; };
    auto apply = [&](bool enabled) { current = enabled; changes.push_back(enabled); };
    state.SetBlocked(true, read, apply);
    assert(!current && changes.size() == 1);
    state.SetBlocked(true, read, apply); // Menu plus console stays blocked.
    assert(changes.size() == 1);
    assert(!state.Request(true)); // The game cannot recapture the menu cursor.
    state.SetBlocked(false, read, apply);
    assert(current && changes.size() == 2);
    state.SetBlocked(true, read, apply);
    assert(!state.Request(false)); // Preserve a changed game request on close.
    state.SetBlocked(false, read, apply);
    assert(!current);
    state.Reset(); current = false;
    state.SetBlocked(true, read, apply);
    state.SetBlocked(false, read, apply);
    assert(!current); // Opening from the game's own menu must not enable capture.
    assert(state.Request(true));
    current = true;
    state.SetBlocked(true, read, apply);
    state.SetBlocked(false, read, apply); // Unload also restores game ownership.
    assert(current);
    current = false; // Game switched to its UI without another setter request.
    state.SetBlocked(true, read, apply);
    state.SetBlocked(false, read, apply);
    assert(!current); // Each opening captures the current mode again.
    std::cout << "Menu mouse regressions passed\n";
}
