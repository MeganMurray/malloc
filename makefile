all: testcfiles
	make test1
	make test2
	make test3
	make test4
	make test5
	make test6
	make test7

test%: testcfiles/test%.c
	gcc -o testfiles/$@ $<

clean: 
	rm -r testfiles/*

