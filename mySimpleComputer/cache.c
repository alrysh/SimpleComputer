#include "mySimpleComputer.h"
#include <stdio.h>

#define CACHE_LINES 5 // в "mySimpleComputer.h"
#define CACHE_COLUMNS 10

typedef struct {
    int data[CACHE_COLUMNS];  // 10 ячеек
    int tag;                    // Номер строки в памяти
    int valid;                  // 1 — данные актуальны, 0 — мусор
    int dirty;                  // Флаг изменения строки
    int last_used;             // Счетчик для LRU
} CacheLine;

CacheLine cache[CACHE_LINES];
int global_usage_counter = 0;

void cache_init() {
    for (int i = 0; i < CACHE_LINES; i++) {
        cache[i].valid = 0;
        cache[i].dirty = 0;
        cache[i].last_used = 0;
    }

    // mt_gotoXY(29, 90);
    // printf("valid: %d, tag: %d", cache[0].valid, cache[0].tag);
    // fflush(stdout);
}

int find_lru_line() {
    int min_index = 0;
    for (int i = 1; i < CACHE_LINES; i++) {
        if (!cache[i].valid) return i;
        if (cache[i].last_used < cache[min_index].last_used)
            min_index = i;
    }
    return min_index;
}

int cache_read(int address, int *value) {
    int tag = address / CACHE_COLUMNS;
    int offset = address % CACHE_COLUMNS;


    for (int i = 0; i < CACHE_LINES; i++) {

        if (cache[i].valid && cache[i].tag == tag) {
            *value = cache[i].data[offset];
            cache[i].last_used = ++global_usage_counter;

            // mt_gotoXY(28, 90);
            // printf("valid: %d, tag: %d", cache[i].valid, cache[i].tag);
            // fflush(stdout);
            // instructTact = 0;
            // printCounters();
            printCache();
            return 1; // cache hit
        }
    }

    // cache miss
    instructTact = 10;
    load_cache_line(address);
    printCounters();
    printCache();
    return cache_read(address, value);  // рекурсивно читаем после загрузки
}

int cache_write(int address, int value) {
    if (address >= 0 && address < MEMORY_SIZE && value >= VAL_MIN &&
    value <= VAL_MAX) {
    memory[address] = value;
    } else {
        return -1;
    }

    int tag = address / CACHE_COLUMNS;
    int offset = address % CACHE_COLUMNS;

    for (int i = 0; i < CACHE_LINES; ++i) {
        if (cache[i].valid && cache[i].tag == tag) {
            cache[i].data[offset] = value;
            cache[i].dirty = 1;
            cache[i].last_used = ++global_usage_counter;

            // instructTact = 0;
            // printCounters();
            printCache();
            return 1; // cache hit
        }
    }

    // cache miss
    instructTact = 10;
    load_cache_line(address);
    printCounters();
    printCache();
    return cache_write(address, value); // повторная попытка записи
}


void load_cache_line(int address) {
    int tag = address / CACHE_COLUMNS;
    int offset = address % CACHE_COLUMNS;

    // printblack(27, 90, "cache line is load");

    int lru_index = find_lru_line();  // выбираем строку кэша для замены

    // если строка dirty, сбрасываем в память
    if (cache[lru_index].valid && cache[lru_index].dirty) {
        int old_tag = cache[lru_index].tag;
        for (int i = 0; i < CACHE_COLUMNS; ++i)
            memory[old_tag * CACHE_COLUMNS + i] = cache[lru_index].data[i];
    }

    // загружаем новую строку из памяти
    for (int i = 0; i < CACHE_COLUMNS; i++)
        cache[lru_index].data[i] = memory[tag * CACHE_COLUMNS + i];

    cache[lru_index].tag = tag;
    cache[lru_index].valid = 1;
    cache[lru_index].dirty = 0;
    cache[lru_index].last_used = ++global_usage_counter;
}

// void memeory_contr() {
//     if (instructTact = 10) {

//     }
// }

void printCache() {
    for (int i = 0; i < CACHE_LINES; i++) {

        CacheLine *line = &cache[i]; 
        if (cache[i].valid == 1) {

            mt_gotoXY(21 + i, 2);
            for (int j = 0; j < CACHE_COLUMNS; j++) {
                int value = line->data[j];
                int sign = 0, command = 0, operand = 0;

                if (line->dirty) mt_setfgcolor(RED);
                // else if (!line->valid) mt_setfgcolor(GREEN);

                // декодир значения
                if (sc_commandDecode(value, &sign, &command, &operand) == 0) {
                    putchar(sign ? '-' : '+');
                    printf("%02X%02X ", command, operand);
                } else {
                    printf(" ???? ");
                }
            }
        }
        
        mt_setdefaultcolor();
        printf("\n");
    }
}



