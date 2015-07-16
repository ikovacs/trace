#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <stdint.h>
#include <time.h>

typedef uint64_t uint64;

class Clock {
public:
	Clock();
	void start();
	uint64 elapsed();
protected:
	struct timespec _start;
};

#endif /* __CLOCK_H__ */
