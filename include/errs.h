//errs.h - Error messages

#pragma once

#define ERR_INSTRUCTION "Joe didn't understand the provided instruction. Are you using the right arguments?\n"
#define ERR_COLOR "Joe didn't understand the provided color. You can only use capital letters.\n"
#define ERR_NUM "Joe didn't understand the provided number. Are you sure you didn't include any *funny* characters?\n"
#define ERR_LABEL_NVAL "Joe didn't understand the provided color lablel. They can only contain CAPITAL colors.\n"
#define ERR_LABEL_NFOUND "Joe couldn't find the provided color label. Make sure it exists and is typed correctly.\n"

#define ERR_INSTR_IF(condition) do { if (condition) { printf(ERR_INSTRUCTION); return 1; } } while(0)
#define ERR_COLOR_IF(str) do { if ((color = parse_color_str(str)) == COLOR_NVAL) { printf(ERR_COLOR); return 1; } } while (0)
#define ERR_NUM_IF(condition) do { if (condition) { printf(ERR_NUM); return 1; } } while(0)
#define ERR_LABEL_NVAL_IF(str) do { if (!str_is_label(str)) { printf(ERR_LABEL_NVAL); return 1;} } while (0)
