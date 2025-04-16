CC = gcc
CFLAGS = -Wall -I../include -I../myTerm
LDFLAGS = -L../myTerm -lmyTerm

TARGET = libmyBigChars.a
SRC = bigchar_func.c
OBJ = bigchar_func.o

all: $(TARGET)

$(TARGET): $(OBJ)
	# Создание статической библиотеки с зависимостью от myTerm
	ar rcs $@ $^
	
	# Дополнительно создаем объектный файл, связанный с myTerm
	$(CC) -o libmyBigChars_with_term.a -nostdlib -r $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ) libmyBigChars_with_term.a


# CC = gcc
# CFLAGS = -Wall -I../include
# AR = ar
# ARFLAGS = rcs

# TARGET = libmyBigChars.a
# SRC = bigchar_func.c
# OBJ = bigchar_func.o

# MYTERM_DIR = ../myTerm
# MYTERM_LIB = $(MYTERM_DIR)/libmyTerm.a

# all: $(MYTERM_LIB) $(LIB_NAME)

# %.o: %.c
# 	$(CC) $(CFLAGS) -I$(MYTERM_DIR) -c $< -o $@

# $(LIB_NAME): $(OBJ) $(MYTERM_LIB)
# 	$(AR) $(ARFLAGS) $@ $^

# # Сборка myTerm (там есть свой Makefile)
# $(MYTERM_LIB):
# 	$(MAKE) -C $(MYTERM_DIR)

# clean:
# 	rm -f $(OBJ) $(LIB_NAME)
# 	$(MAKE) -C $(MYTERM_DIR) clean

# .PHONY: all clean