#include <fcntl.h>
#include <myBigChars.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
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
               
    int fd = open("font.bin", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Ошибка открытия font.bin");
        return -1;
    }
    for (int i = 0; i < 18; i++) {
        if (bc_bigcharwrite(fd, bigchars[i], 1) != 0) {
            close(fd);
            perror("Ошибка записи в font.bin");
            return -1;
        }
    }

    close(fd);
    return 0;
}