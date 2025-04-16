CC = gcc
CFLAGS = -Wall -I../include
AR = ar
ARFLAGS = rcs

LIB_NAME = libmySimpleComputer.a

SRC = command.c memory.c print.c register.c
OBJ = $(SRC:.c=.o)

# font.o добавляем отдельно
FONT_SRC = ../console/font.c
FONT_OBJ = ../console/font.o

MYTERM_DIR = ../myTerm
MYTERM_LIB = $(MYTERM_DIR)/libmyTerm.a

all: $(MYTERM_LIB) $(LIB_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(MYTERM_DIR) -c $< -o $@

$(FONT_OBJ): $(FONT_SRC)
	$(CC) $(CFLAGS) -I$(MYTERM_DIR) -c $< -o $@

$(LIB_NAME): $(OBJ) $(FONT_OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(MYTERM_LIB):
	$(MAKE) -C $(MYTERM_DIR)

clean:
	rm -f *.o $(FONT_OBJ) $(LIB_NAME)
	$(MAKE) -C $(MYTERM_DIR) clean

.PHONY: all clean
