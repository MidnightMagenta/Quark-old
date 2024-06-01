#ifndef QRK_OBJECT
#define QRK_OBJECT

#include "../include/vector.hpp"
#include <Windows.h>
#include "../glad/glad.h"
#include <fstream>
#include <filesystem>
#include <thread>
#include <future>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>

namespace qrk
{
	class Object
	{
	public:
		Object(std::string path, bool async = true) : loadFinished(false), asyncLoad(async)
		{
			if(async)
			{
				_path = path;
				futureData = promisedData.get_future();
				std::thread loadThread(std::bind(&qrk::Object::LoadObjectAsync, this, std::placeholders::_1, std::placeholders::_2), path, std::move(promisedData));
				loadThread.detach();
			}
			else
			{
				LoadObject(path);
			}
		};

		bool WaitForLoad()
		{
			if (asyncLoad)
			{
				if (loadFinished)
				{
					data = futureData.get();
					return false;
				}
				else return true;
			}
			else return false;
		}

		void PrintObjectData()
		{
			for (int i = 0; i < data.size(); i += 9)
			{
				std::cout << "Vertex " << (i / 9) + 1 << "\nVertex coordinates:\t "
					<< data[i] << " | "
					<< data[i + 1] << " | "
					<< data[i + 2] << " | "
					<< data[i + 3] << " \nTexture coordinates:\t "
					<< data[i + 4] << " | "
					<< data[i + 5] << " \nNormal coordinates:\t "
					<< data[i + 6] << " | "
					<< data[i + 7] << " | "
					<< data[i + 8] << " \n" << std::endl;
			}
		}

		void DumpObjectData(std::string path = "logs");

	private:
		void LoadObjectAsync(std::string path, std::promise<std::vector<GLfloat>> _promisedData);
		void LoadObject(std::string path);

		std::string _path;
		std::atomic_bool loadFinished;
		bool asyncLoad;
		std::promise<std::vector<GLfloat>> promisedData;
		std::future<std::vector<GLfloat>> futureData;
		std::vector<GLfloat> data;

		struct ObjectData
		{
			std::vector<qrk::vec4f> vertices;
			std::vector<qrk::vec2f> textures;
			std::vector<qrk::vec3f> normals;

			std::vector<int> vertexIndeces;
			std::vector<int> textureIndeces;
			std::vector<int> normalIndeces;

			void Clear()
			{
				vertices.clear();
				vertices.shrink_to_fit();
				textures.clear();
				textures.shrink_to_fit();
				normals.clear();
				normals.shrink_to_fit();
				vertexIndeces.clear();
				vertexIndeces.shrink_to_fit();
				textureIndeces.clear();
				textureIndeces.shrink_to_fit();
				normalIndeces.clear();
				normalIndeces.shrink_to_fit();
			}
		};
	};
}

#endif // !QRK_OBJECT
