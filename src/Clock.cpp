
#include <Clock.hpp>

Clock::Clock() {
}
void Clock::start() {
	clock_gettime(CLOCK_MONOTONIC, &_start);
}
uint64 Clock::elapsed() {
	struct timespec stop, temp;
	clock_gettime(CLOCK_MONOTONIC, &stop);
	if ((stop.tv_nsec - _start.tv_nsec) < 0) {
		temp.tv_sec = stop.tv_sec - _start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + stop.tv_nsec - _start.tv_nsec;
	} else {
		temp.tv_sec = stop.tv_sec - _start.tv_sec;
		temp.tv_nsec = stop.tv_nsec - _start.tv_nsec;
	}
	return (uint64) temp.tv_sec * 1000 + (uint64) temp.tv_nsec / 1000000;
}
