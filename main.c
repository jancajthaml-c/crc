int crc32(const void* data, const unsigned int polynomial, const int init, const int xor_out);

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

double diff(double start, double end) {
  if ((end - start) < 0) {
	return 1000000000 + end - start;
  } else {
	return end - start;
  }
  return 0;
}

static char *rand_string(char *str, size_t size) {
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

char* rand_string_alloc(size_t size) {
    char *s = malloc(size + 1);
    if (s) {
    	rand_string(s, size);
    }
    return s;
}

void expectCrc32(const char* label, const void* data, const unsigned int polynomial, const int init, const int xor_out, const int checksum) {
	const int d = crc32(data, polynomial, init, xor_out);

	if (d != checksum) {
		printf("%s : `%s` expected checksum 0x%2.2x but got 0x%2.2x instead\n\n", label, data, checksum, d);
		exit(EXIT_FAILURE);
	}
}

int main() {
	//printf("\nRunning unit tests\n");
	
	//expectCrc32("test-hand", "a", 0xEDB88320, 0x11);

	// expectations
	// zero data
	expectCrc32("CRC-32",         "", 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
	//expectCrc32("CRC-32/AUTOSAR", "", 0xF4ACFB13, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
	//expectCrc32("CRC-32/BZIP2",   "", 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
	//expectCrc32("CRC-32C",        "", 0x1EDC6F41, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
	//expectCrc32("CRC-32D",        "", 0xA833982B, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
	//expectCrc32("CRC-32/MPEG-2",  "", 0x04C11DB7, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF);
	//expectCrc32("CRC-32/POSIX",   "", 0x04C11DB7, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF);
	//expectCrc32("CRC-32Q",        "", 0x814141AB, 0x00000000, 0x00000000, 0x00000000);
	expectCrc32("CRC-32/JAMCRC",  "", 0x04C11DB7, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF);
	//expectCrc32("CRC-32/XFER",    "", 0x000000AF, 0x00000000, 0x00000000, 0x00000000);

	// single char
	expectCrc32("CRC-32",         "a", 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, 0xE8B7BE43);
	//expectCrc32("CRC-32/AUTOSAR", "a", 0xF4ACFB13, 0xFFFFFFFF, 0xFFFFFFFF, 0xeff3908a); // unchecked
	//expectCrc32("CRC-32/BZIP2",   "a", 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, 0x19939B6B);
	//expectCrc32("CRC-32C",        "a", 0x1EDC6F41, 0xFFFFFFFF, 0xFFFFFFFF, 0xC1D04330);
	//expectCrc32("CRC-32D",        "a", 0xA833982B, 0xFFFFFFFF, 0xFFFFFFFF, 0x60819B69);
	//expectCrc32("CRC-32/MPEG-2",  "a", 0x04C11DB7, 0xFFFFFFFF, 0x00000000, 0xE66C6494);
	//expectCrc32("CRC-32/POSIX",   "a", 0x04C11DB7, 0x00000000, 0xFFFFFFFF, 0x579B24DF);
	//expectCrc32("CRC-32Q",        "a", 0x814141AB, 0x00000000, 0x00000000, 0xD1112B6B);
	expectCrc32("CRC-32/JAMCRC",  "a", 0x04C11DB7, 0xFFFFFFFF, 0x00000000, 0x174841BC);
	//expectCrc32("CRC-32/XFER",    "a", 0x000000AF, 0x00000000, 0x00000000, 0x00003E8F);

	// medium string
	expectCrc32("CRC-32",         "00123014764700968325", 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, 0x57D311BB);
	expectCrc32("CRC-32/JAMCRC",  "00123014764700968325", 0x04C11DB7, 0xFFFFFFFF, 0x00000000, 0xA82CEE44);

	/*
	expectCrc32("test1", "a", 0x04C11DB7, 0xE8B7BE43);

	expectCrc32("test2", "00123014764700968325", 0xA833982B, 0xD303B3F3);
	expectCrc32("test3", "00123014764700968325", 0x04C11DB7, 0x57d311bb);
	expectCrc32("test4", "00123014764700968325", 0x1EDC6F41, 0x660BC80D);
	expectCrc32("test5", "00123014764700968325", 0xA833982B, 0xD303B3F3);
	expectCrc32("test6", "1234567812345678", 0x11c4dfb5, 0xed52dcf8);
	*/

	//printf("passed");

	// micro benchmark
	int i;
	unsigned long long times = 1000;
	
	size_t big_string_size = 1000000;
	char* big_string = rand_string_alloc(big_string_size);

	size_t small_string_size = 10;
	char* small_string = rand_string_alloc(small_string_size);

	//printf("\nRunning benchmark %lu times \n", (unsigned long) times);

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
		crc32(big_string, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF);
		clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
		total_time_big += diff(t1.tv_nsec, t2.tv_nsec);

		clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
		crc32(small_string, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF);
		clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
		total_time_small += diff(t1.tv_nsec, t2.tv_nsec);
	}

	total_time_big = ((total_time_big / times) / big_string_size);
	total_time_small = (((total_time_small - clock_cost) / times) / small_string_size);

	printf("((N * %f) + %f) ns/op\n", total_time_big, total_time_small);

	return 0;
}