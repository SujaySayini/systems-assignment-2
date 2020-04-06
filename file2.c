

int check = 1;  
    int fd1 = 0;
    int fd2 = 0;
    int fd3 = 0;
    int sz = 0;
    char fileH[100];
   
    int fileCheck = strlen(file); // put this into compress
    if(file[fileCheck-1] == 'z'){
   //printf("%s is not an .txt file, cannot compress\n", file);
        return;
    }
    fd2 = open(file, O_RDWR);
    strcpy(fileH, file);
    strcat(fileH, ".hcz");
    printf("test.txt.hcz: %s\n", fileH);
    fd3 = open(fileH, O_RDWR | O_CREAT, 00700);
    memset(fileH, '\0', sizeof(fileH));
    char a;
    int i = 0; // for bytes
    int j = 0; // for strings
    int k = 0; // for second file fd2
    char word[1000];
    int result = 0;
    struct press* head = NULL;
    struct press* ptr = NULL;
    head = malloc(sizeof(struct press));
    ptr = head;
    if ((fd1 = open("HuffmanCodebook.txt", O_RDWR)) >= 0){
        while (read(fd1, &a, 1) == 1){
            //putchar(a);
            if(isdigit(a)){
                ptr->bytes[i] = a;
                i++;
            }
            else if(isalpha(a) || a == '<' || a == '>'){
                ptr->token[j] = a;
                j++;
            }
            else if(a == '\n'){
                 memset(ptr->bytes+i, '\0', 900);
                i = 0;
                j = 0;
                if(strcmp("<SPC>", ptr->token) == 0){
                    memset(ptr->token, '\0', sizeof(ptr->token));
                    ptr->token[0] = ' ';
                }
                else if(strcmp("<TAB>", ptr->token) == 0){
                    memset(ptr->token, '\0', sizeof(ptr->token));
                    ptr->token[0] = '\t';
                    //printf("Value of this string: %d\n", ptr->token[0]);
                }
                else if(strcmp("<LF>", ptr->token) == 0){
                    memset(ptr->token, '\0', sizeof(ptr->token));
                    ptr->token[0] = '\n';
                }
                ptr->next = malloc(sizeof(struct press));
                ptr = ptr->next;
            }
        }
    }
    if (fd2 >= 0){
        memset(word, '\0', sizeof(word));
        while (read(fd2, &a, 1) == 1){
            ptr = head;
            word[k] = a;
            k++;
            result = compressCompare(sz, fd3, ptr, word, check);
            if(result == 1){ // the byte was found.
                k = 0;
                memset(word, '\0', sizeof(word));
            }
        }
    }
    ptr = head;
    compressCompare(sz, fd3, ptr, word, check);
    memset(fileH, '\0', sizeof(fileH));
    close(fd1);
    close(fd2);
    close(fd3);