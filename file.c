#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
 
typedef struct LLNode{//node for linked list that contains all tokens read in
char* tok;
int fre;//frequency
struct LLNode* next;
 
} LLNode;
 
typedef struct MHNode{//min heap node for huffman
char* tok;
int data;
struct MHNode * left;
struct MHNode * right;
 
} MHNode;
 
typedef struct MHeap{
int num;//size
int max;//cap
struct MHNode ** arry;
 
} MHeap;
//Helper
 
void swap(MHNode ** first, MHNode ** second){
MHNode * temp = *first;
*first = *second;
*second = temp;
 
}
 
//Heapify, turn tree into min heap
void heapify(MHeap * heap, int x){
int qwe = x;
int l = 2*x+1;
int r = 2*x+2;
 
if(l < heap->num && heap->arry[l]->data < heap->arry[qwe]->data) qwe = l;
 
        if(r < heap->num && heap->arry[r]->data < heap->arry[qwe]->data) qwe = r;
if(qwe != x){//recur
swap(&heap->arry[qwe], &heap->arry[x]);
heapify(heap, qwe);
}
}
 
void buildHeap(MHeap * heap){
int qwe = heap->num-1;
int q = 0;
 
for(q = (qwe-1)/2; q >=0;q--){
heapify(heap,q);
 
}
}
 
 
void insert(MHeap * heap, MHNode* node){
int qwe =heap->num;
 
(heap->num)++;
while(qwe && node->data < heap->arry[(qwe-1)/2]->data){
heap->arry[qwe] = heap->arry[(qwe-1)/2];
qwe = (qwe-1)/2;
}
 
heap->arry[qwe] = node;
 
}
 
MHNode* getMin(MHeap* heap){
MHNode* qwe = heap->arry[0];
 
heap->arry[0] = heap->arry[heap->num-1];
(heap->num)--;
heapify(heap, 0);
 
return qwe;
}
 
//a is the number of nodes in head
MHeap * buildHeap_Inputs(LLNode* head, int a){
int i;
LLNode* ptr = head;
MHeap* heap = (MHeap*)malloc(sizeof(MHeap));
 
heap->num = 0;
heap->max = a;
heap->arry= (MHNode**)malloc(a*sizeof(MHNode*));
for(i = 0; i < a; i++){
MHNode * qwe = (MHNode*)malloc(sizeof(MHNode));
 
qwe->tok=ptr->tok;
qwe->data=ptr->fre;
qwe->left= NULL;
qwe->right = NULL;
 
heap->arry[i] = qwe;
ptr= ptr->next;
}
 
heap->num = a;
buildHeap(heap);
return heap;
 
}
 
//-------------------------------------
 
int oscount=2;
void createCodebook(MHNode* head, int store[], int a){//codebook recursion
  int fd = open("./HuffmanCodebook",O_RDWR | O_CREAT, 00700);
 
char* cstore = (char*)malloc((a+1)*sizeof(char*));
int i = 0;
  if(head->left){//not NULL
    store[a] = 0;
    createCodebook(head->left, store, a+1);
  }
  if(head->right){//not NULL
    store[a] = 1;
    createCodebook(head->right, store,a+1);
  }
 
  if(!(head->left) && !(head->right)){//leaf node
 
 
for(i = 0; i<a; i++){
cstore[i] = (char)(store[i]+48);
 
 
}
cstore[i] = '\0';
 
   lseek(fd, oscount, SEEK_CUR);
 
    write(fd, cstore, a);
    write(fd, "\t", 1);
    write(fd, head->tok, strlen(head->tok));
    write(fd, "\n", 1);
 
    oscount += (a+2+strlen(head->tok));
 
 
  }
 
}
 
 
void doHuffman(LLNode* head, int a){
//printf("gate");
 
  MHNode* rt = NULL;
  MHNode* lt = NULL;
  MHNode* huff = (MHNode*)malloc(sizeof(MHNode));
  int qwe = 0,wer = 0;
//printf("gate");
 
  MHeap* heap = buildHeap_Inputs(head,a);
//  printf("gate");
if(heap->num != 1) qwe = 1;
 
 
  while(qwe == 1){
    lt= getMin(heap);
 
    rt = getMin(heap);
//  printf("gateloop");
 
   wer = lt->data + rt->data;
//qwe = 0;
 
 
    MHNode* temp = (MHNode*)malloc(sizeof(MHNode));
 
    temp->tok = (char*)' ';
    temp->data=wer;
    temp->left=lt;
    temp->right = rt;
   // printf("2\n");
 
    insert(heap, temp);
   // printf("3\n");
 
    if(heap->num == 1) qwe = 0;
   // printf("4, %d\n", qwe);
 
  }
 
  huff = getMin(heap);
 
    oscount = 2;  
    int fd = open("./HuffmanCodebook",O_RDWR | O_CREAT, 0744);
    int obama =write(fd, "$", 1);
    write(fd, "\n", 1);
    int temp2[a];
    createCodebook(huff, temp2, 0);
 
}
 
