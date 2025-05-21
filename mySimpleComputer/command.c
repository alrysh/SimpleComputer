#include "mySimpleComputer.h"

int sc_commandValidate(int command) {
  if (command < 0 || command > 128) {
    return -1;
  }
  return 0;
}

// int sc_commandValidate(int command) {
//     switch (command) {
//         case 0x10: case 0x11:
//         case 0x20: case 0x21: case 0x22: case 0x23:
//         case 0x30: case 0x31: case 0x32: case 0x33:
//         case 0x40: case 0x41: case 0x42: case 0x43:
//             return 0;
//         default:
//             return -1;
//     }
// }

int sc_commandEncode(int sign, int command, int operand, int *value) {
  if (value == NULL || operand < 0 || operand > 127 ||
      sc_commandValidate(command) == -1 || (sign != 0 && sign != 1)) {
    return -1;
  }

  // итоговый двоичный код команды
  *value =
      (sign << 15) | ((command & COMMAND_MASK) << 7) | (operand & COMMAND_MASK);
  return 0;
}

int sc_commandDecode(int value, int *sign, int *command, int *operand) {
  if (sign == NULL || command == NULL || operand == NULL) {
    return -1;
  }

  *sign = (value & SIGN_MASK) >> 15;
  *command = (value & OPCODE_MASK) >> 7;
  *operand = value & COMMAND_MASK;

  if (sc_commandValidate(*command) == -1) {
    return -1;
  }
  return 0;
}
