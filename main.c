#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

char * currentWorkingDirectory = NULL;
char * actualWorkingDirectory = NULL;
char historyPath[1024];

char *readInp(void) {
        char *resp = (char*)malloc(sizeof(char)*1024);
        fgets(resp, 1024, stdin);
        if(resp == NULL) {
            return resp;
        }
        return resp;
}

char ** split(char *line) {
    char **args = (char**)malloc(sizeof(char*)*1024);
    char *argx = strtok(line, " \n\t\a");
    int pos = 0;
    while(argx != NULL) {
        args[pos] = argx;
        argx = strtok(NULL, " ");
        pos++;
    }
    args[pos] = NULL;
    return args;
}

char * removeQuotes(char * arr){
    char * final = (char*)malloc(1024);
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

char ** parseArray(char ** arr) {
    int pos = 0;

    for(int i=0;arr[i]!=NULL;i++) {
        char * tmp = removeQuotes(arr[i]);
        arr[pos] = tmp;
        pos++;
    }
    return arr;
}

char ** concatenateList(char ** arr) {
    char * lines = (char*)malloc(1024);
    int replacePos = 1;
    int isFirst = 0;
    int j=0;
    while(arr[j]!=NULL) {
        j++;
    }
    if(j == 1) {
        return arr;
    }
    for(int i=1;;i++) {
        char tmp[100];
        if(arr[i] == NULL){
            break;
        }
        if(arr[i][0] == '-' && strlen(arr[i]) == 2) {
            replacePos ++;
            continue;
        }
        else {
            if(isFirst == 0) {
                strcpy(tmp,arr[i]);
                strcpy(lines,tmp);
                isFirst = 1;
                continue;
            }
            else {
                strcat(lines," ");
                strcpy(tmp,arr[i]);
                strcat(lines,tmp);
            }
        }
    }
    arr[replacePos] = lines;
    arr[replacePos+1] = NULL;
    return arr;
}

int getArrayLength(char ** arr) {
    int i=0;
    while(arr[i]!=NULL) {
        i++;
    }
    return i;
}

char * removeLastCharacter(char * arr) {
    int i=0;
    while(1) {
        if(arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
        i++;
    }
    return arr;
}

void writeHistory(char ** val) {

    FILE * fout = fopen(historyPath,"a");
    for(int i=0;;i++) {
        if(val[i] == NULL) {
            fprintf(fout,"\n");
            break;
        }
        fprintf(fout,"%s ",val[i]);
    }

    fclose(fout);
}

void print() {
    struct passwd* pwd;
    pwd = getpwuid(geteuid());
    char * user = NULL;
    char currentDirectoy[1024];
    if(pwd) {
        user = pwd->pw_name;
    }
    else {
        printf("ERROR!!!");
        return;
    }
    printf("%s%s%s%s",user,"$",currentWorkingDirectory,":~");
}

int isSymLink(char * arr) {
    struct stat tmp;
    lstat(arr,&tmp);
    if(S_ISLNK(tmp.st_mode) == 1){
        return 1;
    }
    return 0;
}

char * getFullPath(char * arr) {
    char *finalPath = (char*)malloc(1024);
    strcpy(finalPath,currentWorkingDirectory);
    strcat(finalPath,"/");
    strcat(finalPath,arr);
    return finalPath;
}

char * getTillLastSlash(char * arr) {
    char * finalPath = (char*)malloc(1024);
    int slashCount = 0;
    for(int i=0;arr[i]!='\0';i++) {
        if(arr[i]=='/'){
            slashCount ++;
        }
    }
    int tmp = 0;
    for(int i=0;arr[i]!='\0';i++) {
        if(arr[i]=='/'){
            tmp++;
        }
        if(tmp == slashCount){
            break;
        }
        finalPath[i] = arr[i];
    }
    return finalPath;
}

int goBackCheck(char * arr) {
    for(int i=0;arr[i]!='\0';i++) {
        if(arr[i] == '/' || arr[i] == '.') {
            continue;
        }
        else {
            return 0;
        }
    }
    return 1;
}

/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/

void changeDirectory(char ** arr) {
    int arrayLength = getArrayLength(arr);
    int isPFlag = 0;
    int isLFlag = 0;
    int isPathFound = 0;
    char * path = NULL;

    for(int i=0;i<arrayLength;i++) {
        if(i == 0) {
            continue;
        }
        if(strcmp(arr[i],"-P") == 0){
            isPFlag = 1;
        }
        if(strcmp(arr[i],"-L") == 0) {
            isLFlag = 1;
        }
        if(strcmp(arr[i],"-P") != 0 && strcmp(arr[i],"-L") != 0 && strcmp(arr[i],"cd") != 0) {
            isPathFound = 1;
            path = arr[i];
        }
    }

    if(isPathFound == 0) {
        struct passwd *pw = getpwuid(getuid());
        chdir(pw->pw_dir);
        char currentDirectoy[1024];
        getcwd(currentDirectoy,1024);
        
        actualWorkingDirectory = currentDirectoy;
        currentWorkingDirectory = currentDirectoy;
        return;
    }

    if(strcmp(path,"~") == 0 || strcmp(path,"/") == 0) {
        struct passwd *pw = getpwuid(getuid());
        chdir(pw->pw_dir);
        char currentDirectoy[1024];
        getcwd(currentDirectoy,1024);
        actualWorkingDirectory = currentDirectoy;
        currentWorkingDirectory = currentDirectoy;
        return;
    }

    if(goBackCheck(path) == 1) {
        int slashCount = 0;
        char * tmpPath = currentWorkingDirectory;
        for(int i=0;path[i]!='\0';i++) {
            if(path[i] == '/'){
                slashCount++;
            }
        }

        slashCount++;
        for(int i=0;i<slashCount;i++) {
            if(strcmp(actualWorkingDirectory,currentWorkingDirectory) == 0) {
                int output = chdir("..");
                if(output != 0){
                    printf("DIRECTORY NOT FOUND\n");
                }
                char currentDirectoy[1024];
                getcwd(currentDirectoy,1024);   
                actualWorkingDirectory = currentDirectoy;
                currentWorkingDirectory = currentDirectoy;
            }

            else {
                tmpPath = getTillLastSlash(tmpPath);
                
                if(isSymLink(tmpPath) == 0) {
                    chdir(tmpPath);
                    char currentDirectoy[1024];
                    getcwd(currentDirectoy,1024);
                    actualWorkingDirectory = currentDirectoy;
                    currentWorkingDirectory = currentDirectoy;
                }
                else {
                    char currentDirectoy[1024];
                    chdir(tmpPath);
                    getcwd(currentDirectoy,1024);
                    actualWorkingDirectory = currentDirectoy;
                    currentWorkingDirectory = tmpPath;
                }
            }
        }
        return;
    }  

    if(path[0] != '/') {
        path = getFullPath(path);
    }

    int tmp = isSymLink(path);
    
    if(tmp == 0) {
        int output = chdir(path);
        if(output != 0) {
            printf("DIRECTORY NOT FOUND\n");
            return;
        }
        char currentDirectoy[1024];
        getcwd(currentDirectoy,1024);
        actualWorkingDirectory = currentDirectoy;
        currentWorkingDirectory = currentDirectoy;
        return;
    }

    else {
        int output = chdir(path);
        if(output != 0) {
            printf("DIRECTORY NOT FOUND\n");
            return;
        }
        char currentDirectoy[1024];
        getcwd(currentDirectoy,1024);
        actualWorkingDirectory = currentDirectoy;
        if(isPFlag == 1 && isLFlag == 0) {
            currentWorkingDirectory = currentDirectoy;
        }
        else if(isPFlag == 0 && isLFlag == 1) {
            currentWorkingDirectory = path;
        }
        else {
            currentWorkingDirectory = path;
        }
        return;
    }
}

void historyCommand(char **arr) {
    int i = 1;
    for( ; arr[i] != NULL && arr[i][0] == '-' ; i++) {
        if(strcmp(arr[i], "-c") == 0) {
            FILE *fr = fopen(historyPath,"w");
            fclose(fr);
            return;
        }
        else if(strcmp(arr[i], "-w") == 0) {
            if(arr[i+1] == NULL) {
                printf("Missing argument to save history in. Refer to readme.\n");
                return;
            }
            FILE *fw = fopen(arr[i+1],"w");
            FILE *fr = fopen(historyPath,"r+");
            int ind = 0;
            char ent[128];
            while (fgets(ent, 128, fr) != NULL) {
                ind++;
                fprintf(fw, "%d\t%s", ind, ent);
            }
            fclose(fw);
            fclose(fr);
            return;
        }
        else {
            printf("INVALID INPUT\n");
            return;
        }
    }
    FILE *fr = fopen(historyPath,"r+");
    int ind = 0;
    char ent[128];
    while (fgets(ent, 128, fr) != NULL) {
        ind++;
        fprintf(stdout, "%d\t%s", ind, ent);
    }
    fclose(fr);
}

void pwdCommand(char ** arr) {
    int arrayLength = getArrayLength(arr);
    int isPFlag = 0;
    int isLFlag = 0;
    
    for(int i=0;i<arrayLength;i++) {
        if(i == 0) {
            continue;
        }
        if(strcmp(arr[i],"-P") == 0){
            isPFlag = 1;
        }
        if(strcmp(arr[i],"-L") == 0) {
            isLFlag = 1;
        }
    }

    if(isPFlag == 0 && isLFlag == 0) {
        printf("%s\n",currentWorkingDirectory);
    }
    else if(isPFlag == 0 && isLFlag == 1) {
        printf("%s\n",currentWorkingDirectory);
    }
    else if(isPFlag == 1 && isLFlag == 0) {
        printf("%s\n",actualWorkingDirectory);
    }
    else {
        printf("INVALID INPUT!!! REFER TO THE DOCUMENTATION\n");
    }
}

void echoCommand(char ** arr) {
    int arrayLength = 0;
    int i = 0;
    while(arr[i]!=NULL){
        arrayLength++;
        i++;
    }
    if(arrayLength > 3) {
        printf("Too many Arguments provided\n");
        return;
    }
    if(arrayLength == 1) {
        printf("%c",'\n');
    }
    if(arrayLength == 2 && strcmp(arr[1],"--help")!=0) {
        printf("%s%c",arr[1],'\n');
    }
    if(arrayLength == 2 && strcmp(arr[1],"--help") == 0) {
        printf("NAME:  ECHO\n");
            printf("DESCRIPTION:\n");
            printf("      -n:        do not output the trailing new line.\n%c",'\n');
            printf("      --help     Prints the help menu\n%c%c",'\n','\n');
            printf("COPYRIGHT ©  2018  Free Software Foundation, Inc.  License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>. This is free software: you are free  to  change  and  redistribute  it. There is NO WARRANTY, to the extent permitted by law. \n\n");
    }
    if(arrayLength == 3) {
        if(strcmp(arr[1],"-n") == 0){
            printf("%s",arr[2]);
        }
        else if(strcmp(arr[1],"--help") == 0){
            printf("NAME:  ECHO\n");
            printf("DESCRIPTION:\n");
            printf("      -n:        do not output the trailing new line.\n%c",'\n');
            printf("      --help     Prints the help menu\n%c%c",'\n','\n');
            printf("COPYRIGHT ©  2018  Free Software Foundation, Inc.  License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>. This is free software: you are free  to  change  and  redistribute  it. There is NO WARRANTY, to the extent permitted by law. \n\n");
        }
        else {
            printf("INVALID INPUT!!!\n");
        }
    }
}

void exitCommand(char ** arr) {
    int arrayLength = 0;
    int i = 0;
    while(arr[i]!=NULL){
        arrayLength++;
        i++;
    }
    if(arrayLength == 2 && strcmp(arr[1],"--help") == 0) {
        printf("NAME:     EXIT\n");
        printf("FUNCTION:   The function terminates the calling process immediately.\n");
        printf("DESCRIPTION:\n");
        printf("       --help      for calling the help menu\n");
    }
    else if(arrayLength == 1) {
        exit(0);
    }
    else {
        printf("INVALID INPUT\n");
    }
}

void loop() {
    while(1>0) {
        
        print();
        char * input = readInp();
        char ** splittedArr = split(input);
        splittedArr = concatenateList(splittedArr);

        int arrayLen = getArrayLength(splittedArr);
        char * tmpArr = splittedArr[arrayLen-1];
        tmpArr = removeLastCharacter(tmpArr);
        splittedArr[arrayLen-1] = tmpArr;

        splittedArr = parseArray(splittedArr);
        writeHistory(splittedArr);
        
        if(strcmp(splittedArr[0],"cd") == 0) {
            changeDirectory(splittedArr);
        }
        
        else if(strcmp(splittedArr[0],"echo") == 0) {
            echoCommand(splittedArr);
        }
        else if(strcmp(splittedArr[0],"history") == 0) {
            historyCommand(splittedArr);
        }
        else if(strcmp(splittedArr[0],"pwd") == 0) {
            pwdCommand(splittedArr);
        }
        else if(strcmp(splittedArr[0],"exit") == 0){
            // exit(0);
            exitCommand(splittedArr);
        }

        else if(strcmp(splittedArr[0],"ls") == 0) {
            int processId = fork();
            if(processId == 0){
                int tmp =execvp("ls",splittedArr);
            }
            else{
                wait(NULL);
            }
        }
        else if(strcmp(splittedArr[0],"cat") == 0) {
            int processId = fork();
            if(processId == 0){
                int tmp =execvp("cat",splittedArr);
            }
            else{
                wait(NULL);
            }
        }

        else if(strcmp(splittedArr[0],"date") == 0 ){
            int processId = fork();
            if(processId == 0){
                int tmp =execvp("date",splittedArr);
            }
            else{
                wait(NULL);
            }
        }

        else if(strcmp(splittedArr[0],"rm") == 0) {
            int processId = fork();
            if(processId == 0){
                int tmp =execvp("rm",splittedArr);
            }
            else{
                wait(NULL);
            }
        }

        else if(strcmp(splittedArr[0],"mkdir") == 0) {
            int processId = fork();
            if(processId == 0){
                int tmp =execvp("mkdir",splittedArr);
            }
            else{
                wait(NULL);
            }
        }
    }
}

int main() {
    char currentDirectoy[1024];
    char * result = getcwd(currentDirectoy,1024);
    if(result == NULL) {
        printf("ERROR!!!!");
        return 0;
    }
    currentWorkingDirectory = result;
    actualWorkingDirectory = result;
    strcpy(historyPath,currentWorkingDirectory);
    strcat(historyPath,"/");
    strcat(historyPath,"history.txt");
    loop();
    return 0;
}