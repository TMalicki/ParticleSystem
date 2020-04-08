#pragma once
#include <iostream>
#include <chrono>

auto getTime(std::chrono::high_resolution_clock::time_point& start) 
{
	auto stop = std::chrono::high_resolution_clock::now();
	auto timePassed = std::chrono::duration<double, std::milli> (stop - start);
	start = stop;

	return  timePassed.count();
}