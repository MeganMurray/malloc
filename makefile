all: 

test%: test%.c
	gcc -o testfiles/$@ $<

clean: 
	rm -r testfiles/*

