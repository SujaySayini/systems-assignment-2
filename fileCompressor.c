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
#include <dirent.h>  

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

typedef struct HuffmanCode{
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
void build(int fileD){
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
                buffer2[0] = '*'; // * = new line
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
    //HuffmanCode* root = NULL;
}
void compress(int fileD, int codeBookFD){

}

void decompress(int fileD, int codeBookFD){

}

int selection(int ifrecursive, char* name, char flag){
    int fileD = open(name,O_RDONLY); // file
        if(!(fileD > 0)){
            printf("File doesn't Exist.\n");
            return -1;
        }
        int codeBookFD = open("./HuffmanCodebook",O_WRONLY | O_CREAT, 0744); // codebook

        if(flag == 'b'){ //build codebook
            build(fileD);
        } else if(flag == 'c'){ // compress
            compress(fileD, codeBookFD);
        } else if(flag == 'd'){ // decompress
            decompress(fileD, codeBookFD);
        }
    return 0;
}

int recursiveDirectories(char* name, int ifrecursive,char flag){
    DIR* directory = opendir(name);

    if(directory == NULL){
            printf("Directory doesn't exist\n");
            return -1;
        }
        
        struct dirent* currentElement = NULL;
        readdir(directory);
        readdir(directory);
        currentElement = readdir(directory);

        while(currentElement != NULL){
	        if(currentElement->d_type == DT_REG){ //if its a file
		        printf("File: %s\n", currentElement ->d_name);
                int result = selection(ifrecursive,currentElement ->d_name,flag);
                    if(result == -1){
                        return -1;
                    }
            } 
            if(currentElement->d_type == DT_DIR){ // if its a directory
	            printf("Directory: %s\n", currentElement ->d_name);
                int result = recursiveDirectories(currentElement ->d_name, ifrecursive, flag);
                if(result == -1){
                    return -1;
                }
            }
            currentElement = readdir(directory);
        }
    closedir(directory);
    return 0;
}

int main(int argc, char** argv) { 
 // ------------------------------------------------------------------------------------------------------
 // Flag Stuff
    int ifrecursive = 0, gotFlag = 0;
    char flag;
    int i = 0;
    for(i = 1;i < 3;i++){
        if(string_equal(argv[i],"-R")){
            if(ifrecursive == 0){
                ifrecursive = 1;
            }else{
                printf("Error, already entered Recursive\n");
                return -1;
            }
        }
        else if(string_equal(argv[i],"-b") || string_equal(argv[i],"-c") || string_equal(argv[i],"-d")){
            if(gotFlag == 0){
                flag = argv[i][1];
                gotFlag++; 
            }
            else{
                printf("Error, already entered a Flag\n");
                return -1;
            }
        } 
    }
    if(gotFlag == 0){
        printf("Please enter a flag");
        return -1;
    }

    // ------------------------------------------------------------------------------------------------------

    if(ifrecursive == 1){
        int result = recursiveDirectories(argv[3], ifrecursive, flag);
        if(result == -1){
            return -1;
        }

        // while(currentElement->d_type != DT_DIR && currentElement != NULL){
        //     printf("found dir: %s\n", currentElement ->d_name);
	    //     currentElement = readdir(directory);
        // }

        // int newpartLen = strlen(currentElement ->d_name);
        // printf("newPartlen = %d\n", newpartLen);
        // int pathLen = strlen(argv[3]);
        // printf("pathLen = %d\n", pathLen);
        // char* newPath = (char*)malloc(sizeof(char)* newpartLen+pathLen+2);
        // printf("newPath = %s\n", newPath);
    } else {
        char* name = argv[2];
        int result = selection(ifrecursive,name,flag);
        if(result == -1){
            return -1;
        }
    }    
    return 0;
}