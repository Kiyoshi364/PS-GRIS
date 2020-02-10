#include <stdio.h>
#include <stdlib.h>

#define _APHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz"
#define _NUMBERS "0123456789"
#define _SYMBOLS "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"

#define _UNPRINTABLE "\x01\x02\x03\x04\x05\x06\x07\x08\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x7F"

#define _GOODCHAR _ALPHABET _NUMBERS _SYMBOLS
#define _BADCHAR _UNPRINTABLE

void bruteforce(char *input, size_t len);
void smartbf(char *input, size_t len);

int main() {
	char input[] = "\x1b\x37\x37\x33\x31\x36\x3f\x78\x15\x1b\x7f\x2b\x78\x34\x31\x33\x3d\x78\x39\x78\x28\x37\x2d\x36\x3c\x78\x37\x3e\x78\x3a\x39\x3b\x37\x36";

	size_t len = 34;

	char input2[] = hextoascii();

	//bruteforce(input, len);
	smartbf(input, len);

	return 0;
}

void bruteforce(char *input, size_t len) {
	for (unsigned char i = 1; i; i++) {
		register size_t j = 0;
		printf("0x%02X: ", i);
		while (*(input + j)) {
			putchar((*(input + j))^i);
			j++;
		}
		while (getchar() != '\n');
	}
}

char badchar(char c) {
	return (c&0x80 || !(c&0xE0) || !(c ^ 0x7F) );
	/* DEPRECATED
	register char i = 0;
	while (_BADCHAR[i] != '\0') {
		if ((_BADCHAR[i++] == c)) return 1;
	}
	return 0;
	*/
}

void smartbf(char *input, size_t len) {
	char buffer[len];
	for (unsigned char key = 1; key; key++) {
		char flag = 1;
		for (register size_t i = 0; *(input + i); i++) {
			buffer[i] = (*(input + i)) ^ key;
			if (badchar(buffer[i])) {
				flag = 0;
				break;
			}
		}
		if (flag)
			printf("0x%02X: %s\n", key, buffer);
	}
}
