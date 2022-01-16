#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

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

    int isUFlag = 0;
    int isRFlag = 0;
    int isFileFound = 0;
    char * fileName = NULL;

    for(int i=0;i<arrayLength;i++) {
        if(i == 0){
            continue;
        }
        if(strcmp(argv[i],"-u") == 0){
            isUFlag = 1;
        }
        if(strcmp(argv[i],"-r") == 0){
            isRFlag = 1;
        }
        if(strcmp(argv[i],"-u") != 0 && strcmp(argv[i],"-r") != 0 && strcmp(argv[i],"date") != 0) {
            isFileFound = 1;
            fileName = argv[i];
        }
    }

    if(isUFlag == 0 && isRFlag == 0 && isFileFound == 0) {
        time_t t =time(NULL);
        struct tm * lTime;
        lTime = localtime(&t);
        printf("IST %s",asctime(lTime));
    }

    else if(isUFlag == 1 && isRFlag == 0 && isFileFound == 0) {
        time_t t =time(NULL);
        struct tm * gTime;
        gTime = gmtime(&t);
        printf("UTC %s",asctime(gTime));
    }

    else if(isRFlag == 1 && isFileFound == 0) {
        printf("INSUFFICIENT INPUTS\n");
        return;
    }

    else if(isRFlag == 1 && isFileFound == 1) {
        struct stat attr;
        stat(fileName,&attr);
        char time[50];
        strftime(time, 50, "%Y-%m-%d %H:%M:%S", localtime(&attr.st_mtime));
        printf ("%s\n", time);
    }    
    else {
        printf("INVALID INPUT\n");
        return;
    }
}