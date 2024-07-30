#ifndef QRK_DEBUG
#define QRK_DEBUG

#define _CRT_SECURE_NO_WARNINGS // NOLINT(*-reserved-identifier)

#include <Windows.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

namespace qrk::debug {
enum Error {
    Q_NO_ERROR = 0,
    Q_DEFAULT_ERROR = 1,
    //graphics API errors
    Q_FAILED_TO_COMPILE_SHADER,
    Q_FAILED_TO_CREATE_WINDOW,
    Q_FAILED_TO_CREATE_CONTEXT,
    Q_INVALID_DRAW,
    //filesystem errors
    Q_FAILED_TO_FIND_FILE,
    Q_FAILED_TO_OPEN_FILE,
    Q_FAILED_TO_LOAD_IMAGE,
    Q_FAILED_TO_LOAD_FONT,
    Q_LOADING_ERROR,
    //programming errors
    Q_INVALID_ENUM,
    Q_RUNTIME_ERROR
};

void OpenLogFile(const std::string &path = "logs");
void Log(const std::string &log);
void LogWarning(const std::string &warning);
void LogError(const std::string &error);

inline void ShowErrorBox(const std::string &error) {
    MessageBox(nullptr, error.c_str(), "Error", MB_OK | MB_ICONERROR);
}
inline void ShowWarningBox(const std::string &error) {
    MessageBox(nullptr, error.c_str(), "Warning", MB_OK | MB_ICONEXCLAMATION);
}
inline void Error(const std::string &error,
                  int code = qrk::debug::Q_DEFAULT_ERROR) {
    LogError(error);
    ShowErrorBox(error);
    throw std::exception(std::to_string(code).c_str());
}
inline void Warning(const std::string &error) {
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
}// namespace qrk::debug

#undef _CRT_SECURE_NO_WARNINGS
#endif// !QRK_DEBUG
