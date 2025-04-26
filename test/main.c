#include <stdio.h>
#include <string.h>
#include <unistd.h> 

int bigchars[18][2] = {
    //цифры 0-9
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
    
    //буквы A-H
    {0x183C6642, 0x42FF4242},  //A
    {0xFC42427C, 0x424242FC},  //B
    {0x3C428080, 0x8080423C},  //C
    {0xF8444242, 0x424244F8},  //D
    {0xFE8080F8, 0x808080FE},  //E
    {0xFE8080F8, 0x80808080},  //F
    {0x3C42809E, 0x8242423E},  //G
    {0x4242427E, 0x42424242},   //H
    {0x00081800, 0x00183C18}   // +
};

void print_char_line(int part, int line) {
    write(1, "\033(0", 3);
    for (int j = 0; j < 8; j++) {
        printf("%c", (part & (1 << (7 - j))) ? 'a' : ' ');
    }
    fflush(stdout);
    write(1, "\033(B", 3);
}

void print_big_string(const char *text) {
    int len = strlen(text);
    
    for (int line = 0; line < 8; line++) {
        for (int i = 0; i < len; i++) {
            char c = text[i];
            int index = -1;
            
            if (c >= '0' && c <= '9') index = c - '0';
            else if (c >= 'A' && c <= 'H') index = 10 + (c - 'A');
            else if (c >= 'a' && c <= 'h') index = 10 + (c - 'a');
            
            if (index >= 0 && index < 18) {
                int part = (line < 4) ? bigchars[index][0] : bigchars[index][1];
                print_char_line((part >> ((3 - (line % 4)) * 8)) & 0xFF, line);
            } else {
                printf("        ");
            }
            printf(" ");
        }
        printf("\n");
    }
}

int main() {
    printf("Правильные цифры:\n");
    print_big_string("0123456789");
    
    printf("\nБуквы A-H:\n");
    print_big_string("ABCDEFGH");
    
    printf("\nПроизвольная строка:\n");
    print_big_string("01A2B3C4D");
    
    return 0;
}