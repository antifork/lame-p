all:
	gcc -o test target.c -llame-p -L.. -Wl,--undefined=__traceme -Wl,--undefined==__constructor 
	objcopy test test.strip --set-section-flags .got.foo=alloc,load -S
	elfsh -f test.strip -w test.shtrm -shtrm
clean:
	@rm -f test test.strip test.shtrm
