#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>

char * removeQuotes(char * arr){
    char * final = malloc(1024);
    int j = 0;
    if(arr[0] == '\"' || arr[0] == '\'') {
        for(int i=1;arr[i]!='\0';i++) {
            final[j] = arr[i];
            j++;
        }
        final[j] = '\0';
    }
    if(arr[strlen(arr)-1] == '\"' || arr[strlen(arr)-1] == '\'') {
        final[strlen(final)-1] = '\0';
    }
    else {
        final = arr;
    }
    return final;
}

char ** parseArray(char ** arr,int arrLen) {
    int pos = 0;

    for(int i=0;i<arrLen;i++) {
        char * tmp = removeQuotes(arr[i]);
        arr[pos] = tmp;
        pos++;
    }
    return arr;
}

void main(int argc, char ** argv) {
    int arrayLength = 0;
    for(int i=0;argv[i]!=NULL;i++) {
        arrayLength++;
    }

    argv = parseArray(argv,arrayLength);

    int isEFlagFound = 0;
    int isNFlagFound = 0;
    int isFileFound = 0;
    char * fileName = NULL;

    for(int i=0;argv[i]!= NULL; i++) {
        if(i == 0){
            continue;
        }
        if(strcmp(argv[i],"-E") == 0){
            isEFlagFound = 1;
        }
        if(strcmp(argv[i],"-n") == 0){
            isNFlagFound = 1;
        }
        if(strcmp(argv[i],"-E") != 0 && strcmp(argv[i],"-n") != 0 && strcmp(argv[i],"cat") != 0) {
            isFileFound = 1;
            fileName = argv[i];
        }
    }

    if(isFileFound == 0) {
        int count = 1;
        char arr[256];
        int isNextLineFound = 0;
        while(1>0) {
            fgets(arr,256,stdin);
            int arrLen = strlen(arr);
            if(arr[arrLen-1] = '\n') {
                arr[arrLen-1] = '\0';
                isNextLineFound = 1;
            }
            if(isNFlagFound == 1){
                printf("%d ",count);
                count++;
            }
            printf("%s",arr);
            if(isEFlagFound == 1){
                printf("%s","$");
            }
            if(isNextLineFound == 1){
                printf("%c",'\n');
                isNextLineFound = 0;
            }
        }
    }

    if(isFileFound == 1) {
        int count = 1;
        FILE * fin = fopen(fileName,"r");
        char arr[1024];
        int isNextLineFound = 0;
        if(fin == NULL) {
            printf("ERROR IN OPENING THE FILE\n");
            return;
        }

        while(fgets(arr,1024,fin)!=NULL) {
            int arrLen = strlen(arr);
            if(arr[arrLen-1] = '\n') {
                arr[arrLen-1] = '\0';
                isNextLineFound = 1;
            }
            if(isNFlagFound == 1){
                printf("%d ",count);
                count++;
            }
            printf("%s",arr);
        
            if(isEFlagFound == 1){
                printf("%s","$");
            }
            if(isNextLineFound == 1){
                printf("%c",'\n');
                isNextLineFound = 0;
            }
        }
    }
}