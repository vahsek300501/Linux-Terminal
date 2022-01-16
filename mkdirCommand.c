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

    int isPFlag = 0;
    int isVFlag = 0;
    int isFileFound = 0;
    char * fileName = NULL;

    for(int i=0;i<arrayLength;i++) {
        if(i == 0){
            continue;
        }
        if(strcmp(argv[i],"-p") == 0) {
            isPFlag = 1;
        }
        if(strcmp(argv[i],"-v") == 0){
            isVFlag = 1;
        }
        if(strcmp(argv[i],"-p") != 0 && strcmp(argv[i],"-v") != 0 && strcmp(argv[i],"mkdir") != 0) {
            isFileFound = 1;
            fileName = argv[i];
        }
    }

    if(isFileFound == 0) {
        printf("INVALID INPUT!!!\n");
        return;
    }

    if(isPFlag == 1) {
        int i = 0;
        if(fileName[i] == '/') {
            i++;
        }
        for(;fileName[i]!='\0';i++) {
            if(fileName[i] == '/') {
                fileName[i] = '\0';
                mkdir(fileName,0777);
                fileName[i] = '/';
 
            }
        }
        mkdir(fileName,0777);
    }

    if(isPFlag == 0) {
        struct stat s = {0};
        if(stat(fileName,&s) == 0){
            printf("THE FILE ALREADY EXISTS\n");
            return;
        }
        int i=0;
        if(fileName[i] == '/'){
            i++;
        }
        for(;fileName[i]!= '\0';i++) {
            if(fileName[i] == '/') {
                fileName[i] = '\0';
                int err = stat(fileName,&s);
                if (err == 0 && S_ISDIR(s.st_mode)) {
                    fileName[i] = '/';
                    continue;
                }
                else {
                    printf("ERROR AS ONE PART OF THE PATH DOESN'T EXISTS\n");
                    return;
                }
            }
        }

        if(stat(fileName,&s) == -1) {
            int tmp = mkdir(fileName,0777);
            if(isVFlag == 1){
                printf("THE FILE WAS CREATED %s\n",fileName);
            }
        }
        else {
            printf("CANNOT CREATE FILE\n");
        }
    }
     
}