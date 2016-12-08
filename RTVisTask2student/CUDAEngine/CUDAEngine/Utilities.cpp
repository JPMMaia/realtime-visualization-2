#include "Utilities.h"
#include "EngineException.h"

#include <codecvt>
#include <locale>
#include <fstream>
#include <direct.h>

using namespace std;
using namespace CUDAEngine;

bool Utilities::FileExists(const std::wstring& filename)
{
	std::fstream fileStream(filename);

	return fileStream.good();
}

std::wstring Utilities::GetFilename(const std::wstring& filename)
{
	std::wstring searchString(L"/");

	auto begin = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
	if (begin == filename.end())
		begin = filename.begin();
	else
		++begin;

	std::wstring dot(L".");
	auto end = std::find_end(begin, filename.end(), dot.begin(), dot.end());

	return std::wstring(begin, end);
}
std::wstring Utilities::GetFileExtension(const std::wstring& filename)
{
	std::wstring dot(L".");
	auto end = std::find_end(filename.begin(), filename.end(), dot.begin(), dot.end());
	if (end == filename.end())
		ThrowEngineException(L"Filename has no extension.");

	return std::wstring(end + 1, filename.end());
}
std::wstring Utilities::GetFilePath(const std::wstring& filename)
{
	std::wstring searchString(L"/");

	auto location = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
	if (location == filename.end())
		return L"";

	return std::wstring(filename.begin(), location + 1);
}

wstring Utilities::StringToWString(const string& value)
{
	using convertType = codecvt_utf8<wchar_t>;
	wstring_convert<convertType, wchar_t> converter;

	return converter.from_bytes(value);
}
string Utilities::WStringToString(const wstring& value)
{
	using convertType = codecvt_utf8<wchar_t>;
	wstring_convert<convertType, wchar_t> converter;

	return converter.to_bytes(value);
}

std::wstring Utilities::GetWorkingDirectory()
{
	constexpr auto maxLength = 2048;
	wchar_t result[maxLength];
	_wgetcwd(result, maxLength);
	return std::wstring(result);
}

void Utilities::WriteData(const std::wstring& filename, const std::vector<char>& buffer)
{
	using namespace std;

	// Open file for writing in binary mode:
	ofstream file(filename, ios::out | ios::binary);
	if (!file.good())
		throw runtime_error("Couldn't open file " + Utilities::WStringToString(filename));

	// Write contents to file:
	file.write(buffer.data(), buffer.size());
	if (!file.good())
		throw runtime_error("Error while writing file " + Utilities::WStringToString(filename));
}
