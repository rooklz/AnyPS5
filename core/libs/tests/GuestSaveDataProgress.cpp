#include "prx/libSceSaveData/include/Api.hpp"
#include "prx/libSceSaveData.native/include/Api.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdio>

namespace {

constexpr int SaveDataErrorParameter = -2137063424;
constexpr int SaveDataErrorNotInitialized = -2137063423;

bool Check(int actual, int expected, const char* what) {
    if (actual == expected) return true;
    std::fprintf(stderr, "sceSaveDataGetProgress %s returned %d, expected %d\n", what, actual, expected);
    return false;
}

}

int main() {
    float progress = -1.0f;
    if (!Check(sceSaveDataGetProgress(&progress), SaveDataErrorNotInitialized, "before initialization")) return 1;
    if (progress != -1.0f) {
        std::fprintf(stderr, "sceSaveDataGetProgress wrote progress before initialization\n");
        return 1;
    }
    if (sceSaveDataInitialize3(nullptr) != 0) {
        std::fprintf(stderr, "SaveData did not initialize\n");
        return 1;
    }
    if (!Check(sceSaveDataGetProgress(&progress), SaveDataErrorParameter, "with no operation in progress")) return 1;
    if (progress != -1.0f) {
        std::fprintf(stderr, "sceSaveDataGetProgress wrote progress with no operation in progress\n");
        return 1;
    }
    if (sceSaveDataTerminate() != 0) {
        std::fprintf(stderr, "SaveData did not terminate\n");
        return 1;
    }
    if (!Check(sceSaveDataGetProgress(nullptr), SaveDataErrorNotInitialized, "after termination")) return 1;
    std::puts("save data progress calls: PASS");
    return 0;
}
