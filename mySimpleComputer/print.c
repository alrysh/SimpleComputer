#include "mySimpleComputer.h"'
#include "myBigChars.h"
#include "myTerm.h"
#include <stdio.h>

char INOUT[5][15] = {"", "", "", "", ""};
int nowRedact = 0;

void printCell(int address, enum colors fg, enum colors bg) {
  mt_setfgcolor(fg);
  mt_setbgcolor(bg);

  int value;
  if (sc_memoryGet(address, &value) == 0) {
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
}

void printFlags(void) {
  mt_gotoXY(2, 105);

  printf("Flags: %c %c %c %c %c\n", (flags & FLAG_OVERFLOW) ? 'P' : '_',
         (flags & FLAG_ZERO_DIV) ? '0' : '_',
         (flags & FLAG_MEMORY_OUT) ? 'M' : '_',
         (flags & FLAG_CLOCK_IGNORE) ? 'T' : '_',
         (flags & FLAG_INVALID_CMD) ? 'E' : '_');
         fflush(stdout);
}

void printDecodedCommand(int value) {
  mt_gotoXY(5, 105);

  printf("Hex: %X Bin: ", value);
  for (int i = 15; i >= 0; i--) {
    printf("%d", (value >> i) & 1);
  }
  printf("\n");
}

void printAccumulator(void) {
  mt_gotoXY(2, 65);
  printf("Dec: %d Oct: %o Hex: %X", accumulator, accumulator, accumulator);
  fflush(stdout);
}

void printCounters(void) {
  mt_gotoXY(5, 65);
  printf("Dec: %d Oct: %o Hex: %X", instructCounter, instructCounter, instructCounter);
  fflush(stdout);
}

void print_command_operand(int command, int operand) {
  char buffer[16]; // достаточно места для "XXXX " + '\0'
  int len = snprintf(buffer, sizeof(buffer), "%02X%02X ", command, operand);
  write(1, buffer, len);
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
  // print_command_operand(command, opperand);
  fflush(stdout);
  mt_setdefaultcolor();
}

void printCommand()
{
    int value;
    int sign;
    int command;
    int opperand;
    sc_icounterGet(&value);
    sc_commandDecode(value, &sign, &command, &opperand);
    mt_gotoXY(91, 5);
    if (opperand > 128) {
        printf("! + FF : FF");
        fflush(stdout);
        return;
    }
    char buffer[50];
    snprintf(
            buffer,
            sizeof(buffer),
            "%c %0*X : %0*X",
            (sign == 0) ? '+' : '-',
            2,
            command,
            2,
            opperand);
    printf("%s", buffer);
    fflush(stdout);
}

void printBigCell(void)
{
    if (nowRedact < 0 || nowRedact > 127) {
        return;
    }

    int value;
    sc_memoryGet(nowRedact, &value); // номер редактируемой ячейки
    int sign;
    int command;
    int opperand;
    sc_commandDecode(value, &sign, &command, &opperand);
    if (sign == 0) {
      bc_printbigchar(17, 9, 65, BLUE, BLACK);
    } 
    else {
      bc_printbigchar(bigchar[16], 9, 65, BLUE, BLACK); 
    }

    int encoded = (command << 8) | opperand;

    
    int command1, command2;
    int opperand1, opperand2;
    command1 = command >> 4;
    command2 = command & 0xF;
    opperand1 = (opperand >> 4) & 0xF;
    opperand2 = opperand & 0xF;
    bc_printbigchar(command1, 9, 74, BLUE, BLACK);
    bc_printbigchar(command2, 9, 83, BLUE, BLACK);
    bc_printbigchar(opperand1, 9, 91, BLUE, BLACK);
    bc_printbigchar(opperand2, 9, 100, BLUE, BLACK);
    mt_gotoXY(18, 68);
    mt_setfgcolor(BLUE);
    mt_setbgcolor(BLACK);
    printf("Номер редактируемой ячейки: %0*u", 4, nowRedact);
    fflush(stdout);
}