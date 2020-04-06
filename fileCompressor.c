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
    for (i = 0; i < buffer_l; i++)
    {
        
        if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t')
        
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


void encoding_insert_token(struct huff_encoding **table, struct huff_encoding* toinsert)
{
    int hash = ((int)toinsert->token[0]) - char_lower_bound;
    struct huff_encoding* LLNode = table[hash];
    
    if (LLNode == NULL)
    {
        table[hash] = toinsert;
        printf("%s\n",table[hash]->token);
        return;
    }
    else{
        printf("%s\n",table[hash]->token);
    }
    /*
    while (LLNode->next != NULL)
    {
        LLNode = LLNode->next;
    }
    LLNode->next = toinsert;
    */
}


void traverse(struct mHeapNode* node,int h,int prefix,struct huff_encoding ** table){
    
    if(!node->isSubTree){
        int fd = open("./HuffmanCodebook", O_WRONLY|O_APPEND, 0744); // codebook
        char * token = (char *)node->data;
        struct huff_encoding to_add;
        to_add.encoding = prefix;
        to_add.token = token;
        encoding_insert_token(table,&to_add);
        //;
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
        traverse((struct mHeapNode*)node->data,h+1,prefix * 10,table);
        traverse((struct mHeapNode*)node->data2,h+1,prefix * 10 + 1,table);
    }
}

void insert_freq_nodes_into_heap(struct mHeapInfo *info, struct mHeapNode **heap, struct token_freq **freq_table, struct token_table_info* freq_table_info, int ascii_character_span)
{
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

    if(ifrecursive == 1){

    } else {
        char* name = argv[2];
        flag = 'b';
                 // file or path
        int fd = open("./HuffmanCodebook", O_WRONLY|O_CREAT | O_TRUNC, 0744); // codebook
        write(fd,"$\n",2);
        
        char *buffer;
        int size = r_file(name,&buffer);
        if(size < 0){
            //Error
        }

        struct token_freq **freq_table = (struct token_freq **)malloc((sizeof(struct token_freq *) * ascii_character_span));
        struct token_table_info *freq_table_info = (struct token_table_info *)malloc(sizeof(struct token_table_info *));
        populate_frequency_table(freq_table_info, freq_table, buffer, size);

        int number_of_tokens = freq_table_info->tokens;
        struct mHeapNode **heap = (struct mHeapNode **)malloc(sizeof(struct mHeap **) * number_of_tokens);
        struct mHeapInfo* info = (struct mHeapInfo*) malloc(sizeof(struct mHeapInfo*));
        insert_freq_nodes_into_heap(info,heap,freq_table,freq_table_info,ascii_character_span);
        
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
        struct mHeapNode* root = removemHeap(info,heap);
        
        struct huff_encoding** encoder = malloc(sizeof(struct huff_encoding*) * ascii_character_span);

        traverse(root,0,0,encoder);
        
        }
}
