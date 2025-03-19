#include "mySimpleComputer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  // 1: инициализация
  sc_memoryInit();
  sc_accumulatorInit();
  sc_icounterInit();
  sc_regInit();

  printf("1: Инициализация памяти, аккумулятора, счётчика команд и флагов.\n");

  // 2: заполнение памяти случайными значениями и вывод её содержимого

  printf("\n2: Заполнение памяти случайными значениями:\n");

  srand(time(NULL));
  int rand_memory = rand() % 127;

  for (int i = 0; i < rand_memory; i++) {
    int value = rand() % (VAL_MAX + 1);
    sc_memorySet(i, value);
  }

  for (int i = 0; i < rand_memory; i++) {
    int value;
    sc_memoryGet(i, &value);
    printf("%d ", value);
  }
  printf("\n");

  // 3: установка недопустимого значения в память
  int stat = sc_memorySet(170, 77777);
  printf("\n3: Попытка записать недопустимое значение в память: %s\n",
         stat == -1 ? "Проверка пройдена" : "Проверка не пройдена");

  // 4: установка флагов и их вывод
  sc_regSet(FLAG_OVERFLOW, 1);
  sc_regSet(FLAG_ZERO_DIV, 1);
  sc_regSet(FLAG_INVALID_CMD, 1);
  printf("\n4: Установлены флаги переполнения, деления на 0, неправильной "
         "команды\n");
  printFlags();

  // 5: установка некорректного флага
  stat = sc_regSet(0x20, 1);
  printf("\n5: Установка некорректного флага: %s\n",
         stat == -1 ? "Проверка пройдена" : "Проверка не пройдена");

  // 6 значение аккумулятора
  sc_accumulatorSet(777);
  printf("\n6: Установлено значение аккумулятора: ");
  printAccumulator();

  stat = sc_accumulatorSet(40000);
  printf("\n7: Установка недопустимого значения аккумулятора: %s\n",
         stat == -1 ? "Проверка пройдена" : "Проверка не пройдена");

  // 8 работа со счетчиком команд
  sc_icounterSet(10);
  printf("\n8: Установлено значение счетчика команд: ");
  printCounters();

  int invalid_acum = sc_icounterSet(40000);
  printf("\n9: Установка недопустимого значения счетчика команд: %s\n",
         invalid_acum == -1 ? "Проверка пройдена" : "Проверка не пройдена");
  // printf("Значение акум: %d\n", sc_icounterGet(&acum));

  // 10 декодирование значения из памяти
  int command, operand, sign;
  sc_commandDecode(1234, &sign, &command, &operand);
  // 000 0100 1101 0010
  printf("\n10: декодирование value: знак = %d, команда = %d, операнд = %d\n",
         sign, command, operand);

  // 11 кодирование команды
  int encodedValue;
  int sign_uncoded = 0;
  int command_uncoded = 11;
  int operand_uncoded = 5;
  sc_commandEncode(sign_uncoded, command_uncoded, operand_uncoded,
                   &encodedValue);
  printf(
      "\n11: кодирование команды (знак = %d, команда: WRITE, операнд = %d):\n",
      sign_uncoded, operand_uncoded);
  printDecodedCommand(encodedValue);

  return 0;
}
