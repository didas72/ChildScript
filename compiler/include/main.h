//main.h - 

#pragma once

#include <stdio.h>


enum REGISTER
{
	REG_NVAL = -1,
	REG_R = 0,
	REG_G = 1,
	REG_B = 2,
	REG_Y = 3,
	REG_T = 4,
};

enum INSTRUCTION
{
	INST_NVAL = -1,
	EMPTY = 0,
	ADD = 1,
	REMOVE = 2,
	REG_SWAP = 3,
	MEM_SWAP = 4,
	MERGE = 5,
	EQUAL = 6,
	INPUT = 7,
	OUTPUT = 8,
	BRANCH = 9,
};


char **load_src_lines(FILE *file, size_t *line_count);

int compile_source(char **lines, size_t line_count);
int compile_line(char *line, size_t line_num, FILE* out);

char line_is_empty(char *str);
char line_is_comment(char *str);
char str_is_label(char *str);
int parse_reg(char ch);
int parse_reg_str(char *str);
int parse_ball_num(char *str);
char *get_reg_str(int color);
