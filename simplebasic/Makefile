CC = gcc
CFLAGS = -Wall -I../include
AR = ar
ARFLAGS = rcs

LIB_NAME = libmyBasic.a

SRC = basic.c

OBJ = $(SRC:.c=.o)

all: $(LIB_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(OBJ) $(LIB_NAME)

.PHONY: all clean