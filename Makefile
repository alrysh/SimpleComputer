
all:
	make -C mySimpleComputer
	make -C myTerm
	make -C myBigChars
	make -C simpleassembler
	make -C simplebasic
	make -C console

clean:
	make -C mySimpleComputer clean
	make -C myTerm clean
	make -C myBigChars clean
	make -C simpleassembler clean
	make -C simplebasic clean
	make -C console clean
