#include <stdio.h>
#include <stdlib.h>

#define _DICTIONARY_ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="

int base64toascii(const char *input, char *output);
int asciitobase64(const char *input, char *output);
char ignore(char c);

// Options
#define __FILEMODE__	if(ops&0x01)
#define __DECODE__		if(ops&0x02)
#define __VERBOSE__		if(ops&0x04)
#define __BUGS__		if(ops&0x08)
#define __IGNORE__		if(ops&0x10)

// Error codes
#define ERR_OPEN_FILE			1
#define ERR_NO_MEMORY_INPUT		2
#define ERR_NO_MEMORY_OUTPUT	3

unsigned char ops = 0;
char *ignorelist;
unsigned char ignorelen;
/* Options
 * -f		reads from a file
 * -e		encodes (default)	From ascii to base64
 * -d		decodes				From base64 to ascii
 * -v		verbose
 * -b		bugs				Helps finding bugs from (en/de)crypts loops
 * -i		ignore				Ignores characters from next argument
 */

char parseMacros(const char *input, unsigned char *i);
unsigned char quicksort(char *list, unsigned char len);
void help(const char *name);

int main(const int argc, const char **argv) {

	int i = 1;

	if (argc < 2) {
		help(argv[0]);
		return 0;
	} else { // Parse arguments
		unsigned char flag = 0;
		for (; i < argc; i++) {
			if (flag&0x01) { // Parsing ignorelist
				while (argv[i][ignorelen++])
					if (ignorelen == 0xFF) break;

				ignorelist = malloc(sizeof(char)*ignorelen);

				unsigned char limit = ignorelen;
				unsigned char index = 0;
				for (register unsigned char j = 0; j < limit; j++) {
					if (argv[i][j] == '\\') {
						unsigned char now = j;
						ignorelist[index++] = parseMacros(argv[i], &now);
						ignorelen -= now - 1 - j;
						j = now - 1;
					} else
						ignorelist[index++] = argv[i][j];
					__BUGS__ printf("%d 0x%02x: 0x%02x %c\n", index-1, index-1, ignorelist[index-1], ignorelist[index-1]);
				}

				__BUGS__ printf("Starting quicksort...\n");
				quicksort(ignorelist, --ignorelen);
				__BUGS__ printf("Quicksort finished.\n");
				flag = 0;
			} else if (argv[i][0]!='-') break;
			else for (int j = 1; argv[i][j]; j++) {
				switch(*(argv[i]+j)) {
					case 'f':
						ops |= 0x01; break;
					case 'e':
						ops &=~0x02; break;
					case 'd':
						ops |= 0x02; break;
					case 'v':
						ops |= 0x04; break;
					case 'b':
						ops |= 0x08; break;
					case 'i':
						flag = ops&0x10? flag : flag|0x01;
						ops |= 0x10; break;
					default:
						printf("Invalid code: %c 0x%02x\n", *(argv[i]+j), *(argv[i]+j));
				}
			}
		}
	}

	__BUGS__ printf("argc: %d\ni: %d\nops: %02X\n", argc, i, ops);
	__BUGS__ printf("ignore: %s\nlen:%hhu\n", ignorelist, ignorelen);

	char *input;
	char *output;
	unsigned int len = 0;

	__FILEMODE__ {
		FILE *file = fopen(argv[i], "r");
		if (!file) {
			printf("Could not open file: %s.\n", argv[i]);
			return ERR_OPEN_FILE;
		}

		fseek(file, 0, SEEK_END);
		len = ftell(file);
		fseek(file, 0, SEEK_SET);

		input = malloc(sizeof(char)*len+1);
		if (!input) {
			printf("Err: Could not allocate memory for input\n");
			return ERR_NO_MEMORY_INPUT;
		}

		unsigned int j = 0;
		int c;
		while ((c = fgetc(file)) != EOF) {
			__IGNORE__ {
				while (ignore(c)) {
					c = fgetc(file);
					len--;
				}
			}
			input[j++] = c;
		}

		fclose(file);
	} else {
		// Calcula tamanho do input
		__IGNORE__ {
			for (register unsigned int off = 0; argv[i][off]; off++) {
				if (!ignore(argv[i][off]))
					len++;
			}
		} else while (*(argv[i]+len)) len++;
		input = malloc(sizeof(char)*len+1);
		if (!input) {
			printf("Err: Could not allocate memory for input\n");
			return ERR_NO_MEMORY_INPUT;
		}

		// Copia para o input
		for (register int j = 0; argv[i][j]; j++)
			input[j] = argv[i][j];
	}

	__DECODE__ {
		output = malloc(sizeof(char)*len*3/4+1);
		if (!output) {
			printf("Err: Could not allocate memory for output\n");
			return ERR_NO_MEMORY_OUTPUT;
		}

		base64toascii(input, output);
		__VERBOSE__ printf("\nmain decode:\n%s\n\n%s\n", input, output);
		else printf("%s", output);

	} else {
		output = malloc(sizeof(char)*len*4/3+4);
		if (!output) {
			printf("Err: Could not allocate memory for output\n");
			return ERR_NO_MEMORY_OUTPUT;
		}

		asciitobase64(input, output);
		__VERBOSE__ printf("\nmain encode:\n%s\n\n%s\n", input, output);
		else printf("%s", output);
	}

	return 0;
}

