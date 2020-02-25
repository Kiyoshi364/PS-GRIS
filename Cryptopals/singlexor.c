#include <stdio.h>
#include <stdlib.h>

#define _APHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz"
#define _NUMBERS "0123456789"
#define _SYMBOLS "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"

#define _UNPRINTABLE "\x01\x02\x03\x04\x05\x06\x07\x08\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x7F"

#define _GOODCHAR _ALPHABET _NUMBERS _SYMBOLS
#define _BADCHAR _UNPRINTABLE

char hextoascii(char fst, char sec);

void bruteforce(char *input, size_t len);
int smartbf(char *input, size_t len);

int main(const int argc, const char **argv) {
	if (argc < 2) {
		char flag = 1;
		char input[100], buffer[200], last;
		int i = 0, b = 0, cnt = 1;
		while (flag) {
			char c = 0;
			if (flag - 4) c = getchar();
			switch (flag) {
				case 1:
					if (c == '\n') {
						flag = 0;//last=='\n'? 0 : 4;
					} else {
						flag = 2;
						last = c;
						buffer[b++] = c;
					}
					break;
				case 2:
					if (c == '\n') {
						flag = 4;
					} else {
						flag = 3;
						input[i++] = hextoascii(last, c);
						buffer[b++] = c;
					}
					break;
				case 3:
					if (c == '\n') {
						flag = 4;
					} else {
						flag = 2;
						buffer[b++] = c;
					}
					break;
				case 4:
					//bruteforce(input, i);
					if (smartbf(input, i)) {
						printf("%d>%s\n", cnt, buffer); // THRESH
						putchar(10);
					}
					//printf("i=%d, b=%d, cnt=%d\n", i, b, cnt); // THRESH
					i = 0, b = 0, cnt++;
					flag = 1;
					break;
				default:
					flag = 0;
					break;
			}
			last = c;
		}
		return 0;
	}

	//char input[] = "\x1b\x37\x37\x33\x31\x36\x3f\x78\x15\x1b\x7f\x2b\x78\x34\x31\x33\x3d\x78\x39\x78\x28\x37\x2d\x36\x3c\x78\x37\x3e\x78\x3a\x39\x3b\x37\x36";
	char input[100];
	register int i = 0;
	for (; i < 100 && argv[1][2*i]; i++) {
		input[i] = hextoascii(argv[1][2*i], argv[1][2*i+1]);
	}

	size_t len = i;

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
		//while (getchar() != '\n');
		putchar(10);
	}
}

char badchar(char c) {
	return ( (c&0x80) || (c < 0x20 && c != '\n') || !(c ^ 0x7F) );
	/* DEPRECATED 2
	return ( (c&0x80) || !(c ^ 0xE0) || !(c ^ 0x7F) );
	DEPRECATED 1
	register char i = 0;
	while (_BADCHAR[i] != '\0') {
		if ((_BADCHAR[i++] == c)) return 1;
	}
	return 0;
	*/
}

int smartbf(char *input, size_t len) {
	char a = 0;
	for (unsigned char key = 1; key; key++) {
		char buffer[len];
		char flag = 1;
		for (register size_t i = 0; i < len; i++) {
			buffer[i] = (*(input + i)) ^ key;
			if (badchar(buffer[i])) {
				buffer[i] = '\0';
				flag = 0;
				break;
			}
		}
		if (flag) {
			printf("0x%02X>%s\n", key, buffer);
			a++;
		}
	}
	return a;
}

char hextoascii(char fst, char sec) {
	if (fst >= '0' && fst <= '9') fst -= '0';
	else {
		if (fst >= 'a' && fst <= 'f') fst += 'A' - 'a';
		if (fst >= 'A' && fst <= 'F') fst += 0x0A - 'A';
		else fst = 0;
	}

	if (sec >= '0' && sec <= '9') sec -= '0';
	else {
		if (sec >= 'a' && sec <= 'f') sec += 'A' - 'a';
		if (sec >= 'A' && sec <= 'F') sec += 0x0A - 'A';
		else sec = 0;
	}

	fst <<= 4;
	if (fst&0x0F) printf("Err: hextoascii fst=%X\n", fst);
	if (sec&0xF0) printf("Err: hextoascii sec=%X\n", sec);

	return fst | sec;
}
