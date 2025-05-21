#include "mySimpleComputer.h"
#include "myBigChars.h"
#include "myTerm.h"
#include "myReadkey.h"
#include <stdio.h>
#include <string.h>

char INOUT[5][15] = {"", "", "", "", ""};
int nowRedact = 0;

void printCell(int address, enum colors fg, enum colors bg) {
  mt_setfgcolor(fg);
  mt_setbgcolor(bg);

  //FIX ME: перенести декодирование в sc_memorySet() ?
  int value;
  if (sc_memoryGet(address, &value) == 0) {
    int sign = 0;
    int command = 0;
    int opperand = 0;

    sc_commandDecode(value, &sign, &command, &opperand);
        if (sign == 0) {
        putchar('+');
    } else if (sign == 1) {
        putchar('-');
    }
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

void printCellFormat(void) {
  int value;
  sc_memoryGet(nowRedact, &value);
  mt_gotoXY(18, 3);
  mt_setfgcolor(RED);
  printf("dec: %05d oct: %05o hex: %04x bin: ", value, value, value);
  for (int i = 15; i >= 0; i--) {
      printf("%d", (value >> i) & 1);
      if (i % 4 == 0 && i != 0) 
          printf(" ");
  }
  printf("\n");
  // fflush(stdout);
}

void printAccumulator(void) {

  // FIX ME: negative numbers, +, -, format %04X ?
  int sign = 0, command = 0, operand = 0;
  if (sc_commandDecode(accumulator, &sign, &command, &operand) == 0) {
    mt_gotoXY(2, 65);
    printf("sc: %c%02X%02X\t", (sign == 0 ? '+' : '-'), command, operand);
  } else {
    mt_gotoXY(2, 65);
    printf("sc: Error\t");
  }

  mt_gotoXY(2, 65);
  printf("hex: %04X", (accumulator & 0x7FFF));
  fflush(stdout);
}


void printCounters(void) {
  mt_gotoXY(5, 65);
  // FIX ME: negative numbers, +, -, format %04X ?
  printf("IC: %04X\t T: %d", instructCounter, instructTact);
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
    int operand;  // исправлено опечатку в названии переменной (opperand -> operand)
    sc_icounterGet(&value);
    sc_commandDecode(value, &sign, &command, &operand);
    
    mt_gotoXY(5, 105);
    
    if (operand > 128) {
        printf("! + FF : FF");
        fflush(stdout);
        return;
    }
    
    char buffer[50];
    snprintf(
        buffer,
        sizeof(buffer),
        "%c %02X : %02X",
        (sign == 0) ? '+' : '-',
        command,
        operand);
    
    printf("%s", buffer);
    fflush(stdout);
}

void printDecodedCommand(int value) {
  mt_gotoXY(5, 105);

  printf("Hex: %04X Bin: ", value);
  for (int i = 15; i >= 0; i--) {
    printf("%d", (value >> i) & 1);
  }
  printf("\n");
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
      bc_printbigchar(16, 9, 65, BLUE, BLACK); 
    }

    // int encoded = (command << 8) | opperand;
    
    int command1, command2;
    int opperand1, opperand2;
    command1 = command >> 4;
    command2 = command & 0xF;
    opperand1 = (opperand >> 4) & 0xF;
    opperand2 = opperand & 0xF;
    bc_printbigchar(command1, 9, 76, BLUE, BLACK);
    bc_printbigchar(command2, 9, 86, BLUE, BLACK);
    bc_printbigchar(opperand1, 9, 96, BLUE, BLACK);
    bc_printbigchar(opperand2, 9, 106, BLUE, BLACK);
    mt_gotoXY(18, 68);
    mt_setfgcolor(BLUE);
    mt_setbgcolor(BLACK);
    printf("Номер редактируемой ячейки: %0*u", 4, nowRedact);
    fflush(stdout);
}

void printCursor(int cursor, enum colors cursor_color) {

  int row = cursor / 10;  // строка от 0 до 9
  int col = cursor % 10;  // столбец от 0 до 9

  int x = 2 + col * 6;  // сдвиг по X: каждый блок шириной 6
  int y = 2 + row;      // сдвиг по Y: каждая строка по 1

  mt_gotoXY(y, x);
  mt_setbgcolor(cursor_color);
  mt_setfgcolor(WHITE);

  int value;
  sc_memoryGet(cursor, &value);
  int sign, command, operand;
  sc_commandDecode(value, &sign, &command, &operand);

  char buffer[18];
  if (sign == 0)
    snprintf(buffer, sizeof(buffer), "+%02X%02X ", command, operand);
  else
    snprintf(buffer, sizeof(buffer), "-%02X%02X ", command, operand);

  write(1, buffer, strlen(buffer));
  fflush(stdout);

  mt_setdefaultcolor();

}

void editCell(int nowCell) {

  int row = nowCell / 10;  // строка от 0 до 9
  int col = nowCell % 10;  // столбец от 0 до 9

  int x = 2 + col * 6;  // сдвиг по X: каждый блок шириной 6
  int y = 2 + row;      // сдвиг по Y: каждая строка по 1

  mt_gotoXY(y, x); //x-6?

  int input;
  int sign, command, operand;
  if (rk_readvalue(&input, 5) == 0) {
      sc_memorySet(nowCell, input);
  } else {
      sc_memorySet(nowCell, 0);
  }
}

void printAllMemory(void) {
  mt_gotoXY(2, 2);
  for (int i = 0; i != 13; i++) {
      mt_gotoXY(2+i, 2);
      for (int j = 0; j < 10; j++) {
          printCell(i * 10 + j, WHITE, BLACK);
      }
      printf("\n");
  }
  printf("\n");
  mt_setdefaultcolor();
}

void printblack(int row, int col, char *text) {
  mt_gotoXY(row, col);
  mt_setbgcolor(BLACK);
  for (int i = 0; i< 35; i++)
    printf(" ");
  printf("\n");
  mt_gotoXY(row+1, col);
  for (int i = 0; i< 35; i++)
    printf(" ");
  fflush(stdout);

  mt_gotoXY(row, col);
  printf("%s", text);
  // write(1, text, strlen(text));
  mt_setdefaultcolor();
  // printf("\n");
  fflush(stdout);
}

void printbl(int row, int col) {
  mt_gotoXY(row, col);
  mt_setbgcolor(BLACK);
  for (int i = 0; i< 35; i++)
    printf(" ");
  printf("\n");
  fflush(stdout);
  mt_gotoXY(row+1, col);
  for (int i = 0; i< 35; i++)
    printf(" ");
  fflush(stdout);
}