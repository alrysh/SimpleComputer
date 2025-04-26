#include "mySimpleComputer.h"
#include <stdio.h>

// #define ACCUMULATOR_MASK 0x7FFF

// #define FLAG_OVERFLOW      0x01  // Переполнение
// #define FLAG_ZERO_DIV      0x02  // Деление на 0
// #define FLAG_MEMORY_OUT    0x04  // Выход за границы памяти
// #define FLAG_CLOCK_IGNORE  0x08  // Игнорирование тактовых импульсов
// #define FLAG_INVALID_CMD   0x10  // Неверная команда

int accumulator = 0;
int instructCounter = 0;
int flags = 0;

int sc_regInit(void) {
  flags = 0;
  return 0;
}

int sc_regSet(int reg, int value) {
  if (reg != FLAG_OVERFLOW && reg != FLAG_ZERO_DIV && reg != FLAG_MEMORY_OUT &&
      reg != FLAG_CLOCK_IGNORE && reg != FLAG_INVALID_CMD) {
    return -1;
  }
  if (value)
    flags |= reg;
  else
    flags &= ~reg;

  return 0;
}

// int sc_regGet (int register, int * value) – возвра-
// щает значение указанного флага.

int sc_regGet(int reg, int *value) {
  if (reg != FLAG_OVERFLOW || reg != FLAG_ZERO_DIV || reg != FLAG_MEMORY_OUT ||
      reg != FLAG_CLOCK_IGNORE || reg != FLAG_INVALID_CMD || value == NULL) {
    return -1;
  }

  *value = (flags & reg) ? 1 : 0;

  return 0;
}

// int sc_accumulatorInit (void) – инициализирует аккуму-
// лятор значением по умолчанию;

int sc_accumulatorInit(void) {
  accumulator = 0;
  return 0;
}

// int sc_accumulatorSet (int value) – устанавливает зна-
// чение аккумулятора.

int sc_accumulatorSet(int value) {
  if (value >= VAL_MIN && value <= VAL_MAX) {
    accumulator = value;
  } else {
    return -1;
  }
  return 0;
}

// int sc_accumulatorGet (int * value) – возвращает зна-
// чение аккумулятора. Если передан неверный указатель на значение,
// то функция завершается со статусом -1.

int sc_accumulatorGet(int *value) {
  if (value != NULL) {
    *value = accumulator;
    return 0;
  } else {
    return -1;
  }
}

int sc_icounterInit(void) {
  instructCounter = 0;
  return 0;
}

int sc_icounterSet(int value) {
  if (value >= VAL_MIN && value <= VAL_MAX) {
    instructCounter = value;
    return 0;
  } else {
    return -1;
  }
}

int sc_icounterGet(int *value) {
  if (value != NULL) {
    *value = instructCounter;
    return 0;
  } else {
    return -1;
  }
}
