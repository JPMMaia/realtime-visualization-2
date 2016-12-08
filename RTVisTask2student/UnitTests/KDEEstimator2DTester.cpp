#include "stdafx.h"
#include "CppUnitTest.h"
#include "DataReader.h"
#include "Utilities.h"
#include "KDE2D.cuh"

#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace CUDAEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

extern "C"
float KDEEstimator2D(const float* x_arr, const float* y_arr, size_t data_num, float epsilon, float minX, float maxX, float minY, float maxY, float* kde_image, size_t numBins);

namespace UnitTests
{		
	TEST_CLASS(KDEEstimator2DTester)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// Set parameters:
			auto numBins = 128;
			auto minX = -100.0f;
			auto maxX = 500.0f;
			auto minY = -100.0f;
			auto maxY = 600.0f;
			auto epsilon = 1.0f;

			// Read data:
			vector<float> departureDelays;
			vector<float> arrivalDelays;
			DataReader::Read(L"../../Data/smalldata.data", departureDelays, arrivalDelays, 500);

			// Create an image with 0s:
			std::vector<float> image(numBins*numBins, 0.0f);

			auto minDep = float(minX);
			auto maxDep = float(maxX);
			auto rangeDep = maxDep - minDep;
			auto minArr = float(minY);
			auto maxArr = float(maxY);
			auto rangeArr = maxArr - minArr;
			auto epsilonLocal = epsilon * std::max<float>(rangeDep / float(numBins - 1), rangeArr / float(numBins - 1));

			auto maxBin = KDEEstimator2D(departureDelays.data(), arrivalDelays.data(), departureDelays.size(), epsilonLocal, minDep, maxDep, minArr, maxArr, image.data(), numBins);

			CallKDE2D();
			
		}

	};
}