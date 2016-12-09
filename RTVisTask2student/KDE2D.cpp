#include <math.h>
#include <algorithm>

float isotropicGaussKernel2D(float x, float y, float center_x, float center_y, float eps)
{
	register float tmpx = x - center_x;
	register float tmpy = y - center_y;
	register float eps_p_2 = eps*eps;

	return ::expf(-0.5f * tmpx * tmpx / eps_p_2) * ::expf(-0.5f * tmpy * tmpy / eps_p_2); // (2.0f*eps_p_2*M_PI);
}

extern "C" 
float KDEEstimator2D(const float* xData, const float* yData, size_t dataCount, float epsilon, float minX, float maxX, float minY, float maxY, float* kde_image, size_t numBins)
{
	float rangeX = maxX - minX;
	float rangeY = maxY - minY;

	for (int i = 0; i < numBins; ++i)
	{
		float y = float(i) / (numBins - 1)*rangeY + minY;
		for (int j = 0; j < numBins; ++j)
		{
			float x = float(j) / (numBins - 1)*rangeX + minX;

			float sum = 0.0f;
			for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
			{
				sum += isotropicGaussKernel2D(x, y, xData[dataIndex], yData[dataIndex], epsilon);
			}
			kde_image[i*numBins + j] = sum;

			//qDebug("k: %d, l: %d", k, l);
		}
	}

	return *std::max_element(kde_image, kde_image+(numBins*numBins));
}