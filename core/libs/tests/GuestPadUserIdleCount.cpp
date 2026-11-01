#include "SceTypes.hpp"
#include "prx/libScePad/include/Pad.hpp"
#include <cstdlib>
#include <cstdint>
#include <initializer_list>

static void Require(bool value) { if (!value) std::abort(); }

int main() {
    for (int userId : {-1, 0, 1, 15, 16, 12345}) {
        Require(scePadGetIdleCount(userId) == 0);
    }

    Require(scePadInit_nid_postfix() == PAD_OK);

    for (int userId : {-1, 0, 1, 15, 16, 12345, 0x7FFFFFFF}) {
        Require(scePadGetIdleCount(userId) == 0);
    }
}
