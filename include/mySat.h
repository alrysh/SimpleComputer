#ifndef MYSAT_H
#define MYSAT_H

#include <stdio.h>
#include <stdlib.h>
#include "mySat.h"

int command_to_code(const char *name);
int parse_line(char *line, int *address, int *value);
int assemble(const char *src_path, const char *out_path);
int run_assembler_prompt();

#endif