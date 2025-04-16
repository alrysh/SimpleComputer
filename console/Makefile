CC = gcc
CFLAGS = -Wall -I../mySimpleComputer/../include -I../myTerm -I../myBigChars
LDFLAGS = -L../mySimpleComputer -lmySimpleComputer \
          -L../myBigChars -lmyBigChars \
          -L../myTerm -lmyTerm

TARGET = console
SRC = console.c
FONTGEN = font
FONT_SRC = font.c
FONT_OUT = font.bin

all: deps $(FONT_OUT) $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(FONTGEN): $(FONT_SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(FONT_OUT): $(FONTGEN)
	./$(FONTGEN) > $(FONT_OUT)

.PHONY: all clean deps

deps:
	$(MAKE) -C ../myTerm
	$(MAKE) -C ../mySimpleComputer
	$(MAKE) -C ../myBigChars

clean:
	rm -f $(TARGET) $(FONTGEN) $(FONT_OUT)
	$(MAKE) -C ../mySimpleComputer clean
	$(MAKE) -C ../myTerm clean
	$(MAKE) -C ../myBigChars clean
