#include <stdio.h>
#include <cuda_runtime.h>
#include <cmath>
#include <algorithm>

constexpr auto c_blockSize = 256;

__device__
float CudaIsotropicGaussKernel2D(float x, float y, float center_x, float center_y, float eps)
{
	register float tmpx = x - center_x;
	register float tmpy = y - center_y;
	register float eps_p_2 = eps*eps;

	return ::expf(-0.5f * tmpx * tmpx / eps_p_2) * ::expf(-0.5f * tmpy * tmpy / eps_p_2);
}

__global__
void CudaKDE2D(const float* xData, const float* yData, size_t dataCount, float epsilon, float minX, float maxX, float minY, float maxY, float* kdeImage, size_t numBins)
{
	int imageSize = static_cast<int>(numBins * numBins);
	int id = blockDim.x * blockIdx.x + threadIdx.x;
	if (id >= imageSize)
		return;

	int i = id / numBins;
	int j = id % numBins;

	float rangeX = maxX - minX;
	float rangeY = maxY - minY;
	float y = float(i) / (numBins - 1)*rangeY + minY;
	float x = float(j) / (numBins - 1)*rangeX + minX;

	float sum = 0.0f;

	// Version 1:
	/*{
		for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
		{
			sum += CudaIsotropicGaussKernel2D(x, y, xData[dataIndex], yData[dataIndex], epsilon);
		}
	}*/

	// Version 2:
	{
		__shared__ float xDataShared[c_blockSize];
		__shared__ float yDataShared[c_blockSize];

		for (int blockStart = 0; blockStart < dataCount; blockStart += c_blockSize)
		{
			for (int index = 0; index < c_blockSize && (blockStart + index) < dataCount; ++index)
			{
				xDataShared[index] = xData[blockStart + index];
				yDataShared[index] = yData[blockStart + index];
			}

			__syncthreads();

			for (int index = 0; index < c_blockSize && (blockStart + index) < dataCount; ++index)
			{
				sum += CudaIsotropicGaussKernel2D(x, y, xDataShared[index], yDataShared[index], epsilon);
			}

			__syncthreads();
		}
	}

	kdeImage[id] = sum;
}

float CallKDE2D(const float* xData, const float* yData, size_t dataCount, float epsilon, float minX, float maxX, float minY, float maxY, float* kdeImage, size_t numBins)
{
	size_t dataByteSize = dataCount * sizeof(float);
	size_t imageByteSize = numBins * numBins * sizeof(float);

	// Initialize GPU memory for the x-data:
	float* cudaXData;
	cudaMalloc(&cudaXData, dataByteSize);
	cudaMemcpy(cudaXData, xData, dataByteSize, cudaMemcpyHostToDevice);

	// Initialize GPU memory for the y-data:
	float* cudaYData;
	cudaMalloc(&cudaYData, dataByteSize);
	cudaMemcpy(cudaYData, yData, dataByteSize, cudaMemcpyHostToDevice);

	// Initialize GPU memory for KDE image with values 0:
	float* cudaKDEImage;
	cudaMalloc(&cudaKDEImage, imageByteSize);
	cudaMemset(cudaKDEImage, 0, imageByteSize);

	// Call kde 2d:
	int blockDimension = c_blockSize;
	int gridDimension = static_cast<int>((numBins*numBins + blockDimension - 1) / blockDimension);
	CudaKDE2D << <gridDimension, blockDimension >> > (cudaXData, cudaYData, dataCount, epsilon, minX, maxX, minY, maxY, cudaKDEImage, numBins);

	// Copy memory of the KDE image from GPU to CPU:
	cudaMemcpy(kdeImage, cudaKDEImage, imageByteSize, cudaMemcpyDeviceToHost);

	// Free memory on GPU:
	cudaFree(cudaKDEImage);
	cudaFree(cudaYData);
	cudaFree(cudaXData);

	return *std::max_element(kdeImage, kdeImage + (numBins*numBins));
}
