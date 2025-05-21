#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myBigChars.h"
#include "myReadkey.h"

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

void setup_timer() {
    struct itimerval timer;
    timer.it_interval.tv_sec = 0;     // интервал: 1 сек
    timer.it_interval.tv_usec = 500000;
    timer.it_value.tv_sec = 0;        // первое срабатывание: через 1 сек
    timer.it_value.tv_usec = 500000;

    setitimer(ITIMER_REAL, &timer, NULL);
}

void CU(int sig) {

    // RESET
    if (sig == SIGUSR1) {
        accumulator = 0;
        instructCounter = 0;
        sc_memoryInit();
        sc_regInit();

        sc_regSet(FLAG_OVERFLOW, 0);
        sc_regSet(FLAG_ZERO_DIV, 0);
        sc_regSet(FLAG_MEMORY_OUT, 0);
        sc_regSet(FLAG_INVALID_CMD, 0);
        sc_regSet(FLAG_CLOCK_IGNORE, 1);

        mt_gotoXY(30,10);
        printf("[RESET]\n");
        fflush(stdout);

        return;
    }

    //генератор такт импульсов
    if (sig == SIGALRM) { 
        // mt_gotoXY(27,2);
        // printf("CU HANDLED: %d", sig);
        // fflush(stdout);

        int value = 0;
        int ac_value = 0;
        int sign = 0;
        int command = 0;
        int address = 0;

        if (sc_memoryGet(instructCounter, &value) != 0) {
            sc_regSet(FLAG_MEMORY_OUT, 1);
            return;
        }

        mt_gotoXY(27, 40);
        printf("instructCounter: %d", instructCounter);
        fflush(stdout);
        // sc_regSet(FLAG_CLOCK_IGNORE, 1);
        printCommand();
        printCounters();

        if (sc_commandDecode(value, &sign, &command, &address) == -1)
        {
            sc_regSet(FLAG_INVALID_CMD, 1);
            sc_regSet(FLAG_CLOCK_IGNORE, 1);

            mt_gotoXY(29,10);
            printf("Decoded: sign=%d, command=0x%04X, address=%d\n", sign, command, address); //обнуляется
            fflush(stdout);
        }

        int val;

        switch (command) {
            case 0x01: // CPUINFO
                mt_gotoXY(27,2);
                printf("Шкатулова София Михайловна, ИВ-322\n");
                fflush(stdout);
                break;
            
            //INPUT
            case 0x0A:
                char input[10];
                // mt_gotoXY(28, 2);
                mt_setfgcolor(YELLOW);
                printblack(28, 2, "Введите значение: ");

                rk_mytermregime(1, 0, 0, 1, 1);  // канон реж для fgets
                fgets(input, sizeof(input), stdin);
                rk_mytermregime(0, 0, 1, 0, 0);  // неканон обратно

                if (sscanf(input, "%d", &value) != 1) { ///???
                    printblack(29, 2, "Ошибка ввода: ");
                } else if (value < VAL_MIN || value > VAL_MAX) {
                    printblack(29, 2, "Ошибка: значение вне диапазона");
                } else {
                    sc_memorySet(address, value);
                }
                // printAllMemory();
                break;
                
            case 0x0B:
                
                sc_memoryGet(address, &val);
                printbl(28, 2);

                mt_gotoXY(29,2);
                printf("Значение (не декодированное) hex: 0x%X, dec: %d\n", val, val);
                // printf("Значение (не декодированное): %d", val);
                // printf("\n");
                fflush(stdout);   
                break;
            
            //LOAD
            case 0x14:
                // Загрузка в аккумулятор значения из указанного адреса
                sc_memoryGet(address, &val);
                sc_accumulatorSet(val);
                break;

            //STORE
            case 0x15:
                // Выгружает значение из аккумулятора по указанному адресу
                sc_accumulatorGet(&val);
                sc_memorySet(address, val);
                break;
                
            case 0x1E: // ADD
            case 0x1F: // SUB
            case 0x20: // DIVIDE
            case 0x21: // MUL
                int operand = 0;
                sc_memoryGet(address, &operand);
                if (ALU(command, operand) == 0) {
                    break;
                    // return;
                } else if (ALU(command, operand) == -1)
                    printblack(29, 2, "ALU ERROR");
                    // return -1;
                break;
                
            // УУ
            case 0x28: // JUMP - сразу выполняет
                sc_icounterSet(address); // -1 для корр перехода (т.к instructcount++)
                break;
                // return; //? тогда будет не за один такт
                
            case 0x29: // JNEG
                if (accumulator < 0) sc_icounterSet(address - 1);
                break;
                
            case 0x2A: // JZ
                if (accumulator == 0) sc_icounterSet(address - 1);
                break;
                
            case 0x2B: // HALT
                sc_regSet(FLAG_CLOCK_IGNORE, 1);
                return; //if 1 -> finish() ?

            case 0x33:
                // NOT();
                sc_accumulatorGet(&ac_value);
                if (sc_memorySet(instructCounter, ~ac_value) == -1) 
                    sc_regSet(FLAG_OVERFLOW, 1);
                //почему обнул аккумулятор?
                break;

            case 0x34:
                 // AND();
                sc_accumulatorGet(&ac_value);
                int result = ac_value & value;
                sc_accumulatorSet(result);
                break;

            case 0x00:
                break;

            default:
                sc_regSet(FLAG_INVALID_CMD, 1); //?
                return;
        }
        instructCounter++;
    }

    // exit(sig);
    
}

