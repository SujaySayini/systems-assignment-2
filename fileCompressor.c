#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <limits.h>
#include <libgen.h>
#include <dirent.h>

#define char_lower_bound 32
#define char_upper_bound 127

#define true 1
#define false 0

int string_equal(char *arg1, char *arg2)
{
    int a = 0;
    //We loop until both equal the null terminator
    //The two strings will differ either in character or length before
    //we hit an out of bounds
    while (arg1[a] != '\0' || arg2[a] != '\0')
    {
        if (arg1[a] != arg2[a]){
            return false;
        }
        a++;
    }
    return true;
}

struct token_freq
{
    int count;
    char *token;
    struct token_freq *next;
};

struct token_table_info
{
    int tokens;
};

int freq_hash_function(char first_char)
{
    return ((int)first_char) - char_lower_bound;
}

void freq_hash_increment_token(struct token_table_info *info, struct token_freq **table, char *token)
{
    int hash = freq_hash_function(token[0]);
    struct token_freq *LLNode = table[hash];
    struct token_freq *holder;
    struct token_freq *parent;
    if (LLNode == NULL)
    {
        holder = malloc(sizeof(struct token_freq *));
        holder->token = "";
        table[hash] = holder;
        holder->next = malloc(sizeof(struct token_freq *));
        holder->next->token = token;
        holder->next->count = 1;
        info->tokens = info->tokens + 1;
        return;
    }
    while (LLNode != NULL)
    {
        if (string_equal(LLNode->token, token))
        {
            LLNode->count = LLNode->count + 1;
            return;
        }
        parent = LLNode;
        LLNode = LLNode->next;
    }
    parent->next = malloc(sizeof(struct token_freq *));
    parent->next->count = 1;
    parent->next->token = token;
    info->tokens = info->tokens + 1;
}

void print_hash_table(struct token_freq **table, int length)
{
    int a;
    for (a = 0; a < length; a++)
    {
        if (table[a] != NULL)
        {
            struct token_freq *current = table[a]->next;
            while (current != NULL)
            {
                printf("%s %i token \n", current->token, current->count);
                current = current->next;
            }
        }
    }
}

void string_create(char **buffer, char *start, int begin, int amount_to_copy)
{
    int a;
    *buffer = (char *)malloc((amount_to_copy + 1) * sizeof(char));
    char *buffer_to_change = *buffer;
    for (a = 0; a < amount_to_copy; a++)
    {
        buffer_to_change[a] = start[begin + a];
    }
    buffer_to_change[a] = '\0';
}

void populate_frequency_table(struct token_table_info *table_info, struct token_freq **freq_table, char *buffer, int buffer_l)
{
    int i;
    int string_start = 0;
    for (i = 0; i <= buffer_l; i++)
    {
        if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t' || i == (buffer_l))
        
        {
            int length = (i - string_start);

            char *working_token;
            string_create(&working_token, buffer, string_start, length);
            if(i != string_start){
                freq_hash_increment_token(table_info, freq_table, working_token);
            }
            if (buffer[i] == '\n')
            {
                freq_hash_increment_token(table_info, freq_table, "$n");
            }
            else if (buffer[i] == '\t')
            {
                freq_hash_increment_token(table_info, freq_table, "$t");
            }
            else
            {
                freq_hash_increment_token(table_info, freq_table, "$s");
            }
            i++;
            string_start = i;
        }
    }
}

struct mHeapNode
{
    int count;
    int isSubTree;
    void *data;
    void *data2;
};

struct mHeapInfo
{
    int length;
    int lastnode;
};

void print_heap(struct mHeapInfo *info,struct mHeapNode ** heap){
    int b;
        for (b = 0; b < info->length; b++)
        {
            if (heap[b] != NULL)
            {
                printf("%s %d %d| ", (char *)heap[b]->data,heap[b]->count,b);
            }
        }
        printf("\n");
}

