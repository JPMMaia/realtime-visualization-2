#include "Timer.h"

using namespace CUDAEngine;
using namespace std;
using namespace std::chrono;

void Timer::Start()
{
	m_start = high_resolution_clock::now();
}

void Timer::End()
{
	m_end = high_resolution_clock::now();
}