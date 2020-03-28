#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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
	struct HuffmanCode* zero;
	struct HuffmanCode* one;
} HuffmanCode ;

int main(){
	char * token[] = {"this", "is", "an", "example", "EOF", "this", "is" "EOF"};
	int num_token = 8;
	int num_unique_tokens = 5;
	//filling frequencies
	HuffmanCode** node_array = malloc(sizeof(HuffmanCode*)*num_unique_tokens);
    
    
    HuffmanCode** frequency_array = malloc(sizeof(HuffmanCode*)*num_unique_tokens);
	for (int i=0; i <num_unique_tokens; i++){
	frequency_array[i] = NULL;
	}
	print_frequency_array(frequency_array, num_unique_tokens);
	// populate frequency array
	for(int i=0; i <num_token; i++){
		add_token_to_frequency_array(frequency_array, num_unique_tokens, token[i]);
		print_frequency_array(frequency_array, num_unique_tokens);
	}

	//Start Huffman Coding here
	
}