// ToDo change algorithm to binary search
char ignore(char c) {
	register unsigned char start = 0, end = ignorelen - 1, now = end / 2;
	while (start < end) {
		if (ignorelist[now] == c) return 1;
		if (ignorelist[now] > c) start = now + 1;
		else end = now - 1;
		now = (end - start)/2 + start;
	}
	return 0;
	/*
	register char i = 0;
	while (ignorelist[i]) {
		if ((ignorelist[i++] == c)) return 1;
	}
	return 0;
	*/
}

int asciitobase64(const char *input, char *output) {
	// Controla o flow do while loop
	char flag = 0;

	// Caractere no ultimo loop
	unsigned char last = '\0';

	// Controle de onde estou na "string"
	size_t t = 0, i = 0;

	// While não estou no final da string
	while (*(input + t)) {
		// Pega proximo caractere
		unsigned char now = flag-3? *(input + t++) : now;
		__IGNORE__ {
			while (ignore(now)) now = flag-3? *(input + t++) : now;
		}


		// Conversao magica em binario
		*(output + i++) = _DICTIONARY_[ ((last << 6-flag*2) | (now >> (flag+1)*2)) & 0x3F ];

		// Debuging
		__BUGS__ printf("bta loop: t=%lu i=%lu last=%c now=%c new=%X - %c\n", t, i, last, now, *(output+i-1), *(output+i-1));

		// Se for o ultimo caractere
		if (!(*(input+t))) {
			switch (flag) {
				case 0:
					*(output + i++) = _DICTIONARY_[ (now << 4) & 0x30 ];
					// 2 '='
					*(output + i++) = _DICTIONARY_[64];
					*(output + i++) = _DICTIONARY_[64];
					break;
				case 1:
					*(output + i++) = _DICTIONARY_[ (now << 2) & 0x3C ];
					// 1 '='
					*(output + i++) = _DICTIONARY_[64];
					break;
				case 2:
					*(output + i++) = _DICTIONARY_[ now & 0x3F ];
					break;
				case 3: // Nunca cai aqui
					*(output + i++) = _DICTIONARY_[ last & 0x3F ];
					__VERBOSE__ printf("\n\nCAIU AQUI!\n");
					else __BUGS__ printf("\n\nCAIU AQUI!\n");
					break;
			}
		}

		// Andando em mod 4
		flag = (flag+1)%4;
		last = flag? now : '\0';
	}

	*(output + i++) = '\0';
	return 0;
}

unsigned char tohex(char input) {
	register char i = 0;
	for (; i < 64; i++) {
		if ((input == _DICTIONARY_[i])) break;
	}
	// Debuging
	__BUGS__ printf("tohex: %c -> %hhd\n", input, i);
	return i;
	/* Alternative
	if (!(input&0xD0)) { // + and /
		i = 62;
	} else if (!(input&0xC0)) { // Numbers
		i = 53;
	} else if (!(input&0x80)) { // p-z
		i = 41;
	} else if (!(input&0x90)) { // a-o
		i = 26;
	} else if (!(input&0xA0)) { // P-Z
		i = 15;
	}
	 */
}

