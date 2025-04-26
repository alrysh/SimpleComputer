#include "myTerm.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <termios.h>
#include <unistd.h>
#include <string.h>

int mt_clrscr (void) {
    if (write(1, "\033[H\033[2J", 7) == -1) {
        return -1;
    }
    return -1;
    //clstr=tgetstr("cl",0);
}

// ➢ int mt_gotoXY (int, int) - перемещает курсор в указан-
// ную позицию. Первый параметр номер строки, второй - номер
// столбца;

int mt_gotoXY(int row, int col) {
    char buf[50];
    sprintf(buf, "\033[%d;%dH", row, col);
    unsigned int len = strlen(buf);

    if (write(1, buf, len) == -1) {
        return -1;
    }
    fflush(stdout);
    return 0;
}

// структура winsize уже определена в #include <sys/ioctl.h>:
// struct winsize {
//     unsigned short ws_row;
//     unsigned short ws_col;
//     unsigned short ws_xpixel;
//     unsigned short ws_ypixel;
// };

int mt_getscreensize(int *rows, int *cols) {
  struct winsize ws;
  if (!ioctl(1, TIOCGWINSZ, &ws)) { // 1 - дескриптор //-1
    *rows = ws.ws_row;
    *cols = ws.ws_col;

    return 0;
  } else {
    return -1;
  }
}

int mt_setfgcolor(enum colors color) {
    if (color >= BLACK && color <= WHITE) {
        char buf[50];
        sprintf(buf, "\033[3%dm", color);

        unsigned int len = strlen(buf);
        if (write(1, buf, len) == -1) {
            return -1;
        }
        return 0;
    } else {
        return -1;
    }
}

int mt_setbgcolor(enum colors color) {
    if (color >= BLACK && color <= WHITE) {
        char buf[50];
        sprintf(buf, "\033[4%dm", color);

        unsigned int len = strlen(buf);
        if (write(1, buf, len) == -1) {
            return -1;
        }
        return 0;
    } else {
        return -1;
    }
}

// ➢ int mt_setdefaultcolor (void) – устанавливает цвета
// символов и фона в значения по умолчанию;
// ➢ int mt_setcursorvisible (int value) – скрывает или
// показывает курсор;
// ➢ int mt_delline (void) – очищает текущую строку

int mt_setdefaultcolor(void) {
    if (write(1, "\033[0m", 4) == -1) {
        return -1;
    }
    return 0;
}

int mt_setcolor(enum colors fg, enum colors bg) {
    if (fg >= BLACK && fg <= WHITE && bg >= BLACK && bg <= WHITE) {
        char buf[50];
        sprintf(buf, "\033[3%d;4%dm", fg, bg);

        unsigned int len = strlen(buf);
        if (write(1, buf, len) == -1) {
            return -1;
        }
        
        return 0;
    } else {
        return -1;
    }
}

int mt_setcursorvisible(int value) {
    if (value != 0 && value != 1) {
        return -1;
    }
    const char *seq = value ? "\033[?25h" : "\033[?25l";
    if (write(1, seq, 6) == -1) {
        return -1;
    }
    return 0;
}

int mt_delline(void) {
    if (write(STDOUT_FILENO, "\033[2K", 4) == -1) {
        return -1;
    }
    return 0;
}

// int main(void) {
    // printf("Testing mt_clrscr()...\n");
    // mt_clrscr();

    // printf("Testing mt_gotoXY()...\n");
    // mt_gotoXY(10, 20);

    // int rows, cols;
    // if (mt_getscreensize(&rows, &cols) == 0) {
    //     printf("Screen size: %d rows, %d cols\n", rows, cols);
    // } else {
    //     printf("Failed to get screen size\n");
    // }

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

    // printf("Testing mt_setcursorvisible()...\n");
    // mt_setcursorvisible(0);
    // sleep(2);
    // mt_setcursorvisible(1);

    // printf("Testing mt_delline()...\n");
    // mt_gotoXY(rows / 2, 0);
    // printf("This line will be deleted in 2 seconds\n");
    // sleep(2);
    // mt_delline();

//     mt_setdefaultcolor();
//     printf("Test completed.\n");

//     return 0;
// }
