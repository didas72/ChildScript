//main.c - Entry point for the program

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "errs.h"



int main(int argc, char *argv[])
{
	printf("Child Script Interpreter v0.1 by Didas72\n");

	if (argc != 2)
	{
		printf("Usage:\n\tchild_script <path_to_src>\n");
		return 1;
	}

	FILE* src_file = fopen(argv[1], "r");
	if (!src_file)
	{
		printf(ERR_SRC_FILE, argv[1]);
		return 1;
	}

	size_t line_count;
	char **lines = load_src_lines(src_file, &line_count);
	//printf("Found %ld lines.\n", line_count);
	fclose(src_file);

	interpret_source(lines, line_count);

	for (size_t i = 0; i < line_count; i++)
		free(lines[i]);
	free(lines);

	return 0;
}



//Read all lines in 2 passes (suboptimal)
char **load_src_lines(FILE *file, size_t *line_count)
{
	long linec = 1, colc = 0;
	int ch;

	while ((ch = fgetc(file)) != EOF)
	{
		if (ch == '\n')
			linec++;
	}

	char **lines = malloc(linec * sizeof(char*));
	linec = 0;
	rewind(file);

	while ((ch = fgetc(file)) != EOF)
	{
		if (ch == '\n')
		{
			lines[linec] = malloc(colc + 1);

			fseek(file, -colc - 1, SEEK_CUR);
			fread(lines[linec], 1, colc, file);
			lines[linec][colc] = '\0';
			fseek(file, 1, SEEK_CUR);

			colc = 0;
			linec++;
		}
		else
			colc++;
	}

	//Read last line
	lines[linec] = malloc(colc + 1);
	fseek(file, -colc - 1, SEEK_CUR);
	fread(lines[linec], 1, colc, file);
	lines[linec][colc] = '\0';
	fseek(file, 1, SEEK_CUR);

	*line_count = linec;
	return lines;
}

char interpret_source(char **lines, size_t line_count)
{
	state_t *state = malloc(sizeof(state_t));
	if (!state) return 1;

	state->mem = calloc(MEM_MAX, sizeof(uint8_t));
	if (!state->mem) { free(state); return 1; }
	state->colors[COLOR_R] = state->colors[COLOR_G] = state->colors[COLOR_B] = state->colors[COLOR_Y] = 0;
	state->program_counter = 0;

	while (state->program_counter < line_count)
	{
		if (interpret_line(lines[state->program_counter], state))
		{
			printf("Joe got confused and threw a tantrum! Check line %ld.\n", state->program_counter);
			break;
		}
	}

	print_state(state);

	free(state->mem);
	free(state);

	return 0;
}

