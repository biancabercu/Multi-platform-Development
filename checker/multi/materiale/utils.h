#ifndef LIN_UTILS_H_
#define LIN_UTILS_H_	1

#include<stdio.h>
#include<stdlib.h>

#define DIE(assertion, call_descript)
	do {
		if (assertion) {
			fprintf(stderr, "(%s, %d): ",
					__FILE__, __LINE__);
			perror(call_descript);
			exit(EXIT_FAILURE);
		}
	} while (0)

#endif
