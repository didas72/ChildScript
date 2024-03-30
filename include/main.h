//main.h - 

#pragma once

#include <stdio.h>
#include <stdint.h>

#define MEM_MAX 256



enum COLOR
{
	COLOR_NVAL = -1,
	COLOR_R = 0,
	COLOR_G = 1,
	COLOR_B = 2,
	COLOR_Y = 3,
};


typedef struct
{
	uint8_t colors[4];
	size_t program_counter;
	uint8_t *mem;
} state_t;



char **load_src_lines(FILE *file, size_t *line_count);
char interpret_source(char **lines, size_t line_count);
char interpret_line(char **lines, size_t line_count, state_t *state);
void print_state(state_t *state);

char line_is_empty(char *str);
char line_is_comment(char *str);
char str_is_label(char *str);
int parse_color(char ch);
int parse_color_str(char *str);
int parse_ball_num(char *str);
