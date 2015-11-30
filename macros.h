/*
 * macros.h
 *
 *  Created on: Nov 23, 2015
 *      Author: mac
 */

#ifndef MACROS_H_
#define MACROS_H_

#include <assert.h>
#include <stdio.h>

#define TEST_EQUALS(result, a, b) if ((result) && ((a) != (b))) { \
								result = false; \
							}

#define TEST_DIFFERENT(result, a, b) if ((result) && ((a) == (b))) { \
								result = false; \
							}

#define TEST_TRUE(result, bool) if ((result) && !(bool)) { \
								result = false; \
							}

#define TEST_FALSE(result, bool) if ((result) && (bool)) { \
								result = false; \
							}

#define RUN_TEST(name)  printf("Running "); \
						printf(#name);		\
						printf("... ");		\
						if (!name()) { \
							printf("[FAILED]\n");		\
							return false; \
						}								\
						printf("[SUCCESS]\n");

#endif /* MACROS_H_ */
