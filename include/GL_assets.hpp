#ifndef QRK_GL_ASSETS
#define QRK_GL_ASSETS

#include "../include/qrk_debug.hpp"
#include "../glad/glad.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace qrk::assets
{
	class Program
	{
	public:
		Program() = delete;
		Program(std::string vertexPath, std::string fragmentPath)
		{
			programHandle = glCreateProgram();
			if (!Compile(vertexPath, fragmentPath))
			{
				qrk::Debug::LogError("Failed to compile shader. Vertex path: " + vertexPath + " Fragment shader: " + fragmentPath);
			}
		}

		GLuint programHandle;

	private:
		bool Compile(std::string vertexPath, std::string fragmentPath);
	};
}

#endif // !QRK_GL_ASSETS
