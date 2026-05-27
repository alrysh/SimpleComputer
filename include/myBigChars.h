#ifndef MYBIGCHARS_H
#define MYBIGCHARS_H

#include <stdio.h>
#include <stdlib.h>
#include "myTerm.h"

extern int bigchar[18][2];

int bc_strlen(const char *str);
int bc_printA (char * str);

int bc_box(int x1, int y1, int x2, int y2,
    enum colors box_fg, enum colors box_bg, char *header, 
    enum colors header_fg, enum colors header_bg);

int bc_setbigcharpos(int *big, int x, int y, int value);
int bc_getbigcharpos(int *big, int x, int y, int *value);
// int bc_printbigchar(int big[2], int x, int y, enum colors fg, enum colors bg);
int bc_bigcharwrite(int fd, int* big, int count);
int bc_bigcharread(int fd, int* big, int need_count, int* count);

void bc_print_char_line(int part);
void bc_printbigchar(int index, int x, int y, enum colors fg, enum colors bg);
#endif