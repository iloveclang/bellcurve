#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <time.h>
#endif

#define LEFT 		0
#define RIGHT 		1

#define GRUG		0
#define MIDWIT		1
#define WOJAK		2

#define C_BROWN		"\033[38;5;138m"
#define C_RED		"\033[38;5;203m"
#define C_BLUE		"\033[38;5;39m"
#define C_GREY		"\033[38;5;240m"
#define C_0			"\033[0m"

#define TXT_W_MAX	25
#define TXT_H_MAX	3

#define MY_OP		0
#define YOUR_OP		1


#define BELLCURVE_DRAW_DELAY_MS		100

struct vec2 {
	int y;
	int x;
};

struct facedata {
	int height;
	int width;
	int orientation;
	int color_count;
	char *data;
	char **colors;
};

void strnull(char *str, int max) {
	int i;
	for (i = 0; i < max; i++) {
		if (str[i] == '\0')
			break;
		else
			str[i] = '\0';
	}
}

int strncpylen(char *dest, const char *src, int n) {
	int i, l;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	
	l = i;

	for ( ; i < n; i++)
		dest[i] = '\0';

   return l;
}

void sleep_ms(int x) {
	if (x > 999 || x < 0) {
		fprintf( stderr, "sleep_ms() argument outside of range (0-999)\n");
		exit(EXIT_FAILURE);
	}

	#ifdef _WIN32
		Sleep(x);
	#else
		struct timespec timer;

		timer.tv_sec = 0;
		timer.tv_nsec = (x * 1000000);

		nanosleep(&timer, NULL);
	#endif
}

int randomn(int seed, int x) {
// (returns int between 0 and (x - 1). seed should be > x. x should be > 0. compiler error if x is 0.)
	return (seed % x);
}

void clear_screen(void) {
	int i;

	for (i = 0; i < 23; i++) {	
	// don't clear 24th line so the terminal doesn't scroll
		printf("\033[%d;1H                                                                                          ", (i + 1));
	}
	fflush(stdout);
}

void draw_text(const char *str, struct vec2 *pos, int max_height, int max_width) {

	char **lines;
	int *lengths;
	int i, ii, main_i;
	int str_len;
	int line_count, offset;

	lines = malloc(max_height * sizeof(char*));
	for (i = 0; i < max_height; i++) {
		lines[i] = calloc((max_width + 1), sizeof(char));
	}
	lengths = malloc(max_height * sizeof(int));

	for (i = 0, main_i = 0, line_count = 0; i < max_height; i++) {
		lengths[i] = strncpylen(lines[i], &str[main_i], max_width);
		++line_count;		

		if (lengths[i] == max_width) {
			for (ii = (lengths[i] - 1); ii >= 0; ii--) {
				if (lines[i][ii] == ' ') {
					lengths[i] = (ii + 1);
					lines[i][ii] = '\0';
					break;
				}
			}
			main_i += (lengths[i] - 1);
		}
		else {
			break;
		}
	}
	
	offset = 0;
	if (line_count < max_height) {
		offset = (int)((max_height - line_count) / 2);
	}

	for (i = 0; i < line_count; i++) {
		printf("\033[%d;%dH%s", (pos->y + offset + i), (pos->x + (int)((max_width - lengths[i]) / 2)), lines[i]);
		fflush(stdout);
	}

	for (i = 0; i < max_height; i++) {
		free(lines[i]);
	}
	free(lines);
	free(lengths);
}

void draw_face(struct vec2 *pos, struct facedata facestruct[], int face, int facing) {
	int i, ii, c;
	char *linebuffer = calloc((facestruct[face].width + 1), sizeof(char));
	int buf_pos;
	int x_count, total_x_count, x_line_count;
	int color_flipper;

	total_x_count = 0;
	x_count = 0;
	color_flipper = 1;
	if (facestruct[face].orientation == facing) {
		for (i = 0; i < facestruct[face].height; i++) {
			strnull(linebuffer, facestruct[face].width);
			buf_pos = 0;
			total_x_count += x_count;
			x_count = 0;
			for (ii = 0; (ii - x_count) < facestruct[face].width; ii++) {
				if (facestruct[face].data[(i * facestruct[face].width + ii + total_x_count)] == 'x') {
					printf("\033[%d;%dH%s%s", (pos->y + i), (pos->x + ii - x_count - buf_pos), linebuffer, facestruct[face].colors[total_x_count + x_count]);
					++x_count;
					strnull(linebuffer, facestruct[face].width);
					buf_pos = 0;
				}
				else {
					linebuffer[buf_pos++] = facestruct[face].data[i * facestruct[face].width + ii + total_x_count];
				}
			}
			printf("\033[%d;%dH%s", (pos->y + i), (pos->x + ii - x_count - buf_pos), linebuffer);
		}
	}
	else {
		for (i = 0; i < facestruct[face].height; i++) {
			strnull(linebuffer, facestruct[face].width);
			buf_pos = 0;
			x_line_count = 0;
			for (ii = 0; (ii - x_line_count) < facestruct[face].width; ii++) {
				if (facestruct[face].data[i * facestruct[face].width + ii + total_x_count] == 'x') {
					++x_line_count;
				}
			}
			total_x_count += x_line_count;
			x_count = 0;
			for (ii = (facestruct[face].width + x_line_count - 1); ii >= 0; ii--) {
				if ((c = facestruct[face].data[i * facestruct[face].width + ii + total_x_count - x_line_count]) == 'x') {
					printf("\033[%d;%dH%s%s", (pos->y + i), (pos->x + (facestruct[face].width + x_line_count) - ii - x_count - buf_pos), linebuffer, facestruct[face].colors[(total_x_count - (x_count + 1) + (color_flipper *= (-1)))]);
					++x_count;
					strnull(linebuffer, facestruct[face].width);
					buf_pos = 0;
				}
				else {
					if (c == '\\')
						linebuffer[buf_pos++] = '/';
					else if (c == '/')
						linebuffer[buf_pos++] = '\\';
					else
						linebuffer[buf_pos++] = c;
				}
			}
			printf("\033[%d;%dH%s", (pos->y + i), (pos->x + (facestruct[face].width + x_line_count) - ii - x_count - buf_pos), linebuffer);
		}
	}
	fflush(stdout);
	free(linebuffer);
}

