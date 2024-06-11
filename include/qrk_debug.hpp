#ifndef QRK_DEBUG
#define QRK_DEBUG

#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <string>
#include <sstream>

namespace qrk::Debug
{
	void OpenLogFile(std::string path = "logs");
	void Log(std::string log);
	void LogWarning(std::string warning);
	void LogError(std::string error);

	inline void ShowErrorBox(std::string error)
	{
		MessageBox(0, error.c_str(), "Error", MB_OK | MB_ICONERROR);
	}
	inline void ShowWarningBox(std::string error)
	{
		MessageBox(0, error.c_str(), "Warning", MB_OK | MB_ICONEXCLAMATION);
	}

	inline std::ofstream logFile;

	inline void CloseLog() { logFile.close(); }
}

#endif // !QRK_DEBUG