int base64toascii(const char *input, char *output) {
	// Controla o flow do while loop
	char flag = 0;

	// Caractere no ultimo loop
	unsigned char last = '\0';

	// Controle de onde estou na "string"
	size_t t = 0, i = 0;

	// While não estou no final da string e não é '='
	while (*(input + t) && *(input + t)-'=') {
		// Pega proximo caractere e converte para bin com 6 digitos (A -> 00 0001)
		unsigned char now = *(input + t++);
		__IGNORE__ {
			while(ignore(now)) now = *(input + t++);
		}
		now = tohex(now);

		// Se esta no estado 1, 2 ou 3
		if (flag) {
			// Conversao magica em binario
			*(output + i++) = (last << (2*flag)) | (now >> (6-2*flag));

			// Debuging
			__BUGS__ printf("atb loop: t=%lu i=%lu last=%c now=%c new=%X - %c\n", t, i, last, now, *(output+i-1), *(output+i-1));
		}

		// Andando em mod 4
		flag = (flag+1)%4;
		last = now;
	}

	*(output + i++) = '\0';
	return 0;
}

char parseMacros(const char *input, unsigned char *i) {
	if (input[(*i)++] != '\\')
		return input[*i-1];
	char c;
	switch (input[(*i)++]) {
		case '\\':
			return '\\';
		case '0':
			return '\0';
		case 't':
			return '\t';
		case 'n':
			return '\n';
		case 'o':
			c = input[(*i)++];
			char tempo = 0;
			if (c >= '0' && c <= '7') {
				tempo = (c - '0') * 64;
			} else {
				*i -= 1;
				return '\\';
			}
			c = input[(*i)++];
			if (c >= '0' && c <= '7') {
				tempo += (c - '0') * 8;
			} else {
				*i -= 2;
				return '\\';
			}
			c = input[(*i)++];
			if (c >= '0' && c <= '7') {
				tempo += (c - '0');
			} else {
				*i -= 3;
				return '\\';
			}
			return tempo;
		case 'x':
			c = input[(*i)++];
			char tempx = 0;
			if (c >= '0' && c <= '9') {
				tempx = (c - '0')*16;
			} else if (c >= 'a' && c <= 'f') {
				tempx = (c - 'a' + 10) * 16;
			} else if (c >= 'A' && c <= 'F') {
				tempx = (c - 'A' + 10) * 16;
			} else {
				*i -= 1;
				return '\\';
			}
			c = input[(*i)++];
			if (c >= '0' && c <= '9') {
				tempx += (c - '0');
			} else if (c >= 'a' && c <= 'f') {
				tempx += (c - 'a' + 10);
			} else if (c >= 'A' && c <= 'F') {
				tempx += (c - 'A' + 10);
			} else {
				*i -= 2;
				return '\\';
			}
			return tempx;
		default:
			return '\\';
	}
	return 0;
}

unsigned char quicksort(char *list, unsigned char len) {
	if (len <= 1) {
		return 0;
	}
	register unsigned char pivot = 0, end = len - 1;
	while(pivot != end) {
		if (*(list + pivot) >= *(list + pivot+1)) {
			register unsigned char tmp = *(list + pivot);
			*(list + pivot) = *(list + pivot+1);
			*(list + pivot+1) = tmp;
			pivot += 1;
		} else {
			register unsigned char tmp = *(list + pivot+1);
			*(list + pivot+1) = *(list + end);
			*(list + end) = tmp;
			end -= 1;
		}
	}
	return quicksort(list, pivot) || quicksort(list + pivot+1, len - pivot-1);
}

void help(const char *name) {
	printf("usage: %s [-<options>] [argument] <input>\n", name);
	printf("\n\tOptions:\n\n");
	printf("\t-f\tFile mode\n");
	printf("\t\t\t\tReads input from a file. <input> is now <filemane>\n\n");
	printf("\t-e\tEncoding\n");
	printf("\t\t\t\tEncodes from ascii to base64. (default)\n\n");
	printf("\t-d\tDecoding\n");
	printf("\t\t\t\tDecodes from base64 to ascii.\n\n");
	printf("\t-v\tVerbose\n");
	printf("\t\t\t\tSays that the program is doing.\n\n");
	printf("\t-b\tBug finding\n");
	printf("\t\t\t\tHelps finding bugs from (en/de)crypts loops.\n\n");
	printf("\t-i\tIgnore\n");
	printf("\t\t\t\tIgnores characters from next argument.\n\n");

	putchar(10);
}
