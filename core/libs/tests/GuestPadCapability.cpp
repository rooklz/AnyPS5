#include "SceTypes.hpp"
#include "prx/libScePad/include/Pad.hpp"
#include <cstdlib>
#include <initializer_list>

static void Require(bool value) { if (!value) std::abort(); }

int main() {
    std::uint32_t capability = 0xFFFFFFFFu;
    for (int deviceId : {-1, 0, 1, 12345}) {
        Require(scePadGetCapability(deviceId, nullptr) == PAD_ERROR_NOT_INITIALIZED);
        Require(scePadGetCapability(deviceId, &capability) == PAD_ERROR_NOT_INITIALIZED);
    }
    Require(capability == 0xFFFFFFFFu);

    Require(scePadInit_nid_postfix() == PAD_OK);
    for (int deviceId : {-1, 0, 1, 12345})
        Require(scePadGetCapability(deviceId, nullptr) == PAD_ERROR_INVALID_ARG);
    Require(capability == 0xFFFFFFFFu);
}
