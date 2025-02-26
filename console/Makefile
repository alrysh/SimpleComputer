CC = gcc
CFLAGS = -Wall -I../include
AR = ar
ARFLAGS = rcs

LIB_DIR = ../mySimpleComputer
LIB_NAME = $(LIB_DIR)/libmySimpleComputer.a
LIB_SRC = $(LIB_DIR)/command.c $(LIB_DIR)/memory.c $(LIB_DIR)/print.c $(LIB_DIR)/register.c
LIB_OBJ = $(LIB_SRC:.c=.o)

PRG_SRC = pr01.c
PRG_OBJ = $(PRG_SRC:.c=.o)
PRG_NAME = pr01

all: $(LIB_NAME) $(PRG_NAME)

#компиляция объектных файлов библиотеки
$(LIB_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#сборка статической библиотеки
$(LIB_NAME): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $^

#компиляция pr01
$(PRG_OBJ): $(PRG_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

#линковка pr01 с библиотекой
$(PRG_NAME): $(PRG_OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $(PRG_OBJ) $(LIB_NAME)

clean:
	rm -f $(LIB_DIR)/*.o $(LIB_NAME) $(PRG_OBJ) $(PRG_NAME)

.PHONY: all clean
