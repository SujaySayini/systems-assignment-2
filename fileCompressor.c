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

int string_equal(char* arg1,char* arg2){
    int a = 0;
    //We loop until both equal the null terminator
    //The two strings will differ either in character or length before
    //we hit an out of bounds
    while(arg1[a] != '\0' || arg2[a] != '\0'){
        if(arg1[a] != arg2[a])
            return 0;
        a++;
    }
    return 1;
}

// int count_in_use(HuffmanCode** frequency_array, int array_size){
// 	int count_in_use = 0;

// 	for(int i = 0; i< array_size ;i++){
// 		if(frequency_array[i] != NULL){
// 			count_in_use++;
// 		}
// 	}
// 	return count_in_use;
// }

typedef struct {
	char* token; //“the”
	int count; //“30
	struct HuffmanCode* left;
	struct HuffmanCode* right;
} HuffmanCode ;

typedef struct Node{ 
    char* data;
    int count; 
    struct Node* next; 
} Node; 

void insert(Node** head, char* name){
   Node* current = *head;
    while(current != NULL){ // if the name already exists, just increment 
        if(strcmp(current->data, name) == 0){
            current->count++;
            return;
        }
        current = current->next;
    }

    Node* ptr = (Node*) malloc(sizeof(Node));
    ptr->count = 1;
    ptr->data = (char*) malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(ptr->data, name);
    ptr->next = (*head);
    (*head) = ptr;
}

void printList(Node* head) {
    Node* n = head; 
    while (n != NULL){ 
        printf("-%s- , -%d-\n", n->data, n->count); 
        n = n->next; 
    } 
} 
int sizeOfList(Node* n) { 
    int counter = 0;
    while (n != NULL){ 
        counter++;
        n = n->next; 
    } 
    return counter;
}

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
        char c = ' ';
        int counter = 0;
        char buffer[100];
        Node* head = NULL;
        do {
            read_status = read(fileD, &c, 1);
            if(read_status == 0){ // reached the end of the file
                break;
            }

            if(c == '\n' ||c == ' ') {
                
                buffer[counter] = '\0';

                insert(&head,buffer);

                char buffer2[2] = {};
                if(c == '\n'){
                    buffer2[0] = '$';
                }else{
                    buffer2[0] = c;
                }
                buffer2[1] = '\0';
                insert(&head,buffer2);

                memset(buffer, '\0', 100);
                counter = 0;
            } else {
                buffer[counter] = c;
                counter++;
            }

        }while(read_status > 0);

        buffer[counter] = '\0';
        insert(&head,buffer);

        printList(head);
        int total = sizeOfList(head);
        printf("Total number of nodes in array is %d\n", total); 

        //Start huffman Coding
        
        // int i;
        // for(i = 0; i < ;i++){
        //     if(buffer[i] == ' ' || buffer[i] == '\n'){

        //     } else {

        //     }
        // }

    } else if(string_equal(argv[1 + ifrecursive],"-c")){ // compress
        flag = 'c';


    } else if(string_equal(argv[1 + ifrecursive],"-d")){ // decompress
        flag = 'd';


    } else { //Error
        printf("Incorrect input, please try again");
        return -1;
    }
}