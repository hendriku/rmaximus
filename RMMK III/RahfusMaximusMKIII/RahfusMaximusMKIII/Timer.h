#pragma once
#include <chrono>

using namespace std;
using namespace std::chrono;

class Timer {
	high_resolution_clock::time_point request;
public:
	Timer();
	long long GetDeltaNanoTime();
	long long GetDeltaMilliTime();
};
