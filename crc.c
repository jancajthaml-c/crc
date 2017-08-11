#include <stdio.h>

int main() {
	// FIXME read from input file
	// ... reading from stdin is slow

	// FIXME separate main and .o
	// FIXME add "-c" for non-runable .o

	int crc = ~0;

	// FIXME from cli args
	unsigned int polynomial = 0xEDB88320;

	unsigned int p7 = polynomial >> 1;
	unsigned int p6 = polynomial >> 2;
	unsigned int p5 = polynomial >> 3;
	unsigned int p4 = polynomial >> 4;
	unsigned int p3 = polynomial >> 5;
	unsigned int p2 = (polynomial >> 6) ^ polynomial;
	unsigned int p1 = (polynomial >> 7) ^ p7;
	unsigned char c;

	FILE *fd = freopen(0, "rb", stdin);

loop:
{
	fread(&c, sizeof(unsigned char), 1, fd);
	if (feof(fd)) {
		fclose(fd);
		crc = ~crc;
		goto flush;
	}

	crc ^= c;
	  	crc = (
	  	(((crc << 31) >> 31) & p1) ^
	    (((crc << 30) >> 31) & p2) ^
	    (((crc << 29) >> 31) & p3) ^
	    (((crc << 28) >> 31) & p4) ^
	    (((crc << 27) >> 31) & p5) ^
	    (((crc << 26) >> 31) & p6) ^
	    (((crc << 25) >> 31) & p7) ^
	    (((crc << 24) >> 31) & polynomial)
	) ^ ((unsigned int)crc >> 8);

	goto loop;
}

flush:
{
	FILE *fo = freopen(0, "wb", stdout);	
	fwrite(&crc, sizeof(unsigned int), 1, fo);
	fclose(fo);
}
	return 0;
}
