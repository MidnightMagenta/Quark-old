#include <../include/app_entry_point.hpp>
#include <../include/qrk_debug.hpp>
#include <exception>

int main() {
    try {
        return run();
    } catch (...) {
        return -1;
    }
}