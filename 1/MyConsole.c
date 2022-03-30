#include "MyConsole.h"
#include "Common.h"

bool instructions = true;
bool parameters = true;

void hello_world() {
	const static char* str =
		"Hello World!\nWelcome to Snake.\n\n"
		" * new        New game.\n"
		" * set        Set options variables.\n"
		" * cls        Clear screen.\n"
		" * exit       Close this window . . .\n\n";

	const static char* hint =
		" *** try 'hello', 'music' or 'os' to trigger easter egg!\n\n";

	printf("%s", str);
	if (rand() % 5 == 0)
		printf("%s", hint);
}

int parse(char(*arg)[ARG_LEN], char* str) {
	int a = 0, b = 0;
	bool space = true;
	memset(arg, 0, sizeof(arg[0][0]) * ARG_NUM * ARG_LEN);
	for (int i = 0; i < strlen(str); ++i) {
		if (str[i] == ' ') {
			b = 0;
			space = true;
		}
		else {
			if (space) {
				++a;
			}
			arg[a - 1][b] = str[i];
			arg[a - 1][b + 1] = 0;
			++b;
			space = false;

			if ((a > ARG_NUM - 1) || (b > ARG_LEN - 1)) {
				printf("ERROR: input too long. Array overflow\n\n");
				return 1;
			}
		}
	}
	return 0;
}

void execute_args(char(*arg)[ARG_LEN]) {
	if (arg[0][0] == 0) {
		return;
	}
	if (strcmp(arg[0], "new") == 0) {
		if (instructions) {
			didact();
		}
		struct LinkedList* snake = create_linked_list();
		if (game_snake(snake) == 1) {
			printf("Failed!\n");
			pause();
		}
		clear(snake);
		free(snake);
		clear_screen();
		hello_world();
	}
	else if (strcmp(arg[0], "set") == 0) {

		if (strcmp(arg[1], "status") == 0) {
			printf("Current Settings:\n");
			printf("instructions:\t%s\n", instructions ? "on" : "off");
			printf("parameters:\t%s\n\n", parameters ? "on" : "off");
		}
		else if (strcmp(arg[1], "ins") == 0) {

			if (strcmp(arg[2], "on") == 0) {
				instructions = true;
			}
			else if (strcmp(arg[2], "off") == 0) {
				instructions = false;
			}
			else {
				man_set();
			}
		}
		else if (strcmp(arg[1], "prm") == 0) {

			if (strcmp(arg[2], "on") == 0) {
				parameters = true;
			}
			else if (strcmp(arg[2], "off") == 0) {
				parameters = false;
			}
			else {
				man_set();
			}
		}
		else {
			man_set();
		}
	}
	else if (strcmp(arg[0], "exit") == 0 || strcmp(arg[0], "quit") == 0) {
		exit(0);
	}
	else if (strcmp(arg[0], "cls") == 0) {
		clear_screen();
		hello_world();
	}
	// easter eggs
	else if (strcmp(arg[0], "music") == 0) {
		printf("Listen to Johann Sebastian Bach!\n\n");
	}
	else if (strcmp(arg[0], "hello") == 0) {
		printf("'Hello!!!'\n  (^.^)\n\n");
	}
	else if (strcmp(arg[0], "bwv") == 0) {
		printf("BWV %d\n\n", rand() % 3000);
	}
	else if (strcmp(arg[0], "os") == 0) {
		const static char* os[] = {
			"CentOS 7", "CentOS 8", "CentOS Stream",
			"RedHat Enterprise Linux", "RHEL",
			"RockyLinux", "AlmaLinux", "Oracle Linux",
			"Fedora", "Scientific Linux",
			"Ubuntu", "MacOS",
			"Windows XP", "Windows Vista", "Windows 7",
			"Windows 10", "Windows 11",
		};
		int len = sizeof(os) / sizeof(os[0]);
		printf("%s\n\n", os[rand() % len]);
	}
	else {
		printf("'%s' is not recognized as an internal command.\n\n", arg[0]);
	}
}

void man_set() {
	const static char* str =
		"SYNTAX\n"
		"    set [status]\n"
		"    set [ins | prm] [on | off]\n\n"
		"PARAMETERS\n"
		"    No args    Display help.\n"
		"    status     Show current settings.\n"
		"    ins        Instructions on or off.\n"
		"    prm        Parameters on or off.\n"
		"    on         Turn on.\n"
		"    off        Turn off.\n\n"
		"EXAMPLE\n"
		"    set status\n"
		"    set ins on\n\n";

	printf("%s", str);
}

void didact() {
	const static char* didacts[] = {
		"'w' 's' 'a' 'd' to control;",
		"Game fails when the snake touches the border or its body;",
		"'h' to display help;",
		"'q' to quit;",
	};
	int len = sizeof(didacts) / sizeof(didacts[0]);
	for (int i = 0; i < len; ++i) {
		clear_screen();
		printf("%s\n\n", didacts[i]);
		pause();
	}
}

void show_parameters(char keypress, int head, int dst) {
	if (parameters) {
		printf("key: %c\n", keypress);
		printf("head: %d\n", head);
		printf("dst: %d\n", dst);
	}
}

void show_score(int score) {
	printf("score: %d\n", score);
	if (highest_score < score)
		highest_score = score;
	printf("highest: %d\n", highest_score);
}


/*
void display() {
	int x = 5;
	int y = 5;
	gotoxy(x, y);
	printf("%s", sep_line);
	gotoxy(x, ++y);
	for (int i = 0; i < HEIGHT; ++i) {
		printf("|");
		for (int j = 0; j < WIDTH; ++j) {
			switch (matrix[i][j])
			{
			case SNAKE_HEAD:
				printf("1 ");
				break;
			case SNAKE_BODY:
				printf("1 ");
				break;
			case SNAKE_DST:
				printf("0 ");
				break;
			default:
				printf("  ");
				break;
			}
		}
		printf("|");
		gotoxy(x, ++y);
	}
	printf("%s", sep_line);
}
*/

void display() {
	printf("%s\n", sep_line);
	for (int i = 0; i < HEIGHT; ++i) {
		printf("|");
		for (int j = 0; j < WIDTH; ++j) {
			switch (matrix[i][j])
			{
			case SNAKE_HEAD:
				printf("1 ");
				break;
			case SNAKE_BODY:
				printf("1 ");
				break;
			case SNAKE_DST:
				printf("0 ");
				break;
			default:
				printf("  ");
				break;
			}
		}
		printf("|\n");
	}
	printf("%s\n", sep_line);
}

void do_sleep(double seconds) {
	clock_t start, end;
	double duration = 0;
	start = clock();
	while (duration < seconds) {
		end = clock();
		duration = (double)(end - start) / CLOCKS_PER_SEC;
	}
}

void pause() {
	printf("Press Enter to continue...: ");
	char c = 0;
	while ((c = getchar()) != '\n' && c != EOF);
}