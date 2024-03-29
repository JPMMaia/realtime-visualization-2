#pragma once

#include <chrono>

namespace CUDAEngine
{
	class Timer
	{
	private:
		using TimePointType = std::chrono::time_point<std::chrono::high_resolution_clock>;

	public:
		void Start();
		void End();

		template <class Representation, class Period>
		std::chrono::duration<Representation, Period> ElapsedTime() const;

	private:
		TimePointType m_start;
		TimePointType m_end;
	};

	template <typename Representation, typename Period>
	std::chrono::duration<Representation, Period> Timer::ElapsedTime() const
	{
		return m_end - m_start;
	}
}