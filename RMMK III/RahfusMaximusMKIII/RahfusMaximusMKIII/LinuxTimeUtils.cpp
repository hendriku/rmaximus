/*
 * LinuxTimeUtils.cpp
 *
 *  Created on: 01.03.2016
 *      Author: hendrik
 */
#include "LinuxTimeUtils.h"

static high_resolution_clock::time_point request;

void LinuxTimeUtils::ServerRequest() {
	request = high_resolution_clock::now();
}

long LinuxTimeUtils::GetDeltaTime() {
	return (long) duration_cast<milliseconds>(high_resolution_clock::now()-request).count();
}



