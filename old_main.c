#include <stdio.h>

#define MAX_STR_SIZE_MY		32
#define MAX_STR_SIZE_YOUR	78


int non_shit_strncpy(char *dest, const char *src, int n) {
	int i, l;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	
	l = i;

	for ( ; i < n; i++)
		dest[i] = '\0';

   return l;
}

int main(int argc, char** argv) {
	char my_opinion[MAX_STR_SIZE_MY + 1];
	char your_opinion[MAX_STR_SIZE_YOUR + 1];	
	int my_opinion_length;
	int your_opinion_length;

	if (argc != 3) {
		printf("\nProgram needs 2 arguments. Example:\n$./bellcurve \"my opinion\" \"your opinion\"\n\n");
	}
	else {
		printf(
			"\n\n\n"
			"                                 \033[38;5;138m||////////\033[0m     \n"
			"                                 /       /#     \n"                                   
			"                                 /_==_===|#_    \n"
			"                                |\033[38;5;167m@\033[38;5;39m_\033[0m|  |\033[38;5;167m@\033[38;5;39m_\033[0m| \\ \\  \n"
			"                                \\\033[38;5;39m\"\"\033[0m ;; \033[38;5;39m\"\"\033[0m  |    \n"
			"                                 \033[38;5;39m\"\033[0m\\####\033[38;5;39m\"\"\033[0m  |    \n"
			"                                   _/_;\033[38;5;39m\"\"\033[0m_  \\_  \n"
			"                               34%%  /\033[48;5;75m  |  \033[0m\\  34%%   \n"
			"                                  _/\033[48;5;75m   |   \033[0m\\_      \n"
			"                                 /\033[48;5;75m     |     \033[0m\\     \n"
			"        __ __                   /\033[48;5;75m      |      \033[0m\\                       \033[38;5;138m_/ /\033[0m   \n"
			"       /  \\  \\                 |\033[48;5;75m\033[38;5;240m______60%______\033[0m|                   \033[38;5;138m__/_  \\\033[0m   \n"
			"     _/ ===== |_               /\033[48;5;75m       |       \033[0m\\                  \033[38;5;138m/\033[38;5;240m####\033[38;5;138m\\  |\033[0m  \n"
			"     \\@   _|   @/             /\033[48;5;75m|       |       |\033[0m\\                \033[38;5;138m|\033[0m G/ G \033[38;5;138m| |\033[0m  \n"
			"      | \\___  /         14%% _/\033[48;5;75m |       |       | \033[0m\\_ 14%%          \033[38;5;138m|\\\033[0m __/ \033[38;5;138m| |\033[0m  \n"
			"    _/      _/           __/\033[48;5;75m   |       |       |   \033[0m\\__          \033[38;5;138m_\\_\\____|_|\033[0m  \n"
			"   /    ;  \\__   2%% ____/\033[48;5;75m      |       |       |      \033[0m\\____ 2%% \033[38;5;138m/           \\\033[0m \n"
			"         _0.1%%_____/\033[48;5;75m   |\033[38;5;240m_______\033[0m\033[48;5;75m|\033[38;5;240m______95%%______\033[0m\033[48;5;75m|\033[38;5;240m_______\033[0m\033[48;5;75m|   \033[0m\\_____0.1%%_      \033[38;5;138m|\033[0m\n"
			"       /\033[48;5;75m_______|_______|_______|_______|_______|_______|_______|_______\033[0m\\     \n"  
			"\033[0m\033[38;5;203m    IQ score   55      70      85     100     115     130     145             \n"
			"\033[38;5;15mimgflip.com\033[0m"
		);

		my_opinion_length = non_shit_strncpy(my_opinion, argv[1], MAX_STR_SIZE_MY);
		your_opinion_length = non_shit_strncpy(your_opinion, argv[2], MAX_STR_SIZE_YOUR);

		printf("\033[3;%dH%s", ((MAX_STR_SIZE_YOUR - your_opinion_length) / 2), your_opinion);
		printf("\033[13;%dH%s", ((MAX_STR_SIZE_MY - my_opinion_length) / 2 + 1), my_opinion);
		printf("\033[13;%dH%s", ((MAX_STR_SIZE_MY - my_opinion_length) / 2 + 47), my_opinion);
		printf("\033[24;12H\n");
		fflush(stdout);
	}

	return 0;
}          