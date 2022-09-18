#pragma once
#include <vector>
#include <string>

bool DoesFileExist(const std::string& fileName);
int FileReadToBuffer(std::vector<uint8_t>& outBuffer, const std::string& filename);
int FileReadToString(std::string& outString, const std::string& filename);
int BufferWriteToFile(const std::vector<uint8_t>& buffer, const std::string& filename, bool createFileIfItDoesNotExist = true);

class FileStream
{
public:
	size_t WriteBytes(const char* data, const size_t byteCount);
	size_t ReadBytes(char* data, const size_t byteCount);

	bool OpenForRead(const char* path);
	bool OpenForWrite(const char* path);
	void Close();

private:
	FILE* filePtr = nullptr;
};
