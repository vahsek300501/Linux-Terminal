#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

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

    int isIFlag = 0;
    int isdFlag = 0;
    int isFileFound = 0;
    char * fileName = NULL;

    for(int i=0;i<arrayLength;i++) {
        if(i == 0) {
            continue;
        }
        if(strcmp(argv[i],"-i") == 0){
            isIFlag = 1;
        }
        if(strcmp(argv[i],"-d") == 0) {
            isdFlag = 1;
        }
        if(strcmp(argv[i],"-i") != 0 && strcmp(argv[i],"-d") != 0 && strcmp(argv[i],"rm") != 0) {
            isFileFound = 1;
            fileName = argv[i];
        }
    }

    if(isFileFound == 0){
        printf("INSUFFICIENT ARGUMENTS\n");
        return;
    }

    if(isFileFound == 1) {
        if(isIFlag == 1) {
            printf("CONFIRM REMOVAL(y/n)   ");
            char inp[100];
            scanf("%s",inp);
            if(strcasecmp(inp,"y") != 0){
                return;
            }
        }

        DIR * d = opendir(fileName);
        if(isdFlag == 1) {
            if(d == NULL) {
                printf("CAN'T OPEN THE DIRECTORY\n");
                return;
            }
            int output = rmdir(fileName);
            if(output != 0) {
                printf("THE DIRECTORY IS NOT EMPTY\n");
                return;
            }
            closedir(d);
            return;
        }

        if(isdFlag == 0 && d!=NULL) {
            printf("CAN'T REMOVE DIRECTORY\n");
            return;
        }

        if(isdFlag == 0 && d == NULL) {
            FILE * delFile = fopen(fileName,"r");
            if(delFile == NULL) {
                printf("NO SUCH FILE EXISTS\n");
                return;
            }
            
            remove(fileName);
        }
    }

}