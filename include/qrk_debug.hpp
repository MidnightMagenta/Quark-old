#ifndef QRK_DEBUG
#define QRK_DEBUG

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace qrk::Debug {
void OpenLogFile(std::string path = "logs");
void Log(std::string log);
void LogWarning(std::string warning);
void LogError(std::string error);

inline void ShowErrorBox(std::string error) {
    MessageBox(0, error.c_str(), "Error", MB_OK | MB_ICONERROR);
}
inline void ShowWarningBox(std::string error) {
    MessageBox(0, error.c_str(), "Warning", MB_OK | MB_ICONEXCLAMATION);
}
inline void Error(std::string error, int code = -1) {
    LogError(error);
    ShowErrorBox(error);
    throw std::exception(std::to_string(code).c_str());
}
inline void Warning(std::string error) {
    LogWarning(error);
#ifdef _DEBUG
    ShowWarningBox(error);
#endif// _DEBUG
}
inline void set_cursor(int x = 0, int y = 0) {
    HANDLE handle;
    COORD coordinates;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(handle, coordinates);
}

inline std::ofstream logFile;

inline void CloseLog() { logFile.close(); }
class FrameCounter {
public:
    FrameCounter() : frameStart(std::chrono::steady_clock::now()) {}
    float GetFrameTime() {
        float frameTime = std::chrono::duration<float, std::milli>(
                                  std::chrono::steady_clock::now() - frameStart)
                                  .count();
        frameStart = std::chrono::steady_clock::now();
        return frameTime;
    }
    float GetFrameRate() {
        float frameTime = GetFrameTime();
        return 1000.f / frameTime;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> frameStart;
};
}// namespace qrk::Debug

#undef _CRT_SECURE_NO_WARNINGS
#endif// !QRK_DEBUG
