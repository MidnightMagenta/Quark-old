#include "../include/object.hpp"

void qrk::Object::LoadObjectAsync(std::string path, std::promise<std::vector<GLfloat>> _promisedData)
{
	//get data from file 
	std::ifstream objFile(path);
	qrk::Object::ObjectData object;
	std::string dataBuffer;
	while (std::getline(objFile, dataBuffer))
	{
		int pos = 0;
		std::string breakBuffer;
		std::vector<std::string> buffer;
		
		std::stringstream data;
		data << dataBuffer;
		while (std::getline(data, breakBuffer, ' '))
		{
			buffer.push_back(breakBuffer);
			breakBuffer.clear();
		}

		if (buffer[0] == "v")
		{
			qrk::vec4f bufferVertexVector({ std::stof(buffer[1]), std::stof(buffer[2]), std::stof(buffer[3]), 1.f });
			object.vertices.push_back(bufferVertexVector);
		}
		if (buffer[0] == "vt")
		{
			qrk::vec2f bufferTextureVector({ std::stof(buffer[1]), std::stof(buffer[2]) });
			object.textures.push_back(bufferTextureVector);
		}
		if (buffer[0] == "vn")
		{
			qrk::vec3f bufferNormalVector({ std::stof(buffer[1]), std::stof(buffer[2]), std::stof(buffer[3]) });
			object.normals.push_back(bufferNormalVector);
		}
		if (buffer[0] == "f")
		{
			std::stringstream sBuffer;
			for (int i = 1; i < buffer.size(); i++)
			{
				sBuffer << buffer[i];
				std::string oBuffer;
				std::vector<std::string> osBuffer;
				while (std::getline(sBuffer, oBuffer, '/'))
				{
					osBuffer.push_back(oBuffer);
					oBuffer.clear();
				}
				object.vertexIndeces.push_back(std::stoi(osBuffer[0]));
				object.textureIndeces.push_back(std::stoi(osBuffer[1]));
				object.normalIndeces.push_back(std::stoi(osBuffer[2]));
				osBuffer.clear();
				sBuffer.clear();
			}
		}

		//clean up
		buffer.clear();
		breakBuffer.clear();
		data.clear();
		dataBuffer.clear();
	}

	objFile.close();

	//check if the data was loaded succesfully 
	if (!(object.vertexIndeces.size() == object.textureIndeces.size()) && !(object.vertexIndeces.size() == object.normalIndeces.size()))
	{
		std::string error = "Failed to load object at: " + path + " Index arrays have different sizes";
		MessageBox(0, error.c_str(), "Error", MB_OK);
		return;
	}
	for (unsigned int element : object.vertexIndeces)
	{
		if (element > object.vertices.size())
		{
			std::string error = "Failed to load object at: " + path + " Out of range exception (vertices)";
			MessageBox(0, error.c_str(), "Error", MB_OK);
			return;
		}
	}
	for (unsigned int element : object.textureIndeces)
	{
		if (element > object.textures.size())
		{
			std::string error = "Failed to load object at: " + path + " Out of range exception (textures)";
			MessageBox(0, error.c_str(), "Error", MB_OK);
			return;
		}
	}
	for (unsigned int element : object.normalIndeces)
	{
		if (element > object.normals.size())
		{
			std::string error = "Failed to load object at: " + path + " Out of range exception (normals)";
			MessageBox(0, error.c_str(), "Error", MB_OK);
			return;
		}
	}

	//parse data into a 1d array of GLfloats
	std::vector<qrk::vec4f> alignedVertices;
	std::vector<qrk::vec2f> alignedTextures;
	std::vector<qrk::vec3f> alignedNormals;

	for (int i = 0; i < object.vertexIndeces.size(); i++)
	{
		alignedVertices.push_back(object.vertices[object.vertexIndeces[i] - 1]);
		alignedTextures.push_back(object.textures[object.textureIndeces[i] - 1]);
		alignedNormals.push_back(object.normals[object.normalIndeces[i] - 1]);
	}

	if (!(alignedVertices.size() == alignedTextures.size()) && !(alignedVertices.size() == alignedNormals.size()))
	{
		std::string error = "Failed to load object at: " + path + " Aligned arrays have different sizes";
		MessageBox(0, error.c_str(), "Error", MB_OK);
		return;
	}

	std::vector<GLfloat> returnData;

	for (int i = 0; i < alignedVertices.size(); i++)
	{
		returnData.push_back(alignedVertices[i].x());
		returnData.push_back(alignedVertices[i].y());
		returnData.push_back(alignedVertices[i].z());
		returnData.push_back(alignedVertices[i].w());

		returnData.push_back(alignedTextures[i].x());
		returnData.push_back(alignedTextures[i].y());

		returnData.push_back(alignedNormals[i].x());
		returnData.push_back(alignedNormals[i].y());
		returnData.push_back(alignedNormals[i].z());
	}

	//clean up
	alignedVertices.clear(); alignedVertices.shrink_to_fit();
	alignedTextures.clear(); alignedTextures.shrink_to_fit();
	alignedNormals.clear(); alignedNormals.shrink_to_fit();
	object.Clear();

	//return data
	_promisedData.set_value(returnData);
	loadFinished = true;
}

