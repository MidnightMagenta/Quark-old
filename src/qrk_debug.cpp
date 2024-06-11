#include "../include/qrk_debug.hpp"

void qrk::Debug::OpenLogFile(std::string path) {
    if (std::filesystem::exists(path)) { std::filesystem::remove_all(path); }
    std::filesystem::create_directory(path);
    std::stringstream fullPath;
    fullPath << path << "/log_" << time(NULL) << ".txt";
    logFile.open(fullPath.str());
    if (!logFile.is_open()) {
        MessageBox(0, "Failed to open log file", "Warning",
                   MB_OK | MB_ICONEXCLAMATION);
    }
}
void qrk::Debug::Log(std::string log) {
    if (!logFile.is_open()) { OpenLogFile(); }
    time_t tt;
    time(&tt);
    logFile << "[INFO] Time: " << std::put_time(localtime(&tt), "%c")
            << " Message: " << log << std::endl;
}
void qrk::Debug::LogWarning(std::string warning) {
    if (!logFile.is_open()) { OpenLogFile(); }
    time_t tt;
    time(&tt);
    logFile << "[WARNING] Time: " << std::put_time(localtime(&tt), "%c")
            << " Message: " << warning << std::endl;
}
void qrk::Debug::LogError(std::string error) {
    if (!logFile.is_open()) { OpenLogFile(); }
    time_t tt;
    time(&tt);
    logFile << "[ERROR] Time: " << std::put_time(localtime(&tt), "%c")
            << " Message: " << error << std::endl;
}