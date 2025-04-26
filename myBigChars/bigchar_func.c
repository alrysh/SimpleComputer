#include "mySimpleComputer.h"
#include "myBigChars.h"
#include "myTerm.h"

#include <stdio.h>
#include <stddef.h> 
#include <unistd.h> 
#include <string.h>

int bigchar[18][2] = {0};
int bigchars[18][2] = {
    {0x7E818181, 0x8181817E},  //0
    {0x18381818, 0x1818187E},  //1
    {0x7E81011E, 0x608080FE},  //2
    {0x7E81017E, 0x0181817E},  //3
    {0x8181817F, 0x01010101},  //4
    {0xFF807E01, 0x0181817E},  //5
    {0x7E807E81, 0x8181817E},  //6
    {0xFF010204, 0x08102040},  //7
    {0x7E817E81, 0x8181817E},  //8
    {0x7E81817F, 0x0101017E},  //9
    
    {0x183C6642, 0x42FF4242},  //A
    {0xFC42427C, 0x424242FC},  //B
    {0x3C428080, 0x8080423C},  //C
    {0xF8444242, 0x424244F8},  //D
    {0xFE8080F8, 0x808080FE},  //E
    {0xFE8080F8, 0x80808080},  //F
    {0xFF, 0x0},  //-
    {0x1818FF, 0x18180000}   // +
};


int bc_strlen(const char *str) {
    if (str == NULL) {
        return -1;
    }

    size_t count = 0;
    while (*str != '\0') {
        unsigned char byte = (unsigned char)*str;

        if ((byte & 0x80) == 0) {
            // 1-байтовый символ (0xxxxxxx)
            count++;
            str++;
        } else if ((byte & 0xE0) == 0xC0) {
            // 2-байтовый символ (110xxxxx)
            if ((str[1] & 0xC0) != 0x80) {
                return -1;
            }
            count++;
            str += 2;
        } else if ((byte & 0xF0) == 0xE0) {
            // 3-байтовый символ (1110xxxx)
            if ((str[1] & 0xC0) != 0x80 || (str[2] & 0xC0) != 0x80) {
                return -1;
            }
            count++;
            str += 3;
        } else if ((byte & 0xF8) == 0xF0) {
            // 4-байтовый символ (11110xxx)
            if ((str[1] & 0xC0) != 0x80 || (str[2] & 0xC0) != 0x80 || 
                (str[3] & 0xC0) != 0x80) {
                return -1;
            }
            count++;
            str += 4;
        } else {
            return -1;
        }
    }

    // *result = count;
    // return 0;
    return count;
}

// • int bc_printA (char * str) - выводит строку символов с 
// использованием дополнительной кодировочной таблицы; 

int bc_printA (char * str) {
    if (str == NULL) {
        return -1;
    }

    write(1,"\E(0", 3);

    size_t len = strlen(str);

    write(1, str, len);

    write(1,"\E(B", 3);

    return 0;
}

// int bc_box(int x1, int y1, int x2, int y2, enum
//     colors box_fg, enum colors box_bg, char *header, 
//     enum colors header_fg, enum colors header_bg) -
//     выводит на экран псевдографическую рамку, в которой левый 
//     верхний угол располагается в строке x1 и столбце y1, а её ширина 
//     и высота равна y2 столбцов и x2 строк. Цвет псевдографических 
//     символов и их фон указан в параметрах box_fg, box_bg. Если пе-
//     редан корректный указатель на header и полученная строка деко-
//     дируется

