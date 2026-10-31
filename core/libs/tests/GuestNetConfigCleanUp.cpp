#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <source_location>
#include "prx/libSceNet/libSceNet.hpp"

static void Require(bool condition, std::source_location where = std::source_location::current()) {
    if (!condition) {
        std::fprintf(stderr, "check failed at line %u\n", where.line());
        std::abort();
    }
}

int main() {
    *sceNetErrnoLoc() = -1;
    for (int call = 0; call < 3; ++call) Require(sceNetConfigCleanUpAllInterfaces() == 0);
    Require(*sceNetErrnoLoc() == -1);
    Require(sceNetInit_nid_postfix() == 0);
    *sceNetErrnoLoc() = -1;
    for (int call = 0; call < 3; ++call) Require(sceNetConfigCleanUpAllInterfaces() == 0);
    Require(*sceNetErrnoLoc() == -1);
    Require(sceNetTerm() == 0);
    *sceNetErrnoLoc() = -1;
    for (int call = 0; call < 3; ++call) Require(sceNetConfigCleanUpAllInterfaces() == 0);
    Require(*sceNetErrnoLoc() == -1);
    return 0;
}
