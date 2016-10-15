
#include "history.h"
FILE *fp;
char list[10][81] = { "", "", "", "", "", "", "", "", "", "" };
int first = 0;
int capacity = 10;
int start = 0;
int elements = 0;

void insertIntoHistory(char command[] ,int mode) {
    int len = strlen(command);
    if(len == 0)return;
    if(command[len-1] == '\n'){
        command[len-1] = NULL;
    }
    if(first == capacity){
        for(int i = 0 ; i < capacity-1 ; i++){
            strcpy(list[i],list[i+1]);
        }
        strcpy(list[capacity-1],command);
    }
    else{
        strcpy(list[first++], command);
        elements++;
    }
    if(mode){
        fp = fopen("history.txt","w");
        for(int i = 0 ; i < elements ; i++){
            fputs(list[i],fp);
            fputs("\n",fp);
        }
        fclose(fp);
    }
}

void printAllHistory() {
    int current = 0;
    int cnt = 1;
    do {
        printf("%d. %s\n", cnt, list[current]);
        current++;
        cnt++;
    } while ((strcmp(list[current], "") != 0) && current != first);
}

char * getCommandFromHistory(int index) {
    return list[index];
}

int isEmpty() {
    return elements == 0;
}

int getSize(){
    return elements;
}

void loadHistory(){
    fp = fopen("history.txt","r");
    char * command = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&command, &len, fp)) != -1) {
        if(strlen(command) == 0 || strcmp(command,"\n") == 0)continue;
        insertIntoHistory(command,0);
    }
    fclose(fp);
}