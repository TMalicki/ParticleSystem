/*
#include "Timer.h"

auto Timer::updateTime(std::chrono::high_resolution_clock::time_point& start)
{
	auto stop = std::chrono::high_resolution_clock::now();
	//auto timePassed = std::chrono::duration<float, std::milli>(stop - start);
	auto timePassed = (stop - start);
	start = stop;

	return  timePassed;
}
/*
const auto& Timer::timePlot(float dt)
{

	return time;
}
*/
