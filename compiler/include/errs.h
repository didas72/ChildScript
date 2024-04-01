// errs.h - 

#pragma once

#define ERR_SRC_FILE "Could not find source file '%s'.\n"
#define ERR_INSTRUCTION "Invalid instruction line '%s'. (line %ld)\n"
#define ERR_COLOR "Invalid color '%s'. (line %ld)\n"
#define ERR_NUM "Invalid number representation '%s'. (line %ld)\n"
#define ERR_LABEL_NVAL "Invalid label name '%s'. (line %ld)\n"

#define ERR_INSTR_IF(condition, line, line_num) do { if (condition) { printf(ERR_INSTRUCTION, line, line_num); return 1; } } while(0)
#define ERR_COLOR_IF(str, line_num) do { if ((color = parse_reg_str(str)) == REG_NVAL) { printf(ERR_COLOR, str, line_num); return 1; } } while (0)
#define ERR_NUM_IF(condition, num_str, line_num) do { if (condition) { printf(ERR_NUM, num_str, line_num); return 1; } } while(0)
#define ERR_LABEL_NVAL_IF(str, line_num) do { if (!str_is_label(str)) { printf(ERR_LABEL_NVAL, str, line_num); return 1;} } while (0)
