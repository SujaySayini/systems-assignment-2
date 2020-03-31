#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>  

int count_in_use(HuffmanCode** frequency_array, int array_size){
	int count_in_use = 0;

	for(int i = 0; i< array_size ;i++){
		if(frequency_array[i] != NULL){
			count_in_use++;
		}
	}
	return count_in_use;
}

typedef struct {
	char* token; //“the”
	int count; //“30
	struct HuffmanCode* left;
	struct HuffmanCode* right;
} HuffmanCode ;

int main(int argc, char** argv) { 

    int ifrecursive = 0;
    char flag;

    if(string_equal(argv[1],"-R")){ // Recursive
        ifrecursive = 1;
    }   
    int fileD = open(argv[2 + ifrecursive],O_RDONLY); // file or path
    int fd = open("./HuffmanCodebook",O_WRONLY | O_CREAT, 0744); // codebook
    
    if(string_equal(argv[1 + ifrecursive],"-b")){ //build codebook
        flag = 'b';

        int read_status = 0;
        char *buffer = (char *) calloc(0100, sizeof(char)); 
        int bytesReadSoFar, numOfBytes = 1000;
        
        //read_status = read(fileD, buffer, 100);
        
        do {
            read_status = read(fileD, buffer+bytesReadSoFar, numOfBytes - bytesReadSoFar);
            bytesReadSoFar += read_status;

        }while(read_status > 0 && bytesReadSoFar < numOfBytes);

        int i;
        for(i = 0; i < bytesReadSoFar;i++){
            if(buffer[i] == ' ' || buffer[i] == '\n'){

            } else {

            }
        }



    } else if(string_equal(argv[1 + ifrecursive],"-c")){ // compress
        flag = 'c';


    } else if(string_equal(argv[1 + ifrecursive],"-d")){ // decompress
        flag = 'd';


    } else { //Error
        printf("Incorrect input, please try again");
        return -1;
    }
    
}