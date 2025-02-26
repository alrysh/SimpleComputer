#include "mySimpleComputer.h"
#include <stdio.h>

void
printCell (int address)
{
  printf ("Memory[%d]: \n", address);
}

void
printFlags (void)
{
  printf ("Flags: %c %c %c %c %c\n", (flags & FLAG_OVERFLOW) ? 'P' : '_',
          (flags & FLAG_ZERO_DIV) ? '0' : '_',
          (flags & FLAG_MEMORY_OUT) ? 'M' : '_',
          (flags & FLAG_CLOCK_IGNORE) ? 'T' : '_',
          (flags & FLAG_INVALID_CMD) ? 'E' : '_');
}

void
printDecodedCommand (int value)
{
  printf ("Dec: %d, Oct: %o, Hex: %X, Bin: ", value, value, value);
  for (int i = 15; i >= 0; i--)
    {
      printf ("%d", (value >> i) & 1);
    }
  printf ("\n");
}

void
printAccumulator (void)
{
  printf ("Accumulator: %d\n", accumulator);
}

void
printCounters (void)
{
  printf ("Instruction Counter: %d\n", instructCounter);
}
