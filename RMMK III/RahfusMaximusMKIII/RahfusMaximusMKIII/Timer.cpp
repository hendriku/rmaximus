#include "Timer.h"

Timer::Timer() {
	request = high_resolution_clock::now();
}

long long Timer::GetDeltaNanoTime() {
	return duration_cast<nanoseconds>(high_resolution_clock::now() - request).count();
}
long long Timer::GetDeltaMilliTime() {
	return duration_cast<milliseconds>(high_resolution_clock::now() - request).count();
}
