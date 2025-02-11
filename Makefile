TARGET = test

SOURCE = console/test.c

OBJECT = console/test.o

CC = gcc

CFLAGS = -Wall -g

# Правило по умолчанию
all: $(TARGET)

# Правило для создания объектного файла
$(OBJECT): $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE) -o $(OBJECT)

# Правило для создания исполняемого файла
$(TARGET): $(OBJECT)
	$(CC) $(OBJECT) -o $(TARGET)

clean:
	rm -f $(TARGET) $(OBJECT)