struct mHeapNode* removemHeap(struct mHeapInfo *info,struct mHeapNode ** heap){
    struct mHeapNode* toReturn = heap[0];
    heap[0]=heap[info->lastnode];
    heap[info->lastnode] = NULL;
    int a;
    info->lastnode = info->lastnode - 1;
    
    int i = 0;
    int left_node = 2 * i + 1;
    int right_node = 2 * i + 2;
    while(((left_node <= info->lastnode) && heap[left_node]->count < heap[i]->count) || ((right_node <= info->lastnode) && heap[right_node]->count < heap[i]->count)){
        if(right_node > info->lastnode || heap[left_node]->count <= heap[right_node]->count){
            if(heap[left_node]->count < heap[i]->count){
                struct mHeapNode* holder = heap[left_node];
                heap[left_node] = heap[i];
                heap[i] = holder;
                i = left_node;
                left_node = 2 * i + 1;
                right_node = 2 * i + 2;
            }
        }
        
        else if(heap[right_node]->count < heap[left_node]->count){
            if(heap[right_node]->count < heap[i]->count){
                struct mHeapNode* holder = heap[right_node];
                heap[right_node] = heap[i];
                heap[i] = holder;
                i = right_node;
                left_node = 2 * i + 1;
                right_node = 2 * i + 2;
            }
        }
    }
    
    return toReturn;
    
}

void insertmHeap(struct mHeapInfo *info, struct mHeapNode **heap, struct mHeapNode *node)
{   
    int a;
    for (a = 0; a < info->length; a++)
    {
        if (heap[a] == NULL)
        {
            heap[a] = node;
            int i = a;
            info->lastnode = a;
            if (a != 0)
            {
                struct mHeapNode *parent = heap[(a - 1) / 2];
                struct mHeapNode *current = heap[a];
                while (parent->count > current->count)
                {
                    struct mHeapNode *holder = parent;
                    heap[(i-1)/2] = current;
                    heap[i] = holder;
                    i = ((i - 1 )/ 2);
                    parent = heap[(i-1)/2];
                    current = heap[i];

                }
            }

            break;
        }
    }
}

int power(int base,int exp){
    int tracker = 1;
    int a;
    for(a = 0;a<exp;a++){
        tracker = tracker  * base;
    }
    return tracker;
}

struct huff_encoding{
    int encoding;
    char * token;
    struct huff_encoding* next;
};


int encoding_hash(char* a){
    return ((int)a[0]) - char_lower_bound;
}

void encoding_insert_token(struct huff_encoding **table, struct huff_encoding* toinsert)
{   
    int hash = encoding_hash(toinsert->token);
    struct huff_encoding *LLNode = table[hash];
    struct huff_encoding * holder;
    struct huff_encoding * parent;
    if (LLNode == NULL)
    {
        holder = malloc(sizeof(struct huff_encoding *));
        holder->token = "";
        table[hash] = holder;
        holder->next = malloc(sizeof(struct huff_encoding *));
        holder->next->token = toinsert->token;
        holder->next->encoding = toinsert->encoding;
        return;
    }
    while (LLNode != NULL)
    {
        parent = LLNode;
        LLNode = LLNode->next;
    }
    parent->next = malloc(sizeof(struct huff_encoding*));
    parent->next->token = toinsert->token;
    parent->next->encoding = toinsert->encoding;
}

int getEncoding(struct huff_encoding** table,char * token){
    int hash = encoding_hash(token);
    struct huff_encoding* current = table[hash];
    while(current != NULL){
        if(string_equal(token,current->token)){
            return current->encoding;
        }
        current = current->next;
    }
    return -1;
}

void traverse(struct mHeapNode* node,int h,int prefix){
    
    if(!node->isSubTree){
        int fd = open("./HuffmanCodebook", O_WRONLY|O_APPEND, 0744); // codebook
        char * token = (char *)node->data;
        struct huff_encoding to_add;
        to_add.encoding = prefix;
        to_add.token = token;
        int a;
        for(a = (h - 1);a >= 0;a--){
            int towrite = (prefix/(power(10,a)) + 48);
            write(fd, &towrite,1);
            prefix = prefix % power(10,a);
            
        }
        
        write(fd,"\t",1);
        write(fd,token,strlen(token));
        write(fd,"\n",1);
    }    
    else{
        traverse((struct mHeapNode*)node->data,h+1,prefix * 10);
        traverse((struct mHeapNode*)node->data2,h+1,prefix * 10 + 1);
    }
}

