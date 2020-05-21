#pragma once
#include <iostream>
#include <chrono>

static std::vector<float> s_time (1'000);
static int counter { 0 };

auto getTime(std::chrono::high_resolution_clock::time_point& start) 
{
	auto stop = std::chrono::high_resolution_clock::now();
	auto timePassed = std::chrono::duration<float, std::milli> (stop - start);
	start = stop;

	if (counter == s_time.size()) counter = 0;
	s_time.at(counter) = timePassed.count();
	counter++;

	return  timePassed.count();
}