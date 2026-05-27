#include "mySimpleComputer.h"
#include "myTerm.h"
#include <stdio.h>

char INOUT[5][15] = {"", "", "", "", ""};

void printCell(int address, enum colors fg, enum colors bg) {
  mt_setfgcolor(fg);
  mt_setbgcolor(bg);

  int value;
  sc_memoryGet(address, &value);
  int sign = 0;
  int command = 0;
  int opperand = 0;
  if (sign == 0) {
      putchar('+');
  } else {
      putchar('-');
  }
  sc_commandDecode(value, &sign, &command, &opperand);
  printf("%0*X%0*X ", 2, command, 2, opperand);
  fflush(stdout);
  mt_setdefaultcolor();
}

void printFlags(void) {
  mt_gotoXY(1, 85);

  printf("Flags: %c %c %c %c %c\n", (flags & FLAG_OVERFLOW) ? 'P' : '_',
         (flags & FLAG_ZERO_DIV) ? '0' : '_',
         (flags & FLAG_MEMORY_OUT) ? 'M' : '_',
         (flags & FLAG_CLOCK_IGNORE) ? 'T' : '_',
         (flags & FLAG_INVALID_CMD) ? 'E' : '_');
         fflush(stdout);
}

void printDecodedCommand(int value) {
  mt_gotoXY(3,62);

  printf("Dec: %d, Oct: %o, Hex: %X, Bin: ", value, value, value);
  for (int i = 15; i >= 0; i--) {
    printf("%d", (value >> i) & 1);
  }
  printf("\n");
}

void printAccumulator(void) {
  mt_gotoXY(1, 62);
  printf("Accumulator: %X\n", accumulator);
  fflush(stdout);
}

void printCounters(void) {
  mt_gotoXY(2, 62);
  printf("Instruction Counter: %d\n", instructCounter);
  fflush(stdout);
}

void printTerm(int address, int input)
{
  int value;
  sc_memoryGet(address, &value);
  int sign = 0;
  int command = 0;
  int opperand = 0;
  if (sign == 0) {
      putchar('+');
  } else {
      putchar('-');
  }
  sc_commandDecode(value, &sign, &command, &opperand);
  printf("%0*X%0*X ", 2, command, 2, opperand);
  fflush(stdout);
  mt_setdefaultcolor();

  fflush(stdout);
}