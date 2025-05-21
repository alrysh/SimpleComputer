#include "myReadkey.h"
#include "mySimpleComputer.h"

#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int rk_readkey(enum keys *key) {
    char buf[10] = {0};
    read(STDIN_FILENO, buf, sizeof(buf));
    
    if (!strcmp(buf, "\E")) *key = KEY_ESC;
    else if (!strcmp(buf, "\n")) *key = KEY_ENTER;
    else if (!strcmp(buf, "\E[A")) *key = KEY_UP;
    else if (!strcmp(buf, "\E[B")) *key = KEY_DOWN;
    else if (!strcmp(buf, "\E[C")) *key = KEY_RIGHT;
    else if (!strcmp(buf, "\E[D")) *key = KEY_LEFT;
    else if (!strcmp(buf, "\E[15~")) *key = KEY_F5;
    else if (!strcmp(buf, "\E[17~")) *key = KEY_F6;
    else if (buf[0] == 's') *key = KEY_S;
    else if (buf[0] == 'l') *key = KEY_L;
    else if (buf[0] == 'i') *key = KEY_I;
    else if (buf[0] == 'r') *key = KEY_R;
    else if (buf[0] == 't') *key = KEY_T;
    // else if (buf[0] == '+') *key = KEY_PLUS;
    // else if (buf[0] == '-') *key = KEY_MINUS;
    // else if (buf[0] >= '0' && buf[0] <= '9') *key = KEY_0 + (buf[0] - '0');
    // else if (buf[0] >= 'a' && buf[0] <= 'z') *key = KEY_A + (buf[0] - 'a');
    // else if (buf[0] >= 'A' && buf[0] <= 'Z') *key = KEY_A + (buf[0] - 'A');
    else *key = KEY_UNKNOWN;

    return 0;
}


struct termios term_saved;

int rk_mytermsave(void) {
    return tcgetattr(STDIN_FILENO, &term_saved);
}

int rk_mytermrestore(void) {
    return tcsetattr(STDIN_FILENO, TCSANOW, &term_saved);
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint) {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);

    if (regime == 0) {
        term.c_lflag &= ~(ICANON);
        term.c_cc[VTIME] = vtime;
        term.c_cc[VMIN] = vmin;
    } else {
        term.c_lflag |= ICANON;
    }

    if (echo == 0)
        term.c_lflag &= ~(ECHO);
    else
        term.c_lflag |= ECHO;

    if (sigint == 0)
        term.c_lflag &= ~(ISIG);
    else
        term.c_lflag |= ISIG;

    return tcsetattr(STDIN_FILENO, TCSANOW, &term);
}


int rk_readvalue(int *value, int timeout) {
    char input[6] = {0};
    int i = 0;
    char ch;
    int hex_flag = 0;

    rk_mytermregime(0, timeout, 1, 0, 0);  // неканонический режим

    while (i < 5) {
        if (read(STDIN_FILENO, &ch, 1) <= 0)
            break;

        if ((ch >= '0' && ch <= '9') || ch == '+' || ch == '-') {
            input[i++] = ch;
            write(STDOUT_FILENO, &ch, 1);

        } else if (ch >= 'A' && ch <= 'F') {
            input[i++] = ch;
            write(STDOUT_FILENO, &ch, 1);
            hex_flag++;

        } else if (ch == '\n') {
            break;
        }
    }

    input[i] = '\0';

    int sign = 0;
    int command = 0;
    int operand = 0;

    if (input[0] == '+')
        sign = 0;
    else if (input[0] == '-')
        sign = 1;
    else {
        return -1;
    }

    if (i < 5) return -1;  // Защита от короткого ввода

    char cmd_part[3] = {input[1], input[2], '\0'};
    char op_part[3] = {input[3], input[4], '\0'};

    command = (int)strtol(cmd_part, NULL, 16);
    operand = (int)strtol(op_part, NULL, 16);

    //!CHECK
    mt_gotoXY(27, 90);
    printf("sign: %d command: %04X operand: %04X", sign, command, operand);
    fflush(stdout);

    if (command < 0|| command > 0x7F || operand < 0 || operand > 0x7F) {
        // mt_gotoXY(28, 25);
        // printf("Error command or operand");
        // fflush(stdout);
        return -1;
    }

    if (sc_commandEncode(sign, command, operand, value) == -1) {
        // mt_gotoXY(29, 25);
        // printf("Error commandEncode");
        // fflush(stdout);
        return -1;
    }

    return 0;
}

int rk_readvalue_simple(int *value, int timeout) {
    char input[5] = {0};
    int i = 0;
    char ch;

    rk_mytermregime(0, timeout, 1, 0, 0);  // неканонический режим

    while (i < 4) {
        if (read(STDIN_FILENO, &ch, 1) <= 0)
            break;

        if ((ch >= '0' && ch <= '9') || ch == '+' || ch == '-') {
            input[i++] = ch;
            write(STDOUT_FILENO, &ch, 1);

        } else if (ch == '\n') {
            break;
        }
    }

    input[i] = '\0';

    int result = atoi(input);

    //!CHECK
    mt_gotoXY(28, 90);
    printf("IC dec: %d", result);
    fflush(stdout);

    if (result < 0 || result > 127) {
        return -1;
    } else 
        *value = atoi(input);

    return 0;
}

