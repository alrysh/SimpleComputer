#include "mySimpleComputer.h"
#include "myTerm.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

void fillMemory()
{
    for (int i = 0; i < MEMORY_SIZE; i++) {
        sc_memorySet(i, i+50);
    }
}

int main() {
    if (!isatty(fileno(stdout))) {
        fprintf(stderr, "Ошибка: вывод не является терминалом.\n");
        exit(EXIT_FAILURE);
    }
    printf("Вывод в терминал подтвержден\n");

    struct winsize WS;
    if (ioctl(1, TIOCGWINSZ, &WS)) {
        printf("Не удалось получить размер терминала\n");
        return 2;
    }

    if (WS.ws_col < 150 && WS.ws_row < 150) {
        printf("Малые размеры терминала");
        return 3;
    }
    printf("Размер терминала совпадает\n");

    mt_clrscr();
    sc_memoryInit();
    sc_regInit();
    sc_accumulatorInit();
    sc_icounterInit(); ///
    sc_regSet(FLAG_OVERFLOW, 1);
    sc_regSet(FLAG_MEMORY_OUT, 1);
    sc_accumulatorSet(4691);
    sc_icounterSet(25384); ///
    // mt_clrscr();

    fillMemory();

    // for (int i = 0; i != 128; i++) {
    //     printCell(i, WHITE, BLACK);
    // }

    for (int i = 0; i != 12; i++) {
        for (int j = 0; j < 10; j++) {
            printCell(i * 10 + j, WHITE, BLACK);
        }
        printf("\n");
    }
    printf("\n");
    mt_setdefaultcolor();

    int value;
    sc_memoryGet(0, &value);
    printDecodedCommand(accumulator);

    printAccumulator();
    printCounters();
    printFlags();

    mt_gotoXY(90,1);

    for (int i = 0; i < 7; i++) {
        printCell(i, WHITE, BLACK);
    }
    printf("\n");

    // mt_setcolor(BLUE, WHITE);
    // write(1, "Colored Text", 12);
    // mt_setdefaultcolor();
    // printf("\n");

    // mt_setfgcolor(PURPLE);
    // printf("Colored text\n");
    // mt_setbgcolor(RED);

    // write(1, "Colored Text", 12); //!
    // mt_setdefaultcolor();
    // printf("\n");
    // printf("Colored text\n");

    return 0;
}
