#include <stdio.h>
#include <cuda_runtime.h>
#include <cmath>
#include <algorithm>

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
	int id = blockDim.x * blockIdx.x + threadIdx.x;
	if (id >= static_cast<int>(numBins * numBins))
		return;

	int i = id / numBins;
	int j = id % numBins;

	float rangeX = maxX - minX;
	float rangeY = maxY - minY;
	float y = float(i) / (numBins - 1)*rangeY + minY;
	float x = float(j) / (numBins - 1)*rangeX + minX;

	float sum = 0.0f;
	for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
	{
		sum += CudaIsotropicGaussKernel2D(x, y, xData[dataIndex], yData[dataIndex], epsilon);
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
	int threadsPerBlock = 256;
	int blocksPerGrid = (numBins*numBins + threadsPerBlock - 1) / threadsPerBlock;
	CudaKDE2D << <blocksPerGrid, threadsPerBlock >> > (cudaXData, cudaYData, dataCount, epsilon, minX, maxX, minY, maxY, cudaKDEImage, numBins);

	// Copy memory of the KDE image from GPU to CPU:
	cudaMemcpy(kdeImage, cudaKDEImage, imageByteSize, cudaMemcpyDeviceToHost);

	// Free memory on GPU:
	cudaFree(cudaKDEImage);
	cudaFree(cudaYData);
	cudaFree(cudaXData);

	return *std::max_element(kdeImage, kdeImage + (numBins*numBins));
}
