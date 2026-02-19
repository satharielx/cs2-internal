#include "../mem.h"

class IDXGISwapChain;

namespace sdk {
    class interface_swap_chain_dx11 {
    private:
        padding<0x170> m_pad;
    public:
        IDXGISwapChain* swap_chain;
    };
}