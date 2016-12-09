#pragma once

extern float CallKDE2D(const float* xData, const float* yData, size_t dataCount, float epsilon, float minX, float maxX, float minY, float maxY, float* kdeImage, size_t numBins);