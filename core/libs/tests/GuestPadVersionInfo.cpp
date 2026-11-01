#include "SceTypes.hpp"
#include "prx/libScePad/include/Pad.hpp"
#include <cstdlib>
#include <initializer_list>

static void Require(bool value) { if (!value) std::abort(); }

int main() {
    PadVersionInfo* info = reinterpret_cast<PadVersionInfo*>(0x1);
    for (int handle : {-1, 0, 1, 12345}) {
        Require(scePadGetVersionInfo(handle, nullptr) == PAD_ERROR_NOT_INITIALIZED);
        Require(scePadGetVersionInfo(handle, info) == PAD_ERROR_NOT_INITIALIZED);
    }

    Require(scePadInit_nid_postfix() == PAD_OK);
    for (int handle : {-1, 0, 1, 12345})
        Require(scePadGetVersionInfo(handle, nullptr) == PAD_ERROR_INVALID_ARG);
}