void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

void insert_freq_nodes_into_heap(struct mHeapInfo *info, struct mHeapNode **heap, struct token_freq **freq_table, struct token_table_info* freq_table_info, int ascii_character_span)
{
    print_hash_table(freq_table,ascii_character_span);
    int a;
    for (a = 0; a < ascii_character_span; a++)
    {

        if (freq_table[a] != NULL)
        { 
           
            struct token_freq *current = freq_table[a]->next;
            
            while (current != NULL)
            {
                info->length = freq_table_info->tokens;
                struct mHeapNode *toInsert = (struct mHeapNode *)malloc(sizeof(struct mHeapNode *));
                toInsert->count = current->count;
                toInsert->isSubTree = false;
                toInsert->data = current->token;
                current = current->next;
                insertmHeap(info, heap, toInsert);
            }
            
            
        }
        
        
    }
    
}

int r_file(char * path,char** buff){
    struct stat stats;
    int read_status = 0;
    stat(path,&stats);
    int bytesReadSoFar = 0, numOfBytes = stats.st_size;
    *buff = (char*)malloc(sizeof(char*) * stats.st_size);
    int fileD = open(path, O_RDONLY); 
    do
    {
        read_status = read(fileD, *(buff + bytesReadSoFar), numOfBytes - bytesReadSoFar);
        bytesReadSoFar += read_status;

    } while (read_status > 0 && bytesReadSoFar < numOfBytes);  
    if(read_status > 0)
        return stats.st_size;
    return read_status;
        
}


void do_encoding(char* huffman,struct huff_encoding** encoder)
{
    char* buffer;
    int size = r_file(huffman, &buffer);
    struct huff_encoding* to_ins = (struct huff_encoding*) malloc(sizeof(struct huff_encoding*));
    
    if (size < 0)
    {
        //Error
    }
    int start;
    int tracker1 = 2;
    int tracker2 = 2;
    int a;
    while (tracker2 < size)
    {   
        if (buffer[tracker2] == '\t')
        {
            int b;
            int encoded = 0;
            for (b = tracker1; b < tracker2; b++)
            {
                encoded = encoded * 10;
                encoded = encoded + ((int)buffer[b] - 48);
            }
            to_ins->encoding = encoded;
            tracker2++;
            tracker1 = tracker2;
        }
        else if (buffer[tracker2] == '\n')
        {
            char *s;
            string_create(&s, buffer, tracker1, tracker2 - (tracker1));
            to_ins->token = s;
        
            encoding_insert_token(encoder, to_ins);
            tracker2++;
            tracker1 = tracker2;
        }
        else
        {
            tracker2++;
        }
    }
    
}

void compress_file(char *path, struct huff_encoding **encoder)
{
    char *new_name = (char *)malloc((strlen(path) + 4) * sizeof(char));
    strcat(new_name, path);
    strcat(new_name, ".hcz");
    int fd = open(new_name, O_WRONLY | O_CREAT | O_TRUNC, 0744);
    char *file_c;
    int buff_l = r_file(path, &file_c);
    if (buff_l < 0)
    {
        //error
    }

    int i;
    int string_start = 0;
    for (i = 0; i < buff_l; i++)
    {

        if (file_c[i] == ' ' || file_c[i] == '\n' || file_c[i] == '\t')

        {
            int length = (i - string_start);

            char *working_token;
            string_create(&working_token, file_c, string_start, length);
            if (i != string_start)
            {
                char *x = (char *)malloc(sizeof(char) * 20);
                itoa(getEncoding(encoder, working_token), x);
                write(fd, x, strlen(x));
            }
            if (file_c[i] == '\n')
            {
                char *x = (char *)malloc(sizeof(char) * 20);
                itoa(getEncoding(encoder, "$n"), x);
                write(fd, x, strlen(x));
            }
            if (file_c[i] == '\t')
            {
                char *x = (char *)malloc(sizeof(char) * 20);
                itoa(getEncoding(encoder, "$t"), x);
                write(fd, x, strlen(x));
            }
            else
            {
                char *x = (char *)malloc(sizeof(char) * 20);
                itoa(getEncoding(encoder, "$s"), x);
                write(fd, x, strlen(x));
            }
            i++;
            string_start = i;
        }
    }
}

