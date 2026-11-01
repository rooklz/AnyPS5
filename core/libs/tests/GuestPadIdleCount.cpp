#include "SceTypes.hpp"
#include "prx/libScePad/include/Pad.hpp"
#include <cstdlib>
#include <cstdint>
#include <initializer_list>

static void Require(bool value) { if (!value) std::abort(); }

int main() {
    std::uint64_t count = 0xaaaaaaaaaaaaaaaaull;
    for (int handle : {-1, 0, 1, 12345}) {
        Require(scePadGetControllerIdleCount(handle, &count) == PAD_ERROR_NOT_INITIALIZED);
        Require(count == 0xaaaaaaaaaaaaaaaaull);
    }

    Require(scePadInit_nid_postfix() == PAD_OK);

    count = 0xaaaaaaaaaaaaaaaaull;
    Require(scePadGetControllerIdleCount(0, &count) == PAD_ERROR_INVALID_ARG);
    Require(count == 0xaaaaaaaaaaaaaaaaull);
    Require(scePadGetControllerIdleCount(1, nullptr) == PAD_ERROR_INVALID_ARG);
    Require(scePadGetControllerIdleCount(0, nullptr) == PAD_ERROR_INVALID_ARG);

    for (int handle : {-1, 1, 12345}) {
        count = 0xaaaaaaaaaaaaaaaaull;
        Require(scePadGetControllerIdleCount(handle, &count) == PAD_OK);
        Require(count == 0);
    }

    const int handle = scePadOpen_nid_postfix(0x10000000, 0, 0, nullptr);
    Require(handle > 0);
    count = 0xaaaaaaaaaaaaaaaaull;
    Require(scePadGetControllerIdleCount(handle, &count) == PAD_OK);
    Require(count == 0);
    Require(scePadClose_nid_postfix(handle) == PAD_OK);
}
