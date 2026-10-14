#pragma once

namespace menu_mouse {
    // Calls are serialized by the hook's mutex. SetBlocked runs on the window
    // thread, never in Present: SDL requires mouse-mode changes on its main thread.
    class State {
        bool blocked_ = false;
        bool requested_ = false;
    public:
        bool Request(bool enabled) {
            requested_ = enabled;
            return blocked_ ? false : enabled;
        }
        template<class Read, class Apply>
        void SetBlocked(bool blocked, Read read, Apply apply) {
            if (blocked == blocked_) return;
            if (blocked) requested_ = read();
            blocked_ = blocked;
            apply(blocked ? false : requested_);
        }
        void Reset() { *this = {}; }
    };
}
