#ifndef MYSIMPLECOMPUTER_H
#define MYSIMPLECOMPUTER_H

#include <stdio.h>
#include <stdlib.h>
#include "myTerm.h"
#include "myBigChars.h"

#define MEMORY_SIZE 128

#define VAL_MIN 0
#define VAL_MAX 32767
//и положит и отриц: 2^15 = 32767 и 1 бит для знака
//только положит: 65535
//не менять, используется для аккумулятора и счетчика команд

extern int nowRedact;

extern int memory[MEMORY_SIZE];
extern int accumulator;
extern int instructCounter;
extern int flags;

#define ACCUMULATOR_MASK 0x7FFF // 32767

#define FLAG_OVERFLOW 0x01     // (P) 0001 *переполнение
#define FLAG_ZERO_DIV 0x02     // (0) 0010 *деление на 0
#define FLAG_MEMORY_OUT 0x04   // (M) 0100 *выход за границы
#define FLAG_CLOCK_IGNORE 0x08 // (T) 1000 *игнорирование такт
#define FLAG_INVALID_CMD 0x10  // (E) 0001 0000 *неверная команда

#define COMMAND_MASK 0x7F  // (7 бит) 0111 1111
#define OPCODE_MASK 0x7F80 // (7 бит) 0111 1111 1000 0000
#define SIGN_MASK 0x8000   // (1 бит) 1000 0000 0000 0000

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memorySave(const char *filename);
int sc_memoryLoad(char *filename);

int sc_regInit(void);
int sc_regSet(int reg, int value);
int sc_regGet(int reg, int *value);
int sc_accumulatorInit(void);
int sc_accumulatorSet(int value);
int sc_accumulatorGet(int *value);
int sc_icounterInit(void);
int sc_icounterSet(int value);
int sc_icounterGet(int *value);

//комманда
int sc_commandValidate(int command);
int sc_commandEncode(int sign, int command, int operand, int *value);
int sc_commandDecode(int value, int *sign, int *command, int *operand);

//ввод-вывод
void printCell(int address, enum colors fg, enum colors bg);
void printAccumulator(void);
void printFlags(void);
void printCounters(void);
void printDecodedCommand(int value);
void printTerm(int address, int input);

void printCommand();
void printBigCell(void);

#endif