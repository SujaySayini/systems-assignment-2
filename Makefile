all: 
	gcc ./fileCompressor.c -o fileCompressor 
	
clean:
	rm fileCompressor

everything: clean 
	echo "all done"