void draw_bellcurve(struct vec2 *pos, int color, int timer) {
	int i;

	i = 0;
	printf("\033[%d;%dH%s_", (pos->y), (pos->x + 33), C_0);
	printf("\033[%d;%dH%s_", (pos->y + i++), (pos->x + 37), C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH34%%  /\033[48;5;%dm  |  %s\\  34%%", (pos->y + i++), (pos->x + 27), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH_/\033[48;5;%dm   |   %s\\_", (pos->y + i++), (pos->x + 30), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH/\033[48;5;%dm     |     %s\\", (pos->y + i++), (pos->x + 29), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH/\033[48;5;%dm      |      %s\\", (pos->y + i++), (pos->x + 28), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH|\033[48;5;%dm%s______60%%______%s|", (pos->y + i++), (pos->x + 27), color, C_GREY, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH/\033[48;5;%dm       |       %s\\", (pos->y + i++), (pos->x + 27), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH/\033[48;5;%dm|       |       |%s\\", (pos->y + i++), (pos->x + 26), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH14%% _/\033[48;5;%dm |       |       | %s\\_ 14%%", (pos->y + i++), (pos->x + 20), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH__/\033[48;5;%dm   |       |       |   %s\\__", (pos->y + i++), (pos->x + 21), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH2%% ____/\033[48;5;%dm      |       |       |      %s\\____ 2%%", (pos->y + i++), (pos->x + 13), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH_0.1%%_____/\033[48;5;%dm   |%s_______%s\033[48;5;%dm|%s______95%%______%s\033[48;5;%dm|%s_______%s\033[48;5;%dm|   %s\\_____0.1%%_", (pos->y + i++), (pos->x + 5), color, C_GREY, C_0, color, C_GREY, C_0, color, C_GREY, C_0, color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH/\033[48;5;%dm_______|_______|_______|_______|_______|_______|_______|_______%s\\", (pos->y + i++), (pos->x + 3), color, C_0);
	fflush(stdout); sleep_ms(timer);
	printf("\033[%d;%dH%sIQ score   55      70      85     100     115     130     145%s", (pos->y + i++), pos->x, C_RED, C_0);
	fflush(stdout); 
	sleep_ms(timer);
	sleep_ms(timer);
}

char ***read_data(const char *file, int *line_count, int line_size) {
	FILE *fp;
	char ***lines;
	int i, ii;
	int c;
	int linecount;

	linecount = 0;
	fp = fopen(file, "r");
	if (fp == NULL) {
		perror("ERROR while trying to read file");
		exit(EXIT_FAILURE);
	}

	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n')
			++linecount;
	}

	if (linecount <= 0) {
		fprintf(stderr, "no lines in file %s. file must contain newline-terminated strings in alternating opinion pairs\n", file);
		exit(EXIT_FAILURE);
	}

	if ((linecount % 2)) {
	// if unmatched pair or last newline
		--linecount;
	}
	
	linecount /= 2;
	
	lines = malloc(2 * sizeof(char**));
	lines[MY_OP] = malloc(linecount * sizeof(char*));
	lines[YOUR_OP] = malloc(linecount * sizeof(char*));

	for (i = 0; i < linecount; i++) {
		lines[MY_OP][i] = calloc(line_size, sizeof(char));
		lines[YOUR_OP][i] = calloc(line_size, sizeof(char));
	}

	rewind(fp);

	for (i = 0; i < linecount; i++) {
		fgets(lines[MY_OP][i], line_size, fp);
		for (ii = 0; (ii < line_size) && (lines[MY_OP][i][ii] != '\0'); ii++) {
			if (lines[MY_OP][i][ii] == '\n') {
				lines[MY_OP][i][ii] = '\0';
				break;
			}
		}

		fgets(lines[YOUR_OP][i], line_size, fp);
		for (ii = 0; (ii < line_size) && (lines[YOUR_OP][i][ii] != '\0'); ii++) {
			if (lines[YOUR_OP][i][ii] == '\n') {
				lines[YOUR_OP][i][ii] = '\0';
				break;
			}
		}
	}

	fclose(fp);

	*line_count = linecount;

	return lines;
}

void free_read_data(char ***lines, int linecount) {
	int i;

	for (i = 0; i < linecount; i++) {
		free(lines[MY_OP][i]);
		free(lines[YOUR_OP][i]);
	}
	free(lines[YOUR_OP]);
	free(lines[MY_OP]);
	free(lines);
}


int main(int argc, char *argv[]) {
	char ***lines;
	int line_count;
	int i;
	int num, temp;
	int loops;

	struct vec2 face_pos[3] = {
		{14, 4},
		{4, 33},
		{14, 65}
	};
	
	struct vec2 text_pos[3] = {
		{9, 3},
		{1, 27},
		{9, 52}
	};

	struct vec2 bellcurve_pos = {10, 5};

	struct facedata faces[3] = {
		{
			7, 14, RIGHT, 0,
			"     __ __    "
			"    /  \\  \\   "
			"  _/ ===== |_ "
			"  \\@   _|   @/"
			"   | \\___  /  "
			" _/      _/   "
			"/    ;  \\__   "
		},
		{
			7, 14, LEFT, 20,
			" x||////////x   "
			" /       /#   "
			" /_==_===|#_  "
			"|x@xx_x|  |x@xx_x| \\ \\"
			"\\x\"\"x ;; x\"\"x  |  "
			" x\"x\\####x\"\"x  |  "
			"   _/ ;x\"\"x   \\_"
		},
		{
			7, 14, LEFT, 22,
			"       x_/ /x   "
			"    x__/_  \\x   "
			"   x/xx####xx\\  |x  "
			"  x|x G/ G x| |x  "
			"  x|\\x __/ x| |x  "
			" x_\\_\\____|_|x  "
			"x/           \\x "	// can start but mustn't end with x
		}
	};

	faces[MIDWIT].colors = (char *[]) {
		C_BROWN, C_0,
		C_RED, C_0, C_BLUE, C_0, C_RED, C_0, C_BLUE, C_0,
		C_BLUE, C_0, C_BLUE, C_0,
		C_BLUE, C_0, C_BLUE, C_0,
		C_BLUE, C_0
	};

	faces[WOJAK].colors = (char *[]) {
		C_BROWN, C_0,
		C_BROWN, C_0,
		C_BROWN, C_0, C_GREY, C_0, C_BROWN, C_0,
		C_BROWN, C_0, C_BROWN, C_0,
		C_BROWN, C_0, C_BROWN, C_0,
		C_BROWN, C_0,
		C_BROWN, C_0
	};

	int face_order[3] = {GRUG, MIDWIT, WOJAK};

	if (argc != 3) {
		printf(
			"\nProgram needs path to a file with alternating my_opinion - your_opinion lines. "
			"\nIt also needs a number depticting how many times it should loop"
			"\nExample:\n$./bellcurve \"my_file.txt\" 100\n\n"
		);
		exit(EXIT_SUCCESS);
	}
	
	lines = read_data(argv[1], &line_count, (TXT_W_MAX * TXT_H_MAX));

	if ((loops = atoi(argv[2])) == 0) {
		fprintf(stderr, "second arg is not a number\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < loops; i++) {
		num = randomn(lines[((lines[MY_OP][randomn(((unsigned int) clock()), line_count)][0]) & 1)][randomn(((unsigned int) clock()), line_count)][0], 3);
		// let last byte of first char of MY_OP decide if randomn uses lines[0] or [1] to return the char value 
		// I'm just replacing i with another randomn() because i is no longer <= line_count
		temp = face_order[num];
		face_order[num] = face_order[0];
		face_order[0] = temp;		
		
		if ((lines[MY_OP][randomn(((unsigned int) clock()), line_count)][0]) & 1) 
			num = randomn(lines[MY_OP][randomn(((unsigned int) clock()), line_count)][0], 2);
		else
			num = randomn(lines[YOUR_OP][randomn(((unsigned int) clock()), line_count)][0], 2);

		if (num) {
			temp = face_order[2];
			face_order[2] = face_order[1];
			face_order[1] = temp;
		}

		draw_face(&face_pos[0], faces, face_order[0], RIGHT);
		draw_face(&face_pos[1], faces, face_order[1], LEFT);
		draw_face(&face_pos[2], faces, face_order[2], LEFT);

		num = randomn(((unsigned int) clock()), line_count);

		draw_text(lines[MY_OP][num], &text_pos[0], TXT_H_MAX, TXT_W_MAX);
		draw_text(lines[YOUR_OP][randomn(((unsigned int) clock()), line_count)], &text_pos[1], TXT_H_MAX, TXT_W_MAX);
		draw_text(lines[MY_OP][num], &text_pos[2], TXT_H_MAX, TXT_W_MAX);

		draw_bellcurve(&bellcurve_pos, (randomn(((unsigned long int) clock()), 213) + 17), BELLCURVE_DRAW_DELAY_MS);

		clear_screen();
	}

	free_read_data(lines, line_count);

	return 0;
}