//------------------------------------------------------------------
void compress (DIR* directory, char* codebook, int recur) {
  struct dirent* file;
  int i;
  char temp[5];
  temp[4] = '\0';
 
  while ((file = readdir(directory))) {
    //printf("%s\n", file->d_name);
    if (recur == 1) {
      if (file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0  && strcmp(file->d_name, "..") != 0) {
        DIR* t = opendir(file->d_name);
        char path[4096];
        strcpy(path, file->d_name);
        strcat(path, "/");
        int change = chdir(path);
        compress(t, codebook, recur);
        change = chdir("..");
      }
    }
    if (strlen(file->d_name) > 4) {
      for (i = 0; i < 4; i++) {
        temp[i] = file->d_name[strlen(file->d_name) - 4 + i];
      }
      if (strcmp(temp, ".txt") == 0) {
        int fd = open(file->d_name, O_RDONLY);
 
        char* name = (char*)malloc(sizeof(char) * (strlen(file->d_name) + 4));
        char ext[] = {'.', 'h', 'c', 'z'};
        for (i = 0; i < strlen(file->d_name); i++) {
          name[i] = file->d_name[i];
        }
        for (i = 0; i < 4; i++) {
          name[strlen(file->d_name) + i] = ext[i];
        }
        printf("%s\n", name);
        int fdnew = open(name, O_RDWR | O_CREAT);
      }
    }
  }
  return;
}
 
void decompress (DIR* directory, char* codebook, int recur) {
  struct dirent* file;
  int i;
  char temp[9];
  temp[8] = '\0';
 
  while ((file = readdir(directory))) {
    //printf("%s\n", file->d_name);
    if (recur == 1) {
      if (file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0  && strcmp(file->d_name, "..") != 0) {
        DIR* t = opendir(file->d_name);
        char path[4096];
        strcpy(path, file->d_name);
        strcat(path, "/");
        int change = chdir(path);
        decompress(t, codebook, recur);
        change = chdir("..");
      }
    }
    if (strlen(file->d_name) > 8) {
      for (i = 0; i < 8; i++) {
        temp[i] = file->d_name[strlen(file->d_name) - 8 + i];
      }
      if (strcmp(temp, ".txt.hcz") == 0) {
        int fd = open(file->d_name, O_RDONLY);
        char* name = (char*)malloc(sizeof(char) * (strlen(file->d_name) - 4));
        for (i = 0; i < strlen(file->d_name) - 4; i++) {
          name[i] = file->d_name[i];
        }
        printf("%s\n", name);
        int fdnew = open(name, O_RDWR | O_CREAT);
      }
    }
  }
  return;
}
 
//-----------------------------------------------------------------------------------------
LLNode* LLinsert (LLNode* head, char* token, int size) {
 
  int i;
  if (head->tok == NULL) {
    head->tok = (char*)malloc(sizeof(token));
    strcpy(head->tok, token);
    head->fre = 1;
    head->next = NULL;
 
    return head;
  }
  LLNode* temp = (LLNode*) malloc(sizeof(LLNode*));
  temp->next = (LLNode*) malloc(sizeof(LLNode*));
  temp->next->tok = NULL;
  temp->tok = (char*)malloc(sizeof(token));
  strcpy(temp->tok, token);
  temp->fre = 1;
  LLNode* ptr = head;
  for(i = 0; i < size - 1; i++) {
    ptr = ptr->next;
  }
  ptr->next = temp;
  ptr = head;
  return head;
}
 
