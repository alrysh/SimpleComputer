
all:
	make -C mySimpleComputer
	make -C myTerm
	make -C myBigChars
	make -C console

clean:
	make -C mySimpleComputer clean
	make -C myTerm clean
	make -C myBigChars clean
	make -C console clean
