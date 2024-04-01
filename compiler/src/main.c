//main.c - Entry point for the program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "errs.h"
#include "main.h"

#include "embed/base.asm.h"


int main(int argc, char *argv[])
{
	printf("Child Script Compiler v0.1 by Didas72\n");

	if (argc != 2)
	{
		printf("Usage:\n\tchs_compiler <path_to_src>\n");
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
	fclose(src_file);

	return compile_source(lines, line_count);
}



int compile_source(char **lines, size_t line_count)
{
	size_t line = 0;
	FILE* out = fopen("transpiled_chs.asm", "w");
	fwrite(embed_base_asm, 1, embed_base_asm_len, out);

	for (; line < line_count; line++)
	{
		if (compile_line(lines[line], line, out))
			goto compile_source_fail;
	}

	fprintf(out, "\n; Exit cleanly\n\tcall exit\n");

	fclose(out);
	return 0;

compile_source_fail:
	fclose(out);
	fprintf(stderr, "Compilation failed.\n");
	return 1;
}

int compile_line(char *line, size_t line_num, FILE* out)
{
	if (line_is_empty(line) || line_is_comment(line))
		return 0;

	int color, x;
	char *reg, *regR = get_reg_str(REG_R), *regY = get_reg_str(REG_Y), *regT = get_reg_str(REG_T);
	char *first = strtok(line, " ");
	char *second = strtok(NULL, " ");
	char *last = strtok(NULL, " ");

	ERR_INSTR_IF(last || !first, line, line_num);

	if (str_is_label(first))
	{
		ERR_INSTR_IF(second, line, line_num);
		
		//Color already part of label in chs
		fprintf(out, "%s\n", first);
	}
	else if (!strcmp(first, "X"))
	{
		ERR_INSTR_IF(!second, line, line_num);
		ERR_COLOR_IF(second, line_num);

		reg = get_reg_str(color);
		fprintf(out, "\txor %s, %s\n", reg, reg);
	}
	else if (!strcmp(first, "V"))
	{
		ERR_INSTR_IF(!second, line, line_num);

		x = parse_ball_num(second);
		ERR_NUM_IF(x < 0, second, line_num);

		fprintf(out, "\tadd %s, %d\n", regR, x);
		fprintf(out, "\tand %s, 0xFF\n", regR);
	}
	else if (!strcmp(first, "A"))
	{
		ERR_INSTR_IF(!second, line, line_num);
		
		x = parse_ball_num(second);
		ERR_NUM_IF(x < 0, second, line_num);

		fprintf(out, "\tsub %s, %d\n", regR, x);
		fprintf(out, "\tand %s, 0xFF\n", regR);
		//FIXME: Floor result to 0
	}
	else if (!strcmp(first, "<->"))
	{
		ERR_INSTR_IF(!second, line, line_num);
		ERR_COLOR_IF(second, line_num);

		//TODO: Decide: Keep optimization? It is part of spec
		if (color == REG_R)
			return 0;

		reg = get_reg_str(color);
		fprintf(out, "\tmov %s, %s\n", regT, reg);
		fprintf(out, "\tmov %s, %s\n", reg, regR);
		fprintf(out, "\tmov %s, %s\n", regR, regR);
	}
	else if (!strcmp(first, "<=>"))
	{
		ERR_INSTR_IF(!second, line, line_num);
		ERR_COLOR_IF(second, line_num);
		
		reg = get_reg_str(color);
		fprintf(out, "\tmov %s, [chs_mem + %s]\n", regT, regY);
		fprintf(out, "\tmov [chs_mem + %s], %s\n", regY, reg);
		fprintf(out, "\tmov %s, %s\n", reg, regT);
	}
	else if (!strcmp(first, "A>V"))
	{
		ERR_INSTR_IF(!second, line, line_num);
		ERR_COLOR_IF(second, line_num);

		reg = get_reg_str(color);
		fprintf(out, "\tadd %s, %s\n", regR, reg);
		fprintf(out, "\tand %s, 0xFF\n", regR);
		fprintf(out, "\txor %s, %s\n", reg, reg);
	}
	else if (!strcmp(first, "="))
	{
		ERR_INSTR_IF(!second, line, line_num);
		ERR_COLOR_IF(second, line_num);

		reg = get_reg_str(color);
		fprintf(out, "\tmov %s, %s\n", reg, regR);
	}
	else if (!strcmp(first, "mama"))
	{
		ERR_INSTR_IF(second, line, line_num);
		
		fprintf(out, "\tcall chs_in\n");
	}
	else if (!strcmp(first, "papa"))
	{
		ERR_INSTR_IF(second, line, line_num);
		
		fprintf(out, "\tcall chs_out\n");
	}
	else if (!strcmp(first, "O?"))
	{
		ERR_INSTR_IF(!second, line, line_num);
		ERR_LABEL_NVAL_IF(second, line_num);

		fprintf(out, "\tcmp %s, 0\n", regR);
		fprintf(out, "\tje %s\n", second); //Here in chs there is no colon after label
	}
	else
	{
		printf(ERR_INSTRUCTION, line, line_num);
		return 1;
	}

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
		if (parse_reg(ch) == REG_NVAL)
			return 0;
	return 1;
}
int parse_reg(char ch)
{
	if (ch == 'R') return REG_R;
	if (ch == 'G') return REG_G;
	if (ch == 'B') return REG_B;
	if (ch == 'Y') return REG_Y;
	return REG_NVAL;
}
int parse_reg_str(char *str)
{
	if (strlen(str) != 1) return REG_NVAL;
	return parse_reg(*str);
}
int parse_ball_num(char *str)
{
	int count = 0;

	while (*(str++) == 'O') count++;

	return *str ? -1 : count;
}
char *get_reg_str(int color)
{
	switch (color)
	{
		case REG_R:
			return "r11";
		case REG_G:
			return "r12";
		case REG_B:
			return "r13";
		case REG_Y:
			return "r14";
		case REG_T:
			return "r15";
		default:
			return NULL;
	}
}
