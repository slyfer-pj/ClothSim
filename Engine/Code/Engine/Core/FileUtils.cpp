#include <fstream>
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

bool DoesFileExist(const std::string& fileName)
{
	std::ifstream fileStream(fileName.c_str());
	return fileStream.good();
}

int FileReadToBuffer(std::vector<uint8_t>& outBuffer, const std::string& filename)
{
	std::ifstream readfileStream;
	readfileStream.open(filename, std::ifstream::binary);
	if (readfileStream.is_open())
	{
		readfileStream.seekg(0, readfileStream.end);
		int length = static_cast<int>(readfileStream.tellg());
		readfileStream.seekg(0, readfileStream.beg);

		char* readBuffer = new char[length];
		readfileStream.read(readBuffer, length);
		outBuffer.resize(length);
		memcpy(outBuffer.data(), readBuffer, sizeof(char) * length);
		delete[] readBuffer;
		readfileStream.close();
		return 0;
	}
	else
	{
		std::string errorString = "Failed to open ";
		errorString.append(filename);
		ERROR_AND_DIE(errorString);
		return -1;
	}
}

int FileReadToString(std::string& outString, const std::string& filename)
{
	std::vector<uint8_t> buffer;
	int result= FileReadToBuffer(buffer, filename);
	buffer.push_back('\0');
	outString.assign(buffer.begin(), buffer.end());
	return result;
}

int BufferWriteToFile(const std::vector<uint8_t>& buffer, const std::string& filename, bool createFileIfItDoesNotExist)
{
	if (DoesFileExist(filename) && !createFileIfItDoesNotExist)
	{
		std::string errorString = "Failed to open ";
		errorString.append(filename);
		ERROR_AND_DIE(errorString);
		return -1;
	}

	//creates file even if it doesnt exist, or just opens file if it exists.
	std::ofstream writefileStream;
	writefileStream.open(filename, std::ifstream::binary);

	int length = static_cast<int>(buffer.size());
	char* writeBuffer = new char[length];
	memcpy(writeBuffer, buffer.data(), sizeof(char) * length);
	writefileStream.write(writeBuffer, buffer.size());
	delete[] writeBuffer;
	writefileStream.close();
	return 0;
}

size_t FileStream::WriteBytes(const char* data, const size_t byteCount)
{
	return fwrite(data, sizeof(char), byteCount, filePtr);
}

size_t FileStream::ReadBytes(char* data, const size_t byteCount)
{
	return fread(data, sizeof(char), byteCount, filePtr);
}

bool FileStream::OpenForRead(const char* path)
{
	fopen_s(&filePtr, path, "rb");
	if (filePtr == nullptr)
	{
		ERROR_AND_DIE(Stringf("Failed to open file: %s", path));
	}

	return true;
}

bool FileStream::OpenForWrite(const char* path)
{
	fopen_s(&filePtr, path, "wb");
	if (filePtr == nullptr)
	{
		ERROR_AND_DIE(Stringf("Failed to open file: %s", path));
	}

	return true;
}

void FileStream::Close()
{
	fclose(filePtr);
}
