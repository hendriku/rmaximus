#include "TimeUtils.h"
#include <time.h>

static clock_t requestTime;

long TimeUtils::getDeltaTime() {
	return clock() - requestTime;
}

void TimeUtils::serverRequest() {
	requestTime = clock();
}

TimeUtils::TimeUtils() {}

TimeUtils::~TimeUtils() {}
