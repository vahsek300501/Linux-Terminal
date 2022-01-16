#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>

char ** getSortedArray(char ** arr,int arrLen) {
    char tmp[1024];
    for(int i=0;i<arrLen-1;i++) {
        for(int j=0;j<arrLen-i-1;j++) {
            if(strcasecmp(arr[j],arr[j+1]) > 0){
                strcpy(tmp,arr[j]);
                strcpy(arr[j],arr[j+1]);
                strcpy(arr[j+1],tmp);
            }
        }
    }
    return arr;
}

void main(int argc,char ** argv) {
    int arrayLength = 0;
    for(int i=0;argv[i]!=NULL;i++) {
        arrayLength++;
    }

    if(arrayLength == 1){
        DIR * d = opendir(".");
        struct dirent *dir;
        char ** arr = malloc(256);
        int size = 0;

        if(d == NULL) {
            printf("FAILED TO OPEN CURRENT DIRECTORY!!");
            return;
        }
        while((dir = readdir(d))!=NULL) {
            char * tmp = dir->d_name;
            if(tmp[0] == '.') {
                continue;
            }
            else {
                arr[size++] = tmp;
            }
        }
        arr = getSortedArray(arr,size);
        for(int i=0;i<size;i++) {
            printf("%s\n",arr[i]);
        }
    }

    if(arrayLength == 2) {
        if(strcmp(argv[1],"-a") == 0){
            DIR * d = opendir(".");
            struct dirent *dir;
            char ** arr = malloc(256);
            int size = 0;

            if(d == NULL) {
                printf("FAILED TO OPEN CURRENT DIRECTORY!!");
                return;
            }

            while((dir = readdir(d))!=NULL) {
                char * tmp = dir->d_name;
                arr[size++] = tmp;
            }
            
            arr = getSortedArray(arr,size);
            for(int i=0;i<size;i++) {
                printf("%s\n",arr[i]);
            }
        }

        else if(strcmp(argv[1],"-r") == 0){
            DIR * d = opendir(".");
            struct dirent *dir;
            char ** arr = malloc(256);
            int size = 0;

            if(d == NULL) {
                printf("FAILED TO OPEN CURRENT DIRECTORY!!");
                return;
            }

            while((dir = readdir(d))!=NULL) {
                char * tmp = dir->d_name;
                if(tmp[0] == '.') {
                    continue;
                }
                else {
                    arr[size++] = tmp;
                }
            }
            
            arr = getSortedArray(arr,size);
            for(int i=size-1;i>=0;i--) {
                printf("%s\n",arr[i]);
            }
        }

        else {
            DIR * d = opendir(argv[1]);
            struct dirent *dir;
            char ** arr = malloc(2000);
            int size = 0;

            if(d == NULL) {
                printf("FAILED TO OPEN CURRENT DIRECTORY!!");
                return;
            }

            while((dir = readdir(d))!=NULL) {
                char * tmp = dir->d_name;
                if(tmp[0] == '.') {
                    continue;
                }
                else {
                    arr[size++] = tmp;
                }
            }
            
            arr = getSortedArray(arr,size);
            for(int i=0;i<size;i++) {
                printf("%s\n",arr[i]);
            }
        }
    }
}