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

			auto rangeX = maxX - minX;
			auto rangeY = maxY - minY;
			auto epsilonLocal = epsilon * std::max<float>(rangeX / float(numBins - 1), rangeY / float(numBins - 1));

			// Create an image with 0s:
			std::vector<float> imageCpu(numBins*numBins, 0.0f);
			auto maxBinCpu = KDEEstimator2D(departureDelays.data(), arrivalDelays.data(), departureDelays.size(), epsilonLocal, minX, maxX, minY, maxY, imageCpu.data(), numBins);

			std::vector<float> imageCuda(numBins*numBins, 0.0f);
			auto maxBinCuda = CallKDE2D(departureDelays.data(), arrivalDelays.data(), departureDelays.size(), epsilonLocal, minX, maxX, minY, maxY, imageCuda.data(), numBins);

			// Compare results:
			auto tolerance = 0.01f;
			Assert::AreEqual(maxBinCpu, maxBinCuda, tolerance);
			for(size_t i = 0; i < imageCpu.size(); ++i)
			{
				Assert::AreEqual(imageCpu[i], imageCuda[i], tolerance);
			}
		}

	};
}