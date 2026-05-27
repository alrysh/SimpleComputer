
all:
	make -C myTerm
	make -C mySimpleComputer
	make -C console

clean:
	make -C mySimpleComputer clean
	make -C console clean