int recursiveDirectories(char* name, char flag, struct huff_encoding** encoder){
    char* path = malloc(strlen(name) + 30);
    DIR* directory = opendir(name);
    if(directory == NULL){
        printf("Directory doesn't exist\n");
        closedir(directory);
        return -1;
    }

    struct dirent* currentElement = NULL;
    readdir(directory);
    readdir(directory);

    currentElement = readdir(directory);

    while(currentElement != NULL){
        //struct stat stats;
        //int status = stat(currentElement->d_name, &stats);

        // if(status != 0){
        //     printf("Error couldn't read into the path.\n");
        //     return -1;
        // }
        //if(S_ISREG(stats.st_mode)){ // if its a file
        if(currentElement->d_type == 8){
            if(flag == 'c'){
                char * holder = (char *) malloc((strlen(name) + strlen(currentElement->d_name) + 1) * sizeof(char));
                strcat(holder,name);
                strcat(holder,"/");
                strcat(holder,currentElement->d_name);
                char * d_name = currentElement -> d_name;
                int length = strlen(d_name);
                if(length > 4 && d_name[length - 4] == '.' && d_name[length - 3] == 't' && d_name[length - 2] == 'x' && d_name[length - 1] == 't'){
                    compress_file(holder,encoder);
                }
            }
            if(flag == 'b'){

            }

        }
        //if(S_ISDIR(stats.st_mode)){ // if its directory
        if(currentElement->d_type == 4){
            strcpy(path, name);
            strcat(path, currentElement->d_name);
            //printf("path is %s\n", path);
            recursiveDirectories(path, flag,encoder);
        }
        currentElement = readdir(directory);
    }
    free(path);
    closedir(directory);
    return 0;
}

int recursiveDirectoriesFreqTable(char* name, char flag, struct token_table_info *freq_table_info,struct token_freq** freq_table){
    char* path = malloc(strlen(name) + 30);
    DIR* directory = opendir(name);
    if(directory == NULL){
        printf("Directory doesn't exist\n");
        closedir(directory);
        return -1;
    }

    struct dirent* currentElement = NULL;
    readdir(directory);
    readdir(directory);

    currentElement = readdir(directory);

    while(currentElement != NULL){
        //struct stat stats;
        //int status = stat(currentElement->d_name, &stats);

        // if(status != 0){
        //     printf("Error couldn't read into the path.\n");
        //     return -1;
        // }
        //if(S_ISREG(stats.st_mode)){ // if its a file
        if(currentElement->d_type == 8){
            if(flag == 'b'){
                
                char * holder = (char *) malloc((strlen(name) + strlen(currentElement->d_name) + 1) * sizeof(char));
                
                strcat(holder,name);
                strcat(holder,"/");
                strcat(holder,currentElement->d_name);
                char * buff;
                
                int size = r_file(holder,&buff);
                if(size < 0){
                    //error
                }
                
                populate_frequency_table(freq_table_info, freq_table, buff, size);
                
            }
        }
        //if(S_ISDIR(stats.st_mode)){ // if its directory
        if(currentElement->d_type == 4){
            strcpy(path, name);
            strcat(path, currentElement->d_name);
            //printf("path is %s\n", path);
            recursiveDirectoriesFreqTable(path, flag,freq_table_info,freq_table);
        }
        currentElement = readdir(directory);
    }
    free(path);
    closedir(directory);
    return 0;
}

