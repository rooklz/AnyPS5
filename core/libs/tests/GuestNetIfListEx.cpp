#include "prx/libSceNet/include/Net.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

namespace {

void Require(bool value) { if (!value) std::abort(); }

constexpr int kErrorBase = static_cast<int>(0x80410100);
constexpr int kEinval = 22;
constexpr int kEnotinit = 200;

bool Failed(int result, int error) { return result == (kErrorBase | error) && *sceNetErrnoLoc() == error; }

void CheckUninitialized() {
    NetInterfaceInfo info{};
    std::memset(&info, 0x5a, sizeof(info));
    const NetInterfaceInfo before = info;
    for (const std::int32_t id : {0, 1, 9, 10, -2}) {
        Require(Failed(sceNetGetIfListEx(id, &info, 1, 0), kEnotinit));
        Require(Failed(sceNetGetIfListEx(id, &info, 1, 1), kEnotinit));
        Require(Failed(sceNetGetIfListEx(id, nullptr, 0, 0), kEnotinit));
        Require(std::memcmp(&info, &before, sizeof(info)) == 0);
    }
}

void CheckArguments() {
    NetInterfaceInfo info{};
    std::memset(&info, 0x5a, sizeof(info));
    const NetInterfaceInfo before = info;
    Require(Failed(sceNetGetIfListEx(0, &info, 1, 2), kEinval));
    Require(Failed(sceNetGetIfListEx(0, &info, 1, 0xffffffffu), kEinval));
    Require(Failed(sceNetGetIfListEx(0, nullptr, 1, 0), kEinval));
    Require(Failed(sceNetGetIfListEx(0, &info, 0, 0), kEinval));
    Require(Failed(sceNetGetIfListEx(0, &info, 2, 0), kEinval));
    Require(Failed(sceNetGetIfListEx(0, nullptr, 2, 0), kEinval));
    Require(std::memcmp(&info, &before, sizeof(info)) == 0);
    for (const std::int32_t id : {10, 11, -2, -2147483647}) {
        for (const std::uint32_t flags : {0u, 1u}) {
            *sceNetErrnoLoc() = 0;
            Require(sceNetGetIfListEx(id, &info, 1, flags) == 0);
            Require(*sceNetErrnoLoc() == kEinval);
            Require(sceNetGetIfListEx(id, nullptr, 0, flags) == 0);
            Require(*sceNetErrnoLoc() == kEinval);
            Require(std::memcmp(&info, &before, sizeof(info)) == 0);
        }
    }
}

void CheckLookup() {
    NetInterfaceInfo info{};
    std::memset(&info, 0x5a, sizeof(info));
    for (const std::uint32_t flags : {0u, 1u}) {
        *sceNetErrnoLoc() = 0;
        Require(sceNetGetIfListEx(0, &info, 1, flags) == 1);
        Require(std::memcmp(info.name, "lo0", 4) == 0);
        for (std::size_t index = 4; index < sizeof(info); ++index) {
            Require(reinterpret_cast<const std::uint8_t*>(&info)[index] == 0);
        }
        *sceNetErrnoLoc() = 0;
        Require(sceNetGetIfListEx(0, nullptr, 0, flags) == 1);
        Require(*sceNetErrnoLoc() == 0);
        for (const std::int32_t id : {1, 2, 3, 4, 5, 6, 7, 8, 9}) {
            std::memset(&info, 0x5a, sizeof(info));
            *sceNetErrnoLoc() = 0;
            Require(sceNetGetIfListEx(id, &info, 1, flags) == 0);
            Require(*sceNetErrnoLoc() == 0);
            const std::uint8_t first = 0x5a;
            Require(reinterpret_cast<const std::uint8_t*>(&info)[0] == first);
        }
    }
}

}

int main() {
    CheckUninitialized();
    Require(sceNetInit_nid_postfix() == 0);
    CheckArguments();
    CheckLookup();
    Require(sceNetTerm() == 0);
    CheckUninitialized();
    return 0;
}
