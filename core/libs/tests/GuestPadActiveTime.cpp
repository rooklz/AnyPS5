#include "SceTypes.hpp"
#include "prx/libScePad/include/Pad.hpp"
#include <cstdlib>
#include <cstdint>
#include <initializer_list>

static void Require(bool value) { if (!value) std::abort(); }

int main() {
    std::uint64_t time = 0xaaaaaaaaaaaaaaaaull;
    for (int handle : {-1, 0, 1, 12345}) {
        Require(scePadGetActiveTime(handle, 0, &time) == PAD_ERROR_NOT_INITIALIZED);
        Require(time == 0xaaaaaaaaaaaaaaaaull);
    }

    Require(scePadInit_nid_postfix() == PAD_OK);

    time = 0xaaaaaaaaaaaaaaaaull;
    Require(scePadGetActiveTime(0, 0, nullptr) == PAD_ERROR_INVALID_ARG);
    Require(time == 0xaaaaaaaaaaaaaaaaull);
    Require(scePadGetActiveTime(1, -1, nullptr) == PAD_ERROR_INVALID_ARG);

    for (int handle : {-1, 0, 1, 12345}) {
        for (int type : {-1, 0, 1, 7}) {
            time = 0xaaaaaaaaaaaaaaaaull;
            Require(scePadGetActiveTime(handle, type, &time) == PAD_OK);
            Require(time == 0);
        }
    }

    const int handle = scePadOpen_nid_postfix(0x10000000, 0, 0, nullptr);
    Require(handle > 0);
    time = 0xaaaaaaaaaaaaaaaaull;
    Require(scePadGetActiveTime(handle, 0, &time) == PAD_OK);
    Require(time == 0);
    Require(scePadClose_nid_postfix(handle) == PAD_OK);
}
