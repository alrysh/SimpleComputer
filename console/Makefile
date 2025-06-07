CC = gcc
CFLAGS = -Wall -I../mySimpleComputer/../include -I../myTerm -I../myBigChars -I../myReadKey -I../simpleassembler -I../simplebasic
LDFLAGS = -L../mySimpleComputer -lmySimpleComputer \
          -L../myBigChars -lmyBigChars \
          -L../myTerm -lmyTerm \
		  -L../myReadKey -lmyReadKey \
		  -L../simpleassembler -lmySat \
		  -L../simplebasic -lmyBasic

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
	$(MAKE) -C ../myReadKey
	$(MAKE) -C ../simpleassembler
	$(MAKE) -C ../simplebasic

clean:
	rm -f $(TARGET) $(FONTGEN) $(FONT_OUT)
	$(MAKE) -C ../mySimpleComputer clean
	$(MAKE) -C ../myTerm clean
	$(MAKE) -C ../myBigChars clean
	$(MAKE) -C ../myReadKey clean
	$(MAKE) -C ../simpleassembler clean
	$(MAKE) -C ../simplebasic clean
