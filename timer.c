#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <time.h>
#include <math.h>

static double now()
{
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);

	return time.tv_sec + time.tv_nsec / pow(10,9);
}

static void sleep_interruptible(double seconds)
{
	struct timespec time;
	time.tv_sec = (time_t) floor(seconds);
	time.tv_nsec = (long) round((seconds - time.tv_sec) * pow(10, 9));

	clock_nanosleep(CLOCK_MONOTONIC, 0, &time, NULL);
}

double sleep(double seconds)
{
	double end = now() + seconds;

	double wall;
	while ((wall = now()) < end)
	{
		sleep_interruptible(end - wall);
	}

	/* Return actual time slept */
	return wall - end + seconds;
}