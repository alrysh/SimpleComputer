#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myBigChars.h"
#include "myReadkey.h"
#include "mySat.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>

volatile int running = 1; //для while(running)

void prompt_filename(char *filename, size_t size) {
    mt_gotoXY(28, 2);
    mt_setdefaultcolor();
    printf("Введите имя файла: ");
    fflush(stdout);

    rk_mytermregime(1, 0, 0, 1, 1);  // канон реж для fgets
    fgets(filename, size, stdin);
    rk_mytermregime(0, 0, 1, 0, 0);  // неканон обратно

    filename[strcspn(filename, "\n")] = 0;
}

void run_interactive_mode(void) {
    rk_mytermsave();
    rk_mytermregime(0, 1, 1, 0, 1);

    enum keys key;
    int cursor = 0;
    int oldcursor = 0;

    int nowCell = 0;

    while (1) {
        
        printCursor(oldcursor, BLACK);
        printCursor(cursor, RED);

        rk_readkey(&key);

        int impulsValue;
        int ignoreImpuls;
        sc_regGet(FLAG_CLOCK_IGNORE, &impulsValue);

        // mt_gotoXY(29, 10);
        // printf("%d", impulsValue);
        // fflush(stdout);

        if (impulsValue == 1) {
            switch (key) {
                case KEY_ENTER:
                    nowCell = cursor;
                    editCell(nowCell);
                    break;

                // case KEY_ESC:
                //     rk_mytermrestore();
                //     return;

                case KEY_F5:
                    int a_value;
                    mt_gotoXY(2, 69);
                    rk_readvalue(&a_value, 5);
                    sc_accumulatorSet(a_value);
                    printAccumulator();
                    break;

                case KEY_F6:
                    int c_value;
                    mt_gotoXY(5,69);
                    rk_readvalue_simple(&c_value, 5);
                    sc_icounterSet(c_value);
                    printCounters();
                    break;


                case KEY_S:
                    // sc_memorySave(file);
                    char file[256];
                    prompt_filename(file, sizeof(file));
                    if (sc_memorySave(file) == 0) {
                        printblack(29, 2, "Память сохранена");
                    }
                    else {
                        printblack(29, 2, "Ошибка при сохранении");
                    }
                    break;
                    // printAllMemory();

                case KEY_L:
                    char file_load[256];
                    prompt_filename(file_load, sizeof(file_load));
                    if (sc_memoryLoad(file_load) == 0) {
                        // mt_gotoXY(29,10);
                        // printblack();
                        // printf("Память загружена");
                        // fflush(stdout);
                        printblack(29, 10, "Память загружена");
                    }
                    else {
                        printblack(29, 10, "Ошибка при загрузке");
                    }
                    printAllMemory();
                    break;
                
                case KEY_I:
                    //sc_memoryLoad("command.bin"); //файл памяти по умолчанию
                    sc_memoryInit();
                   
                    sc_accumulatorSet(0);
                    sc_icounterSet(0);
                    
                    fillMemory();
                    printAllMemory();
                    break;

                case KEY_A:
                    // run_assembler_prompt();
                    assemble("main.sa", "main.o");
                    break;

                case KEY_B:
                    parse_basic("test.sb", "test.sa");
                    break;

                case KEY_UP:
                    oldcursor = cursor;
                    if (cursor < 10 && cursor >= 8)
                        cursor = cursor + 110;
                    else if (cursor < 8)
                        cursor = cursor + 120;
                    else 
                        cursor = cursor - 10;
                    break;

                case KEY_DOWN:
                    oldcursor = cursor;
                    if (cursor >= 120)
                        cursor = cursor - 120;
                    else if (cursor >= 118 && cursor < 120)
                        cursor = cursor - 110;
                    else
                        cursor = cursor + 10;
                    break;

                case KEY_LEFT:
                    oldcursor = cursor;
                    cursor = (cursor == 0) ? cursor + 127 : cursor - 1;
                    break;

                case KEY_RIGHT:
                    oldcursor = cursor;
                    cursor = (cursor == 127) ? cursor - 127 : cursor + 1;
                    break;

                default:
                    break;
            }
        }

        switch (key) {
            // case KEY_I:
            //   IRC (SIGUSR1);
            //   currentMC = 0;
            //   break;

            case KEY_R:
                // rk_mytermregime(1, 0, 0, 0, 1);  // канон реж (все нули?)
                sc_regSet(FLAG_CLOCK_IGNORE, 0);
                signals();
                
                // rk_mytermregime(0, 0, 1, 0, 0);
                break;
                //   sc_regGet (, &ignoreImpuls);
                //   if (ignoreImpuls == 1)
                //     {
                //       sc_regSet (IMPULS, 0);
                //       signals ();
                //       IRC (SIGALRM);
                //     }
                //   else
                //     {
                //       sc_regSet (IMPULS, 1);
                //       alarm (0);
                //     }
                //   break;

            case KEY_T:
                raise(SIGALRM);
                // sc_icounterGet (&currentMC);
                break;
                
            case KEY_ESC:
                    rk_mytermrestore();
                    return;    

            default:
                break;
        }   

        nowRedact = cursor;
        mt_setfgcolor(BLUE);
        mt_gotoXY(10, 65);
        printBigCell();

        printCellFormat();
    }
}
