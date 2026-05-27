CC = gcc
CFLAGS = -Wall -I../include
AR = ar
ARFLAGS = rcs

LIB_NAME = libmyReadKey.a

SRC = readkey_func.c       # Указываем исходный файл
OBJ = $(SRC:.c=.o)        # Объектный файл: readkey_func.o

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(OBJ) $(LIB_NAME)

.PHONY: all clean