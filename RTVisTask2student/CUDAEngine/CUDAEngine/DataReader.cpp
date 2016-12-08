#include "DataReader.h"
#include "Utilities.h"

#include <fstream>
#include "EngineException.h"

using namespace CUDAEngine;
using namespace std;

void DataReader::Read(const std::wstring& filename, std::vector<float>& departureDelays, std::vector<float>& arrivalDelays, size_t numElementsToRead)
{
	// Open file for reading in binary mode, and seek to the end of file immediately:
	ifstream file(filename, ios::in | ios::binary | ios::ate);
	if (!file.good())
		ThrowEngineException(L"Couldn't open file " + filename);

	// Get size of file and seek to the begin of file:
	auto size = file.tellg();
	file.seekg(0, ios::beg);

	// Read content of file:
	{
		// Allocate memory for the output vectors::
		auto oneArraySize = (size / sizeof(int)) / 2;
		departureDelays.reserve(oneArraySize);
		arrivalDelays.reserve(oneArraySize);

		using DataType = int;
		size_t count = 0;
		while (!file.eof() && count < numElementsToRead)
		{
			DataType temp;

			// Read departure delay value:
			file.read(reinterpret_cast<char*>(&temp), sizeof(DataType));
			if (file.eof())
				break;
			departureDelays.push_back(static_cast<float>(temp));

			// Read arrival delay value:
			file.read(reinterpret_cast<char*>(&temp), sizeof(DataType));
			if (file.eof())
				break;
			arrivalDelays.push_back(static_cast<float>(temp));

			count += 1;
		}
	}
}
