#include <../include/app_entry_point.hpp>
#include <../include/qrk_debug.hpp>
#include <exception>

int main() {
    try {
        return run();
    } catch (...) {
        qrk::Debug::ShowErrorBox("Check log file");
        return -1;
    }
}