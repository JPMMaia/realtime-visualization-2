#pragma once

#include <future>
#include <vector>
#include <fstream>

namespace CUDAEngine
{
	namespace Utilities
	{
		bool FileExists(const std::wstring& filename);
		std::wstring GetFilename(const std::wstring& filename);
		std::wstring GetFileExtension(const std::wstring& filename);
		std::wstring GetFilePath(const std::wstring& filename);

		std::wstring StringToWString(const std::string& value);
		std::string WStringToString(const std::wstring& value);

		std::wstring GetWorkingDirectory();

		template<typename DataType>
		void ReadData(const std::wstring& filename, std::vector<DataType>& buffer)
		{
			using namespace std;

			// Open file for reading in binary mode, and seek to the end of file immediately:
			ifstream file(filename, ios::in | ios::binary | ios::ate);
			if (!file.good())
				throw runtime_error("Couldn't open file " + Utilities::WStringToString(filename));

			// Get size of file and seek to the begin of file:
			auto size = file.tellg();
			file.seekg(0, ios::beg);

			// Read content of file:
			buffer.resize(static_cast<uint32_t>(size) / sizeof(DataType));
			file.read(reinterpret_cast<char*>(buffer.data()), size);
			if (!file.good())
				throw runtime_error("Error while reading file " + Utilities::WStringToString(filename));
		}
		void WriteData(const std::wstring& filename, const std::vector<char>& buffer);

		template<typename FunctionType, typename... ArgumentsType>
		std::future<typename std::result_of<FunctionType(ArgumentsType...)>::type> RunAsync(FunctionType&& function, ArgumentsType&&... arguments)
		{
			return std::async(std::launch::async, std::forward<FunctionType>(function), std::forward<ArgumentsType>(arguments)...);
		}
	}
}
