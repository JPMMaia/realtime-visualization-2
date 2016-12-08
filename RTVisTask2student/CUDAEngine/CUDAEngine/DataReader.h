#pragma once

#include <vector>

namespace CUDAEngine
{
	class DataReader
	{
	public:
		static void Read(const std::wstring& filename, std::vector<float>& departureDelays, std::vector<float>& arrivalDelays, size_t numElementsToRead = -1);
	};
}
