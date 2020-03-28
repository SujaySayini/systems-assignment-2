All: test.c plusone.o
	Gcc test.c plusone.o
Plusone.o : plusone.c
	Gcc - c plusone.c
Clean:
	Rm. a.out; rm plusone.o
Test: test1 
	Echo “done testing”
Test1:
	./a.out  > dumpfile; diff dumpfile test0 example | wc
