#include <../include/misc_functions.hpp>
#include <../include/object.hpp>

//define entry point of the application
int run() {
    return 1;
}

#include <../include/qrk_debug.hpp>
#ifdef SUBSYSTEM_CONSOLE
int main(){
    try {
        return run();
    } catch (std::exception &e) {
        try {
            return std::stoi(e.what());
        } catch (...) {
            const std::string error(e.what());
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
            return -1;
        }
    } catch (...) {
        qrk::debug::LogError("Unhandled exception");
        return -1;
    }
}
#elif SUBSYSTEM_WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    try {
        return run();
    } catch (std::exception &e) {
        try {
            return std::stoi(e.what());
        } catch (...) {
            const std::string error(e.what());
            qrk::debug::ShowErrorBox(error);
            qrk::debug::LogError(error);
            return -1;
        }
    } catch (...) {
        qrk::debug::LogError("Unhandled exception");
        return -1;
    }
}
#endif