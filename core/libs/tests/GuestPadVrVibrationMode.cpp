#include "SceTypes.hpp"
#include "prx/libScePad/include/Pad.hpp"
#include <cstdlib>
#include <initializer_list>

static void Require(bool value) { if (!value) std::abort(); }

int main() {
    for (int handle : {-1, 0, 1, 12345})
        for (unsigned mode : {0u, 1u, 4u, 0xffffffffu})
            Require(scePadVrControllerSetVibrationMode(handle, mode) == PAD_ERROR_NOT_INITIALIZED);

    Require(scePadInit_nid_postfix() == PAD_OK);
    for (int handle : {-1, 0, 12345}) {
        Require(scePadVrControllerSetVibrationMode(handle, 0) == PAD_ERROR_INVALID_ARG);
        Require(scePadVrControllerSetVibrationMode(handle, 4) == PAD_ERROR_INVALID_ARG);
        Require(scePadVrControllerSetVibrationMode(handle, 6) == PAD_ERROR_INVALID_ARG);
        Require(scePadVrControllerSetVibrationMode(handle, 0xffffffffu) == PAD_ERROR_INVALID_ARG);
        for (unsigned mode : {1u, 2u, 3u, 5u})
            Require(scePadVrControllerSetVibrationMode(handle, mode) == PAD_ERROR_INVALID_HANDLE);
    }

    const int handle = scePadOpen_nid_postfix(0x10000000, 0, 0, nullptr);
    Require(handle > 0);
    Require(scePadVrControllerSetVibrationMode(handle, 0) == PAD_ERROR_INVALID_ARG);
    Require(scePadVrControllerSetVibrationMode(handle, 1) == PAD_OK);
    Require(scePadVrControllerSetVibrationMode(handle, 2) == PAD_OK);
    Require(scePadVrControllerSetVibrationMode(handle, 3) == PAD_OK);
    Require(scePadVrControllerSetVibrationMode(handle, 4) == PAD_ERROR_INVALID_ARG);
    Require(scePadVrControllerSetVibrationMode(handle, 5) == PAD_OK);
    Require(scePadVrControllerSetVibrationMode(handle, 6) == PAD_ERROR_INVALID_ARG);
    Require(scePadClose_nid_postfix(handle) == PAD_OK);

    const int special = scePadOpen_nid_postfix(0x10000000, PAD_PORT_TYPE_SPECIAL, 0, nullptr);
    Require(special > 0);
    Require(scePadVrControllerSetVibrationMode(special, 1) == PAD_OK);
    Require(scePadVrControllerSetVibrationMode(special, 4) == PAD_ERROR_INVALID_ARG);
    Require(scePadClose_nid_postfix(special) == PAD_OK);
}