void qrk::Object::LoadObject(std::string path)
{
	//get data from file 
	std::ifstream objFile(path);
	qrk::Object::ObjectData object;
	std::string dataBuffer;
	while (std::getline(objFile, dataBuffer))
	{
		int pos = 0;
		std::string breakBuffer;
		std::vector<std::string> buffer;

		std::stringstream data;
		data << dataBuffer;
		while (std::getline(data, breakBuffer, ' '))
		{
			buffer.push_back(breakBuffer);
			breakBuffer.clear();
		}

		if (buffer[0] == "v")
		{
			qrk::vec4f bufferVertexVector({ std::stof(buffer[1]), std::stof(buffer[2]), std::stof(buffer[3]), 1.f });
			object.vertices.push_back(bufferVertexVector);
		}
		if (buffer[0] == "vt")
		{
			qrk::vec2f bufferTextureVector({ std::stof(buffer[1]), std::stof(buffer[2]) });
			object.textures.push_back(bufferTextureVector);
		}
		if (buffer[0] == "vn")
		{
			qrk::vec3f bufferNormalVector({ std::stof(buffer[1]), std::stof(buffer[2]), std::stof(buffer[3]) });
			object.normals.push_back(bufferNormalVector);
		}
		if (buffer[0] == "f")
		{
			std::stringstream sBuffer;
			for (int i = 1; i < buffer.size(); i++)
			{
				sBuffer << buffer[i];
				std::string oBuffer;
				std::vector<std::string> osBuffer;
				while (std::getline(sBuffer, oBuffer, '/'))
				{
					osBuffer.push_back(oBuffer);
					oBuffer.clear();
				}
				object.vertexIndeces.push_back(std::stoi(osBuffer[0]));
				object.textureIndeces.push_back(std::stoi(osBuffer[1]));
				object.normalIndeces.push_back(std::stoi(osBuffer[2]));
				osBuffer.clear();
				sBuffer.clear();
			}
		}
		//clean up
		buffer.clear();
		breakBuffer.clear();
		data.clear();
		dataBuffer.clear();
	}

	objFile.close();

	//check if data was loaded sucessfully 
	if (!(object.vertexIndeces.size() == object.textureIndeces.size()) && !(object.vertexIndeces.size() == object.normalIndeces.size()))
	{
		std::string error = "Failed to load object at: " + path + " Index arrays have different sizes";
		MessageBox(0, error.c_str(), "Error", MB_OK);
		return;
	}
	for (unsigned int element : object.vertexIndeces)
	{
		if (element > object.vertices.size())
		{
			std::string error = "Failed to load object at: " + path + " Out of range exception (vertices)";
			MessageBox(0, error.c_str(), "Error", MB_OK);
			return;
		}
	}
	for (unsigned int element : object.textureIndeces)
	{
		if (element > object.textures.size())
		{
			std::string error = "Failed to load object at: " + path + " Out of range exception (textures)";
			MessageBox(0, error.c_str(), "Error", MB_OK);
			return;
		}
	}
	for (unsigned int element : object.normalIndeces)
	{
		if (element > object.normals.size())
		{
			std::string error = "Failed to load object at: " + path + " Out of range exception (normals)";
			MessageBox(0, error.c_str(), "Error", MB_OK);
			return;
		}
	}

	//parse data into a 1d array of GLfloats
	std::vector<qrk::vec4f> alignedVertices;
	std::vector<qrk::vec2f> alignedTextures;
	std::vector<qrk::vec3f> alignedNormals;

	for (int i = 0; i < object.vertexIndeces.size(); i++)
	{
		alignedVertices.push_back(object.vertices[object.vertexIndeces[i] - 1]);
		alignedTextures.push_back(object.textures[object.textureIndeces[i] - 1]);
		alignedNormals.push_back(object.normals[object.normalIndeces[i] - 1]);
	}

	if (!(alignedVertices.size() == alignedTextures.size()) && !(alignedVertices.size() == alignedNormals.size()))
	{
		std::string error = "Failed to load object at: " + path + " Aligned arrays have different sizes";
		MessageBox(0, error.c_str(), "Error", MB_OK);
		return;
	}

	std::vector<GLfloat> returnData;

	for (int i = 0; i < alignedVertices.size(); i++)
	{
		returnData.push_back(alignedVertices[i].x());
		returnData.push_back(alignedVertices[i].y());
		returnData.push_back(alignedVertices[i].z());
		returnData.push_back(alignedVertices[i].w());

		returnData.push_back(alignedTextures[i].x());
		returnData.push_back(alignedTextures[i].y());

		returnData.push_back(alignedNormals[i].x());
		returnData.push_back(alignedNormals[i].y());
		returnData.push_back(alignedNormals[i].z());
	}

	//clean up
	alignedVertices.clear(); alignedVertices.shrink_to_fit();
	alignedTextures.clear(); alignedTextures.shrink_to_fit();
	alignedNormals.clear(); alignedNormals.shrink_to_fit();
	object.Clear();

	//return data and further clean up
	data = returnData;
	returnData.clear();
	returnData.shrink_to_fit();
}

void qrk::Object::DumpObjectData(std::string path)
{
	//check if directory exists, create and open a log file
	if (!std::filesystem::exists(path))
	{
		std::filesystem::create_directories(path);
	}
	std::stringstream filePath;
	filePath << path << "/" << "object_log" << "_" << time(NULL) << ".txt";
	std::ofstream file(filePath.str());

	//print data to the log file
	for (int i = 0; i < data.size(); i += 9)
	{
		file << "Vertex " << (i / 9) + 1 << "\nVertex coordinates:\t "
			<< data[i]	   << " | "
			<< data[i + 1] << " | "
			<< data[i + 2] << " | "
			<< data[i + 3] << " \nTexture coordinates:\t "
			<< data[i + 4] << " | "
			<< data[i + 5] << " \nNormal coordinates:\t "
			<< data[i + 6] << " | "
			<< data[i + 7] << " | "
			<< data[i + 8] << " \n" << std::endl;
	}
	//clean up
	file.close();
}