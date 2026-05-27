#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myBigChars.h"
#include "myReadkey.h"
#include "mySat.h"

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/time.h>


//перенести в memory.c
void fillMemory()
{
    for (int i = 0; i < MEMORY_SIZE; i++) {
        // sc_memorySet(i, i+1);
        sc_memorySet(i, 0);
    }

    // sc_memorySet(1, 128); //0x0100 - 128
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
    sc_icounterInit(); 
    sc_accumulatorSet(1);
    sc_icounterSet(5);
    sc_regSet(FLAG_CLOCK_IGNORE, 1);
    cache_init();

    fillMemory();

    char* str= "Оперативная память";
    bc_box(1, 1, 13, 62, YELLOW, BLACK, str, YELLOW, BLACK);

    sc_memoryLoad("cache.bin");
    // sc_memoryLoad("command.bin");
    // sc_memoryLoad("fact.bin");
    printAllMemory();
    

    bc_box(16, 1, 2, 62, RED, BLACK, "Редактируемая ячейка (формат)", RED, BLACK);
    bc_box(1, 63, 1, 40, RED, BLACK, "Аккумулятор", RED, BLACK);
    bc_box(1, 103, 1, 40,  RED, BLACK, "Регистр флагов", RED, BLACK);
    bc_box(4, 63, 1, 40, RED, BLACK, "Счетчик команд", RED, BLACK);
    bc_box(4, 103, 1, 40, RED, BLACK, "Команда", RED, BLACK);

    bc_box(7, 63, 11, 80, BLUE, BLACK, "Редактируемая команда (увеличено)", BLUE, BLACK);

    bc_box(20, 1, 5, 62, WHITE, BLACK, "Кэш процессора", BLACK, WHITE);
    bc_box(20, 63, 5, 20, WHITE, BLACK, "IN-OUT", BLACK, WHITE);
    bc_box(20, 83, 5, 60, GREEN, BLACK, "Клавиши", GREEN, BLACK);
    mt_setdefaultcolor();


    mt_setbgcolor(BLACK);
    // printCache();
    //БЛОК КОМАНДА:
    printCommand();
    printAccumulator();
    printCounters();
    printFlags();
    
    
    // mt_gotoXY(20, 65);
    // for (int i = 0; i <= 4; i++) {
    //     mt_gotoXY(21+i, 65);
    //     mt_setbgcolor(BLACK);
    //     printTerm(10+i, 1);
    // }

    mt_gotoXY(21, 85);
    printf("ESC - exit\n");
    mt_gotoXY(22, 85);
    printf("F5 - accumulator\n");
    mt_gotoXY(23, 85);
    printf("F6 - instruction counter\n");
    mt_gotoXY(24, 85);
    printf("l - load, s - save, i - reset");
    fflush(stdout);

    run_interactive_mode();

    mt_gotoXY(90, 1);

    return 0;
    // return EXIT_SUCCESS;
}