char interpret_line(char *line, state_t *state)
{
	//printf("Running line '%s'\n", line);
	state->program_counter++;

	if (line_is_empty(line) || line_is_comment(line))
		return 0;

	char *first = strtok(line, " ");
	char *second = strtok(NULL, " ");
	char *last = strtok(NULL, " ");

	int color;

	//printf("Tokens: %p %p %p\n", first, second, last);

	ERR_INSTR_IF(last || !first);

	//printf("Have first '%s'\n", first);

	if (str_is_label(first))
	{
		ERR_INSTR_IF(second);
		//Skip
		//PERFORMANCE: Store for later lookup
	}
	else if (!strcmp(first, "X"))
	{
		ERR_INSTR_IF(!second);

		ERR_COLOR_IF(second);
		state->colors[color] = 0;
	}
	else if (!strcmp(first, "V"))
	{
		ERR_INSTR_IF(!second);

		int x = parse_ball_num(second);
		ERR_NUM_IF(x < 0);

		state->colors[COLOR_R] = (state->colors[COLOR_R] + x) & 0xFF;
	}
	else if (!strcmp(first, "A"))
	{
		ERR_INSTR_IF(!second);
		
		int x = parse_ball_num(second);
		ERR_NUM_IF(x < 0);
		
		state->colors[COLOR_R] = state->colors[COLOR_R] > x ? (state->colors[COLOR_R] - x) & 0xFF : 0;
	}
	else if (!strcmp(first, "<->"))
	{
		ERR_INSTR_IF(!second);
		
		ERR_COLOR_IF(second);

		if (color == COLOR_R)
			return 0;

		uint8_t tmp = state->colors[COLOR_R];
		state->colors[COLOR_R] = state->colors[color];
		state->colors[color] = tmp;
	}
	else if (!strcmp(first, "<=>"))
	{
		ERR_INSTR_IF(!second);
		
		ERR_COLOR_IF(second);
		uint8_t tmp = state->colors[color];
		state->colors[color] = state->mem[state->colors[COLOR_Y]];
		state->mem[state->colors[COLOR_Y]] = tmp;
	}
	else if (!strcmp(first, "A>V"))
	{
		ERR_INSTR_IF(!second);
		
		ERR_COLOR_IF(second);
		state->colors[COLOR_R] = (state->colors[COLOR_R] + state->colors[color]) & 0xFF;
		state->colors[color] = 0;
	}
	else if (!strcmp(first, "="))
	{
		ERR_INSTR_IF(!second);
		
		ERR_COLOR_IF(second);
		state->colors[color] = state->colors[COLOR_R];
	}
	else if (!strcmp(first, "mama"))
	{
		ERR_INSTR_IF(second);
		
		//TODO: Handle EOF
		state->colors[COLOR_R] = getchar();
	}
	else if (!strcmp(first, "papa"))
	{
		ERR_INSTR_IF(second);
		
		printf("'%c' (%d)\n", state->colors[COLOR_R], state->colors[COLOR_R]);
	}
	else if (!strcmp(first, "O?"))
	{
		ERR_INSTR_IF(!second);

		//Jump to label if not zero
		ERR_LABEL_NVAL_IF(second);
		//TODO: Label searching
	}
	else
	{
		printf(ERR_INSTRUCTION);
		return 1;
	}

	return 0;
}

void print_state(state_t *state)
{
	printf("\n===State dump===\n");
	printf("R:0x%02x G:0x%02x B:0x%02x Y:0x%02x\n",
		state->colors[COLOR_R], state->colors[COLOR_G],
		state->colors[COLOR_B], state->colors[COLOR_Y]);
	printf("Mem:\nAddr  +0 +1 +2 +3 | +4 +5 +6 +7\n");
	for (int i = 0; i < MEM_MAX; i += 8)
	{
		printf("0x%02x: %02x %02x %02x %02x | %02x %02x %02x %02x\n", i,
			state->mem[i], state->mem[i + 1],
			state->mem[i + 2], state->mem[i + 3],
			state->mem[i + 4], state->mem[i + 5],
			state->mem[i + 6], state->mem[i + 7]);
	}
}


char line_is_empty(char *str)
{
	int i = 0;
	while (str[i] != '\0' && isspace(str[i])) i++;
	return str[i] == '\0';
}
char line_is_comment(char *str)
{
	size_t len = strlen(str);

	if (len >= 1 && str[0] == '#') return 1;
	return len >= 2 && str[0] == '/' && str[1] == '/';
}
char str_is_label(char *str)
{
	char ch;
	for (; (ch = *str); str++)
		if (parse_color(ch) == COLOR_NVAL)
			return 0;
	return 1;
}
int parse_color(char ch)
{
	if (ch == 'R') return COLOR_R;
	if (ch == 'G') return COLOR_G;
	if (ch == 'B') return COLOR_B;
	if (ch == 'Y') return COLOR_Y;
	return COLOR_NVAL;
}
int parse_color_str(char *str)
{
	if (strlen(str) != 1) return COLOR_NVAL;
	return parse_color(*str);
}
int parse_ball_num(char *str)
{
	int count = 0;

	while (*(str++) == 'O') count++;

	return *str ? -1 : count;
}
