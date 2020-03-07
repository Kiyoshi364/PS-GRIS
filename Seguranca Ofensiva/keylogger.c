#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
	char filename[] = "/dev/input/event0";
	if (argc > 1 && argv[1][0] >= '0' && argv[1][0] <= '9') {
		filename[16] = '4';
	}
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("Could not open file: %s\nEither it does not exist or does not have permission\nTry: sudo %s", filename, *argv);
		return 1;
	}

	char *normal[] = {
		"dummy",	"<ESC>",	"1",		"2",		"3",		"4",		"5",		"6",		"7",		"8",		// 00
		"9",		"0",		"-",		"=",		"<BACK>",	"<TAB>",	"q",		"w",		"e",		"r",		// 10
		"t",		"y",		"u",		"i",		"o",		"p",		"`",		"[",		"<ENTER>",	"<LCTRL>",	// 20
		"a",		"s",		"d",		"f",		"g",		"h",		"j",		"k",		"l",		"ç",		// 30
		"^",		"'",		"<LSHIFT>",	"]",		"z",		"x",		"c",		"v",		"b",		"n",		// 40
		"m",		",",		".",		";",		"<RSHIFT>",	"<NUM*>",	"<LALT>",	"	",		"<CAPS>",	"<F1>",		// 50
		"<F2>",		"<F3>",		"<F4>",		"<F5>",		"<F6>",		"<F7>",		"<F8>",		"<F9>",		"<F10>",	"<NUMLK>",	// 60
		"<SCROLL>",	"<NUM7>",	"<NUM8>",	"<NUM9>",	"<NUM->",	"<NUM4>",	"<NUM5>",	"<NUM6>",	"<NUM+>",	"<NUM1>",	// 70
		"<NUM2>",	"<NUM3>",	"<NUM0>",	"<NUM,>",	"dummy",	"dummy",	"\\",		"<F11>",	"<F12>",	"dummy",	// 80
		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"\n",		"<RCTRL>",	"/",		"<PRTS>",	// 90
		"<RALT>",	"dummy",	"<HOME>",	"<UARR>",	"<PGUP>",	"<LARR>",	"<RARR>",	"<END>",	"<DARR>",	"<PGDN>",	// 100
		"<INSERT>",	"<DEL>",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	// 110
		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"<SUPER>",	"dummy",	"dummy",	"dummy",	"dummy",	// 120
/*		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	// 130
		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	// 140
		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	// 150
		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	// 160
		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	// 170
		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	// 180
		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	// 190
		"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	"dummy",	// 200
*/		NULL
	};

	register char l = 0;
	while (l++, !feof(file)) {
		char **keyboard = normal;

		register char i = 0;
		unsigned long time = 0;
		for (i = 0; i < 4; i++) {
			time |= fgetc(file) << (8 * i);
		}
		//printf("time: %02u:%02u:%02d:%02d\n", time/86400, (time/3600)%24, (time/60)%60, time%60);

		for (i = 0; i < 38; i++)
			fgetc(file);

		char key = fgetc(file);
		fgetc(file);
		char mode = fgetc(file);

		switch (mode) {
			case 0:
				printf("time: %s", ctime(&time));
				printf("key: %d ^: %s\n", key, keyboard[key]);
				break;
			case 1:
				printf("time: %s", ctime(&time));
				printf("key: %d v: %s\n", key, keyboard[key]);
				break;
			case 2:
				printf("time: %s", ctime(&time));
				printf("key: %d h: %s\n", key, keyboard[key]);
				break;
		}

		for (i = 0; i < 27; i++)
			fgetc(file);
	}
	return 0;
}
