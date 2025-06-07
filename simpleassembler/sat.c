#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mySimpleComputer.h"
#include "mySat.h"

#define MAX_LINE_LENGTH 100
#define MAX_COMMAND_LENGTH 10

typedef struct {
    char *name;
    int code;
} Command;

Command command_table[] = {
    {"READ", 0x0A},
    {"WRITE", 0x0B},
    {"LOAD", 0x14},
    {"STORE", 0x15},
    {"ADD", 0x1E},
    {"SUB", 0x1F},
    {"DIVIDE", 0x20},
    {"MUL", 0x21},
    {"JUMP", 0x28},
    {"JNEG", 0x29},
    {"JZ", 0x2A},
    {"HALT", 0x2B},
    // {"=", 0x0}, 
    {NULL, -1}
};

int command_to_code(const char *name) {
    for (int i = 0; command_table[i].name != NULL; i++) {
        if (strcmp(command_table[i].name, name) == 0) {
            return command_table[i].code;
        }
    }
    return -1;
}

int parse_line(char *line, int *address, int *value) {
    char cmd[MAX_COMMAND_LENGTH];
    int operand;

    char *comment = strchr(line, ';');
    if (comment) *comment = '\0';

    // удал пробелы
    while (isspace(*line)) line++;
    if (*line == '\0') return 0;

    if (strstr(line, "=") != NULL) {
        if (sscanf(line, "%d = +%d", address, value) == 2) {
            return 1;
        } else {
            return -1; 
        }
    } else {
        if (sscanf(line, "%d %s %d", address, cmd, &operand) == 3) {
            int code = command_to_code(cmd);
            if (code == -1) return -1; 
            *value = (code << 7) | (operand & 0x7F); //обьеденяем в ячек
            return 1;
        } else {
            return -1; 
        }
    }
}

int assemble(const char *src_path, const char *out_path) {
    FILE *src = fopen(src_path, "r");
    if (!src) {
        perror("fopen (input)");
        return 1;
    }

    FILE *out = fopen(out_path, "wb");
    if (!out) {
        perror("fopen (output)");
        fclose(src);
        return 1;
    }

    int memory[MEMORY_SIZE] = {0};
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, src)) {
        int addr, val;
        int res = parse_line(line, &addr, &val);
        if (res == 1) {
            if (addr < 0 || addr >= MEMORY_SIZE) {
                fprintf(stderr, "Invalid memory address: %d\n", addr);
                continue;
            }
            memory[addr] = val;
        }
    }


    fwrite(memory, sizeof(int), MEMORY_SIZE, out);
    fclose(src);
    fclose(out);
    return 0;
}

int run_assembler_prompt() {
    char run_command[MAX_LINE_LENGTH];
    char *argv[3];

    mt_gotoXY(28, 2);
    printf("Запуск транслятора (пример: sat file.sa file.o): ");
    fflush(stdout);

    rk_mytermregime(1, 0, 0, 1, 1);
    fgets(run_command, sizeof(run_command), stdin);
    rk_mytermregime(0, 0, 1, 0, 0);

    run_command[strcspn(run_command, "\n")] = 0;

    char *token = strtok(run_command, " ");
    int i = 0;
    while (token != NULL && i < 3) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }

    if (i != 3 || strcmp(argv[0], "sat") != 0) {
        return -1;
    }

    // mt_gotoXY(29, 2);
    // printf("file.sa: %s file.o: %s", argv[1], argv[2]);
    // fflush(stdout);

    return assemble(argv[1], argv[2]);
}