int ALU (int command, int operand) {
    int result;
    int ac_operand = 0;

    if (sc_accumulatorGet(&ac_operand) == -1) {
        return -1;
    }

    printblack(28, 2, " ");

    switch (command)
    {
        case 0x1E: //0x1E:
            printblack(28, 2, "case 0x1E is run");
            result = ac_operand + operand;
            if (result > VAL_MAX) {
                sc_regSet(FLAG_OVERFLOW, 1);
                return -1;
            }
            break;

        case 0x1F: //0x1F:
            printblack(28, 2, "case 0x1F is run");
            result = ac_operand - operand;
            if (result < VAL_MIN) {
                sc_regSet(FLAG_OVERFLOW, 1);
                return -1;
            }
            break;

        case 32:
            printblack(28, 2, "case 0x20 is run");
            if (operand != 0) {
                result = ac_operand / operand;

            } else {
                sc_regSet(FLAG_ZERO_DIV, 1);
                return -1;
            }
            break;

        case 33:
            printblack(28, 2, "case 0x21 is run");
            result = ac_operand * operand;
            if (result > VAL_MAX) {
                sc_regSet(FLAG_OVERFLOW, 1);
                return -1;
            }        
            break;    

        default:
            printblack(31, 2, "command not recognized");
            break;
    }

    mt_gotoXY(29, 2);
    printf("Result: %d operand: %d", result, operand);
    fflush(stdout);

    sc_accumulatorSet(result);
    return 0;
}

void signalHandler(int sig) {
    mt_gotoXY(29,2);
    printf("Sig handled: %d", sig);
    // exit(sig);
}

void signals() {
    signal(SIGALRM, CU);   // Таймер → CU
    // signal(SIGUSR1, CU);   // Reset → CU

    // signal(SIGINT, signalHandler);
    // raise(SIGINT);

    // signal(SIGALRM, signalHandler);
    raise(SIGALRM);

    setup_timer();

    // pause();

    // int value;
    // sc_commandEncode(0, 0x0A, 5, &value); // READ в ячейку 5
    // sc_memorySet(0, value);

    // raise(SIGALRM);
    // CU(SIGALRM);
}

void IRC(int signum) {
    if (signum == SIGALRM) {
        // 1. Проверка флага игнорирования тактовых импульсов
        int value;
        sc_regGet(FLAG_CLOCK_IGNORE, &value);
        if (value) {
            return; // игнорируем импульс
        }

        // 2. Проверка счётчика простоя
        if (instructTact > 0) {
            instructTact--;
            return;
        }
        CU(signum);
    }
    else if (signum == SIGUSR1) {
        CU(signum);
    }
}
