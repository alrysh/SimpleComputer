#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myBigChars.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

void fillMemory()
{
    for (int i = 0; i < MEMORY_SIZE; i++) {
        sc_memorySet(i, i+1);
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

    // int font;

    // if (argc > 2) {
    //     printf("Много аргументов");
    //     return -1;
    // } else if (argc == 2) {
    //     font = open(argv[1], O_RDONLY);
    // } else {
    //     font = open("src/font.bin", O_RDONLY);
    // }
    // if (font == -1) {
    //     printf("Не удается открыть файл шрифта");
    //     return -1;
    // }

    // int count;
    // if (bc_bigcharread(font, bigchar[0], 18, &count)) {
    //     printf("Не удается прочесть файл шрифта");
    //     return -1;
    // }

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

    char* str= "Оперативная память";
    bc_box(1, 1, 13, 62, YELLOW, BLACK, str, YELLOW, BLACK);   

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

    int value;
    sc_memoryGet(0, &value);
    mt_setbgcolor(BLACK);

    printDecodedCommand(accumulator);
    printAccumulator();
    printCounters();
    printFlags();

    
    mt_gotoXY(20, 65);
    for (int i = 0; i <= 4; i++) {
        mt_gotoXY(21+i, 65);
        mt_setbgcolor(BLACK);
        printTerm(10+i, 1);
    }

    nowRedact = 0;
    mt_setfgcolor(BLUE);
    mt_gotoXY(10, 65);
    printBigCell();
    // mt_setdefaultcolor();

    // mt_gotoXY(90, 1);
 
    // int big_A[2] = {0};
    // init_big_A(big_A);   
    
    // bc_printbigchar(big_A, 9, 65, WHITE, BLACK);
    // mt_gotoXY(10, 65);
    // mt_setbgcolor(BLACK);
    // char *big_char = "+0123ABC"; 
    // bc_bigstring(big_char, 9, 65);

    // bc_printbigchar0(17, 10, 65);

    mt_gotoXY(90, 1);
    // mt_gotoXY(100, 1);

    return 0;
}
