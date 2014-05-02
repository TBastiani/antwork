#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <stdlib.h>
#include <stdio.h>

#define REQUIRE_CONDITION(condition, msg)\
{\
	if ((condition) == 0)\
	{\
		printf("%s\n", msg);\
		exit(-1);\
	}\
}

#define DEBUG_MESSAGE(...)\
{\
	printf(__VA_ARGS__);\
	printf("\n");\
}

#endif /* !_EXCEPTION_H_ */
