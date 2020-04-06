all: test.c plusone.o plustwo.o
	gcc test.c plusone.o plustwo.o
dev: test.c plusone.o
	gcc -g test.c plusone.o

plusone.o : plusone.c
	gcc -c plusone.c

plustwo.o : plustwo.c
	gcc -c plustwo.c

clean:
	rm. a.out; rm plusone.o
test: test1 
	echo “done testing”
test1:
	./a.out  > dumpfile; diff dumpfile test0 example | wc
everything: clean all test
	echo "all done"