int main(int argc, char **argv)
{
    int ascii_character_span = char_upper_bound - char_lower_bound;
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


        if(flag == 'b'){
            char* name = argv[2];
            int fd = open("./HuffmanCodebook", O_WRONLY|O_CREAT | O_TRUNC, 0744); // codebook
            write(fd,"$\n",2);
            
            char *buffer;
            int size;

            struct token_freq **freq_table = (struct token_freq **)malloc((sizeof(struct token_freq *) * ascii_character_span));
            struct token_table_info *freq_table_info = (struct token_table_info *)malloc(sizeof(struct token_table_info *));
            
            if(ifrecursive){
                recursiveDirectoriesFreqTable(argv[3],flag,freq_table_info,freq_table);
            }
            else{
                size = r_file(name,&buffer);
                if(size < 0){
                    //Error
                }
                populate_frequency_table(freq_table_info, freq_table, buffer, size);
            }
            
            int number_of_tokens = freq_table_info->tokens;
            struct mHeapNode **heap = (struct mHeapNode **)calloc(sizeof(struct mHeap **),sizeof(struct mHeap **) * number_of_tokens);
            struct mHeapInfo* info = (struct mHeapInfo*) malloc(sizeof(struct mHeapInfo*));
            insert_freq_nodes_into_heap(info,heap,freq_table,freq_table_info,ascii_character_span);
            print_heap(info,heap);
            
            while(info->lastnode >= 1){
                struct mHeapNode* j = removemHeap(info,heap);
                
                struct mHeapNode* k = removemHeap(info,heap);
                
                int combined = j->count + k->count;
                
                struct mHeapNode *new = (struct mHeapNode *)malloc(sizeof(struct mHeapNode *));
                new->count = combined;
                new->isSubTree = true;
                if(j->count > k->count){
                    new->data = k;
                    new->data2 = j;
                }
                else{
                    new->data = j;
                    new->data2 = k;
                }
                insertmHeap(info,heap,new);
                
               
            }
            print_heap(info,heap);
            struct mHeapNode* root = removemHeap(info,heap);

            traverse(root,0,0);
            
         
        }
         else if(flag == 'c'){
            struct huff_encoding** encoder = malloc(sizeof(struct huff_encoding*) * ascii_character_span);
                        
            do_encoding("./HuffmanCodebook",encoder);
            if(ifrecursive){
                recursiveDirectories(argv[3], flag, encoder);
            }
            else{
                char* name = argv[2];
                printf("%s\n",name);
                compress_file(name,encoder);
            }
            
            

         }
         else if (flag == 'd'){
            int fd = open("./HuffmanCodebook", O_RDONLY, 0744);
            if(fd < 0){
                printf("File doesn't exist\n");
                return -1;
            }
            char c[3]; // dummy files 
            read(fd, c,2);
            char* buffer;
            int size = r_file("./HuffmanCodebook",&buffer);
            if(size < 0){
                //Error
            }


            struct token_freq **freq_table = (struct token_freq **)malloc((sizeof(struct token_freq *) * ascii_character_span));
            struct token_table_info *freq_table_info = (struct token_table_info *)malloc(sizeof(struct token_table_info *));
            populate_frequency_table(freq_table_info, freq_table, buffer, size);
            //void print_hash_table(struct token_freq **table, int length)
            print_hash_table(freq_table,6);


            int length = strlen(argv[2]);

            if(!(argv[2][length-1] == 'z' && argv[2][length-2] == 'c'  && argv[2][length-3] == 'h')){
                printf("Incorrect File.\n");
                return -1;
            }

            int fd2 = open(argv[2], O_RDONLY, 0744);
            if(fd2 < 0){
                printf("File doesn't exist.\n");
                return -1;
            }



            int fd3 = open("./HuffmanCodebook", O_WRONLY | O_CREAT, 0744);




        }
    }
