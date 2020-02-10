#include <stdio.h>
#include <stdlib.h>

size_t xordist(const char const *input1, const char const *input2, const size_t len);

#ifndef __NOMAIN__
int main(const int argc, const char **argv) {
	if (argc < 3) {
		printf("Not enough arguments\n");
		return 1;
	}

	//char *input1 = argv[1];
	//char *input2 = argv[2];

	char input1[] = "this is a test";
	char input2[] = "wokka wokka!!!";

	size_t len = 0;
	while (*(input1 + len) && *(input2 + len)) len++;

	printf("len = %ld\n", len);

	printf("main:\n1:%s\n2:%s\ncount: %lu\n", input1, input2, xordist(input1, input2, len));

	return 0;
}
#endif //__NOMAIN__

unsigned char addbits(char byte) {
	return byte? 
		( (byte&1) + addbits((byte>>1)&0x7F) )
		: 0;
}

size_t xordist(const char const *input1, const char const *input2, const size_t len) {
	size_t count = 0;

	for (register int i = 0; i < len; i++) {
		char a = *(input1 + i), b = *(input2 + i);
		count += addbits(a^b);

		// Debugging
		//printf("xdist: a=%X b=%X xor=%X cnt=%hhu\n", a, b, xor, cnt);
	}

	return count;
}