int tokenize (char* path, LLNode* head, int size) {
  int i;
 
  int file = open(path, O_RDONLY);
  int counter = open(path, O_RDONLY);
 
  char c, temp, term;
  int count;
  int repeat = 0;
 
  while (read(counter, &c, 1) == 1) {
    read(file, &temp, 1);
 
    count = 1;
    char temp1[] = {'$', '\0'};
    while (read(counter, &term, 1) == 1) {
if (term == ' ') {
        temp1[1] = 's';
        break;
      } else if (term == '\n') {
        temp1[1] = 'n';
        break;
      } else if (term == '\t') {
        temp1[1] = 't';
        break;
      } else if (term == '\0') {
        temp1[1] = '0';
        break;
      }
count++;
}
 
 
char* rest = (char*)malloc(sizeof(char) * count);
    for (i = 0; i < count; i++) rest[i] = '\0';
 
for (i = 0; i < count; i++) {
read(file, &temp, 1);
if (temp == ' ' || temp == ',' || temp == '\n' || temp == '\0') break;
rest[i] = temp;
}
 
char* result = (char*)malloc((sizeof(char*) * count) + sizeof(char));
result[0] = c;
strcat(result, rest);
    for (i = count; i < strlen(result); i++) result[i] = '\0';
    LLNode* temp = (LLNode*) malloc(sizeof(LLNode*));
    LLNode* ptr = head;
    repeat = 0;
 
 
    for(i = 0; i < size; i++) {
      if (strcmp(ptr->tok, result) == 0) {
        ptr->fre ++;
        repeat = 1;
        break;
      }
      if (ptr->next == NULL) break;
      ptr = ptr->next;
    }
    if (repeat == 0) {
      head = LLinsert(head, result, size);
      size++;
    }
 
    ptr = head;
    repeat = 0;
    for(i = 0; i < size; i++) {
      if (strcmp(temp1, ptr->tok) == 0) {
        ptr->fre ++;
        repeat = 1;
        break;
      }
      if (ptr->next == NULL) break;
      ptr = ptr->next;
    }
    if (repeat == 0) {
      head = LLinsert(head, temp1, size);
      size++;
    }
 
 
  }
 
 
  int clo = close(file);
  clo = close(counter);
  return size;
}
 
int create (DIR* directory, int recur, LLNode* head, int size) {
  struct dirent* file;
 
  char temp[5];
  temp[4] = '\0';
 
  LLNode* ptr = head;
 
  int i;
 
  while ((file = readdir(directory))) {
    if (recur == 1) {
      if (file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0  && strcmp(file->d_name, "..") != 0) {
        DIR* t = opendir(file->d_name);
        char path[4096];
        strcpy(path, file->d_name);
        strcat(path, "/");
        int change = chdir(path);
        size = create(t, recur, head, size);
        change = chdir("..");
      }
    }
    if (strlen(file->d_name) > 4) {
      for (i = 0; i < 4; i++) {
        temp[i] = file->d_name[strlen(file->d_name) - 4 + i];
      }
      if (strcmp(temp, ".txt") == 0) {
        size = tokenize(file->d_name, head, size);
      }
    }
  }
 
  return size;
}
 
int main(int argc, char** argv){
 
DIR* directory;
  directory = opendir(argv[3]);
int recur = 0;
  char* flag;
  char* codebook;
 
 
  if (strcmp(argv[1], "-R") == 0) {
    recur = 1;
    flag = argv[2];
    directory = opendir(argv[3]);
    if (strcmp(flag, "-c") == 0 || strcmp(flag, "-d")) {
      codebook = argv[4];
    }
  } else {
    flag = argv[1];
    directory = opendir(argv[2]);
    if (strcmp(flag, "-c") == 0 || strcmp(flag, "-d")) {
      codebook = argv[3];
    }
  }
 
 
 
  if (directory == NULL) {
    printf("Error: could not open directory\n");
    return 0;
  }
 
  if (strcmp(flag, "-b") == 0) {
    LLNode* head = (LLNode*)malloc(sizeof(LLNode));
    head->tok = NULL;
    int size = 0;
    size = create(directory, recur, head, size);
    //printf("%d\n", size);
    LLNode* ptr = head;
    int i;
    for (i = 0; i < size; i++) {
      printf("%s -- %d\n", ptr->tok, ptr->fre);
      ptr = ptr->next;
    }
    doHuffman(head, size);
 
  } else if (strcmp(flag, "-c") == 0) {
    compress(directory, codebook, recur);
  } else if (strcmp(flag, "-d") == 0) {
 
  }
 
 
 
        int clo = closedir(directory);
  return 0;
 
 
 
}