/////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <wait.h>
#include <sys/stat.h>
#include "history.h"
#define MAX_LINE 2000
int pathLength = 0;
char line[1024];
char aux[1024];
int noWait = 0;
char *path[MAX_LINE + 1];

int batchMode = 0;

void loadPath(char *pString[]);

void execute(char *args[81]);

void history();

void handler(int x);

void trim();

int main(int argc, char **argv) {
    signal(SIGCHLD, handler);
    loadPath(path);
    loadHistory();
    if (argc == 2) {
        executeCommandsFromFile(argv[1]);
    } else if (argc > 2) {
        printf("Too many arguments!\n");
    }

    batchMode = 0;

    int should_run = 1;
    while (should_run) {
        printf("shell>");
        gets(line);
        trim(line);
        if(strlen(line) == 0){
            continue;
        }
        parse();
    }
    return 0;
}

void handler(int sig){
    pid_t pid;
    while(pid = waitpid(-1,NULL,0) > 0){
        sleep(0.1);
    }
}

void executeCommandsFromFile(char *fileName) {
    char *command = NULL;
    FILE * file;
    size_t len = 0;
    file = fopen(fileName, "r");
    batchMode = 1;
    int c = 0;
    if (file) {
        while (getline(&command, &len, file) != -1) {
            trim(command);
            if(strlen(command) == 0){
                continue;
            }
            printf("%s\n", command);
            if(c == 21)break;
            strcpy(line,command);
            parse();
        }
        fclose(file);
    } else {
        printf("File doesn't exist!");
    }
}

void parse(){
    char *args[81];
    getArgs(args);
    if (strcmp(args[0], "history") == 0 && args[1] == NULL) {
        history();
        insertIntoHistory(line,1);
    } else if(args[0][0] == '!') {
        noWait = 0;
        if(strlen(args[0]) == 2 && args[0][1] == '!' && args[1] == NULL){
            if(isEmpty()){
                printf("error in ! command\n");
            }
            else{
                char *command = getCommandFromHistory(getSize()-1);
                strcpy(line, command);
                puts(line);
                getArgs(args);
                execute(args);
            }
        }
        else{
            int operations = 0;
            int error = 0;
            for(int i = 1 ; i < strlen(args[0]) ; i++){
                if(args[0][i] >= '0' && args[0][i] <= '9'){
                    operations = operations * 10 + args[0][i] - '0';
                }
                else{
                    error = 1;
                    break;
                }
            }
            if(args[1] != NULL || operations > getSize()){
                error = 1;
            }
            if(error){
                printf("error in ! command\n");
            }
            else{
                char *command = getCommandFromHistory(operations-1);
                strcpy(line, command);
                puts(line);
                getArgs(args);
                execute(args);
            }
        }
    }
    else {
        insertIntoHistory(line,1);
        execute(args);
    }
}
void execute(char *args[81]){
    if(strlen(line) > 80){
        printf("Error, too long command\n");
        return;
    }
    if(strcmp(args[0],"exit") == 0){
        exit(0);
    }
    if (strcmp(args[0], "history") == 0 && args[1] == NULL) {
        history();
        return;
    }
    char res[1024];
    getPathToCommand(args[0], res);
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        if(strcmp(res,"") == 0){
            exit(0);
        }
        int ret_val = execv(res, args);
        printf("failed to execute command, exist value: %d\n", ret_val);
        exit(0);
    } else if(pid < 0){
        return;
    }
    else {
        if(!noWait){
            waitpid(pid, 1, 0);
        }
    }
}
void history() {
    if (isEmpty()) {
        printf("No History!\n");
        return;
    }
    printAllHistory();
}

int is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

void getPathToCommand(char *command, char res[]) {
    if(access(command,F_OK) == 0){
        strcpy(res,command);
        return;
    }
    for (int i = 0; i < pathLength; ++i) {
        sprintf(res, "%s/%s", path[i], command);
        while(*res != '/'){
            res++;
        }
        if (access(res,F_OK) == 0) {
            return;
        }
    }
    strcpy(res,"");
}

void trim(char *str){
    char *c = str;
    while(*c && (*c == ' ' || *c == '\n' || *c == '\t')){
        c++;
    }
    int len = strlen(c);
    for(int i = len - 1 ; i >= 0 ; i--){
        if(c[i] != ' ' && c[i] != '\t' && c[i] != '\n')break;
        c[i] = NULL;
    }
    strcpy(str,c);
}

void getArgs(char *args[]) {
    noWait = 0;
    char seps[] = " \t\n";
    int argsLen = 0;
    char *token;
    strcpy(aux,line);
    trim(aux);
    token = strtok(aux, seps);
    while (token != NULL) {
        args[argsLen] = token;
        token = strtok(NULL, seps);
        argsLen++;
    }
    if(argsLen == 0){
        args[0] = "";
        argsLen++;
    }
    if(argsLen > 0){
        int len = strlen(args[argsLen-1]);
        if(len > 0 && args[argsLen-1][len-1] == '&'){
            noWait = 1;
            if(len == 1){
                args[argsLen-1] = NULL;
                return;
            }
            args[argsLen-1][len-1] = NULL;
        }
    }
    args[argsLen] = NULL;
    argsLen++;
}

void loadPath(char *pString[]) {
    char *path = getenv("PATH");
    char seps[] = ":";
    int argsLen = 0;
    char *token;
    token = strtok(path, seps);
    while (token != NULL) {
        pString[argsLen] = token;
        token = strtok(NULL, seps);
        argsLen++;
    }
    pString[argsLen] = NULL;
    argsLen++;
    pathLength = argsLen;
}
