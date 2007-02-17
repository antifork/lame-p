all:
	$(CC) -c liblame-p.c -O0 
	ar rcs liblame-p.a liblame-p.o
#	$(CC) -o liblame-p.so liblame-p.o -shared -Wl,-soname=liblame-p.so
clean:
	@rm -f liblame-p.o liblame-p.a liblame-p.so