int bc_box(int x1, int y1, int x2, int y2,
    enum colors box_fg, enum colors box_bg, char *header, 
    enum colors header_fg, enum colors header_bg) {

    if (header != NULL && bc_strlen(header) == -1) {
        return -1;
    }

    // mt_clrscr();

    mt_gotoXY(x1,y1);
    mt_setcolor(box_fg, box_bg);
    bc_printA("l");
    fflush(stdout);
    for (int i = 0; i < y2-2;  i++) { //-2 - l и q
        bc_printA("q");
        fflush(stdout);
    }
    bc_printA("k");
    fflush(stdout);

    // mt_gotoXY(x1, (y2 - y1)/2 - strlen(header)/2);
    mt_gotoXY(x1, y1+10);
    //mt_gotoXY(x1, y1 + (y2 - y1) / 2);

    
    mt_setcolor(header_fg, header_bg);
    write(1, header, strlen(header));
    mt_setdefaultcolor();
    printf("\n");

    mt_gotoXY(x1+1,y1);
    mt_setcolor(box_fg, box_bg);
    for (int i = 0; i < x2; i++) {
        mt_gotoXY(x1 + i + 1, y1);
        bc_printA("x");
        fflush(stdout);
        
        for (int j = 0; j < y2 - 2; j++) {
            bc_printA(" ");
            fflush(stdout);
        }
    
        bc_printA("x");
        fflush(stdout);
    }
    
    mt_gotoXY(x1+x2+1,y1);
    mt_setcolor(box_fg, box_bg);
    bc_printA("m");
    for (int i = 0; i < y2 - 2;  i++) {
        bc_printA("q");
        fflush(stdout);
    }
    bc_printA("j");

    mt_gotoXY(x2*2,1);
    // mt_gotoXY(60,1);
    fflush(stdout);

    return 0;
}

// int bc_setbigcharpos (int *big, int x, int y, 
//     int value) – манипулирует элементом шрифта и устанавливает 
//     в нем значение знакоместа "большого символа" в строке x и столб-
//     це y в значение value;

int bc_setbigcharpos(int *big, int x, int y, int value) {
    if (x < 0 || x > 7 || y < 0 || y > 7 || (value != 0 && value != 1)) {
        return -1;
    }

    int index = (x < 4) ? 0 : 1;
    int bit_pos = (x % 4) * 8 + y;

    if (value == 1) {
        big[index] |= (1 << bit_pos);
    } else {
        big[index] &= ~(1 << bit_pos);
    }

    return 0;
}

int bc_getbigcharpos(int *big, int x, int y, int *value) {
    if (x < 0 || x > 7 || y < 0 || y > 7 || value == NULL) {
        return -1;
    }

    int index = (x < 4) ? 0 : 1;
    int bit_pos = (x % 4) * 8 + y;

    *value = (big[index] >> bit_pos) & 1;

    return 0;
}

// int bc_printbigchar(int big[2], int x, int y, enum colors fg, enum colors bg)
// {
//     write(1, "\033(0", 3);

//     mt_setfgcolor(fg);
//     mt_setbgcolor(bg);

//     for (int y1 = 0; y1 != 8; y1++) {
//         mt_gotoXY(x, y + y1);
//         for (int x1 = 0; x1 != 8; x1++) {
//             int value;
//             bc_getbigcharpos(big, x1, y1, &value);
//             printf("%c", value ? 'a' : ' ');
//         }
//         fflush(stdout);
//     }
//     write(1, "\033(B", 3);
//     mt_setdefaultcolor();
//     return 0;
// }    

int bc_bigcharwrite(int fd, int* big, int count)
{
    int flag;
    flag = write(fd, big, count * sizeof(int) * 2);
    if (flag != count * sizeof(int) * 2) {
        return -1;
    }
    return 0;
}

int bc_bigcharread(int fd, int* big, int need_count, int* count)
{
    if (!big || !count)
        return -1;
    int flag;
    flag = read(fd, big, need_count * sizeof(int) * 2);
    if ((flag / (sizeof(int) * 2)) == need_count) {
        *count = flag / (sizeof(int) * 2);
        return 0;
    }
    *count = 0;
    return -1;
}

void bc_print_char_line(int part) {
    write(1, "\033(0", 3);
    for (int j = 0; j < 8; j++) {
        printf("%c", (part & (1 << (7 - j))) ? 'a' : ' ');
    }
    fflush(stdout);
    write(1, "\033(B", 3);
}

void bc_printbigchar(int index, int x, int y, enum colors fg, enum colors bg) {
    if (index < 0 || index >= 18) {
        return;
    }

    mt_setfgcolor(fg);
    mt_setbgcolor(bg);

    for (int line = 0; line < 8; line++) {
        mt_gotoXY(x + line, y);

        int part = (line < 4) ? bigchars[index][0] : bigchars[index][1];
        int byte = (part >> ((3 - (line % 4)) * 8)) & 0xFF;
        bc_print_char_line(byte);
    }

    mt_setdefaultcolor();
}
