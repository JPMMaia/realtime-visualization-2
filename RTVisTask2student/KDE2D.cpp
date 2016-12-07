#include <math.h>
#include <algorithm>

float isotropicGaussKernel2D(float x, float y, float center_x, float center_y, float eps)
{
	register float tmpx = x - center_x;
	register float tmpy = y - center_y;
	register float eps_p_2 = eps*eps;

	return ::expf(-0.5f*tmpx*tmpx / eps_p_2)*::expf(-0.5f*tmpy*tmpy / eps_p_2); // (2.0f*eps_p_2*M_PI);
}


extern "C" 
float KDEEstimator2D(const float* x_arr, const float* y_arr, size_t data_num, float epsilon, float minX,float maxX,float minY,float maxY, float* kde_image, size_t numBins)
{
	float rangeX = maxX - minX;
	float rangeY = maxY - minY;

	float maxBin = 0.0f;
	for (int l = 0; l < numBins; ++l)
	{
		float y = float(l) / (numBins - 1)*rangeY + minY;
		for (int k = 0; k < numBins; ++k)
		{
			float x = float(k) / (numBins - 1)*rangeX + minX;

			for (int i = 0; i < data_num; i++)
			{
				kde_image[l*numBins + k] += isotropicGaussKernel2D(x, y, x_arr[i], y_arr[i], epsilon);
			}
			//qDebug("k: %d,l: %d", k, l);
		}
	}

	maxBin = *std::max_element(kde_image, kde_image+(numBins*numBins));

	return maxBin;
}