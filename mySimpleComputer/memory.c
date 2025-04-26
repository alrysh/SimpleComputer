#include "mySimpleComputer.h"
#include <stdio.h>

int memory[MEMORY_SIZE];

int sc_memoryInit() {
  for (int i = 0; i < MEMORY_SIZE; i++) {
    memory[i] = 0;
  }
  return 0;
}

int sc_memorySet(int address, int value) {
  if (address >= 0 && address < MEMORY_SIZE && value >= VAL_MIN &&
      value <= VAL_MAX) {
    memory[address] = value;
  } else {
    // printf("Ошибка: Выход за границы memory (адрес %d, значение %d)\n",
    // address, value);
    return -1;
  }
  return 0;
}

int sc_memoryGet(int address, int *value) {
  if (address >= 0 && address < MEMORY_SIZE && value != NULL) {
    *value = memory[address];
    return 0;
  } else {
    // printf("Ошибка: Неверный адрес чтения (%d)!\n", address);
    return -1;
  }
}

int sc_memorySave(const char *filename) {
  if (filename == NULL) {
    return -1;
  }

  FILE *file = fopen(filename, "wb"); //имя файла
  if (file == NULL) {
    return -1;
  }

  size_t written = fwrite(memory, sizeof(int), MEMORY_SIZE, file);
  fclose(file);

  if (written != MEMORY_SIZE) {
    return -1;
  }

  return 0;
}

int sc_memoryLoad(char *filename) {
  if (filename == NULL) {
    return -1;
  }

  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    return -1;
  }

  size_t count_read = fread(memory, sizeof(int), MEMORY_SIZE, file);
  fclose(file);

  if (count_read != MEMORY_SIZE) {
    return -1;
  }

  return 0;
}
