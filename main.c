#include "crc.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

double diff(double start, double end) {
  double temp;
  if ((end - start) < 0) {
  	return 1000000000 + end - start;
  } else {
		return end - start;
  }
  return temp;
}

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* rand_string_alloc(size_t size)
{
     char *s = malloc(size + 1);
     if (s) {
        rand_string(s, size);
     }
     return s;
}

int _crc32(const char* data, const unsigned int polynomial) {
	return crc32(data, polynomial, strlen(data));
}

int main() {
	printf("\nRunning unit tests\n");

	// expectations

	if (_crc32("", 0xedb88320) != 0x0) {
		return 1;
	}
	if (_crc32("00123014764700968325", 0xedb88320) != 0x8ee65698) {
		return 1;
	}

	if (_crc32("1234567812345678", 0x11c4dfb5) != 0xed52dcf8) {
		return 1;
	}

	printf("passed");

	// micro benchmark

	int i;
	unsigned long long times = 1000;
	
	size_t big_string_size = 1000000;
	char* big_string = rand_string_alloc(big_string_size);

	size_t small_string_size = 10;
	char* small_string = rand_string_alloc(small_string_size);


	printf("\n\nRunning benchmark %lu times \n", (unsigned long) times);

	double total_time_big = 0;
	double total_time_small = 0;
	double clock_cost = 0;

	struct timespec t1;
	struct timespec t2;

	for (i = 0; i < times; ++i) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
		clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
		clock_cost += diff(t1.tv_nsec, t2.tv_nsec);

		clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
		_crc32(big_string, 0xedb88320);
		clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
		total_time_big += diff(t1.tv_nsec, t2.tv_nsec);

		clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
		_crc32(small_string, 0xedb88320);
		clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
		total_time_small += diff(t1.tv_nsec, t2.tv_nsec);
	}

	total_time_big = ((total_time_big / times) / big_string_size);
	total_time_small = (((total_time_small - clock_cost) / times) / small_string_size);

	printf("cost: ((N * %f) + %f) ns/op\n\n", total_time_big, total_time_small);

	return 0;
}