/////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "history.h"
#include "string_utilities.h"

#define MAX_LINE 2000
int pathLength = 0;
char currentLine[1024];
char aux[1024];
int dont_wait = 0;
char *path[MAX_LINE + 1];


void loadPath(char *pString[]);

void execute(char *args[81]);

void history();

void setupSignal(int x);


void initialSetups();

void executeInputFile(char *fileName);

void readLine();

bool validInutLine();


void parse();

bool isHistory(char *const *args);

void handleHistory();

bool isit_execute_last_History_command(char *const *args);

void excute_history_command(char *args[], int n);


bool get_number_of_history_command(char *const *args, int *operations) ;

int main(int argc, char **argv) {
    initialSetups();
    if (argc == 2) {
        executeInputFile(argv[1]);
        return 0;
    } else if (argc > 2) {
        printf("Illegal Arguments\n");
    }


    while (true) {
        printf("shell>");
        readLine();
        if (validInutLine()) {
            continue;
        }
        parse();
    }
}

bool validInutLine() { return strlen(currentLine) == 0; }

void readLine() {
    gets(currentLine);
    trim(currentLine);
}

void initialSetups() {
    signal(SIGCHLD, setupSignal);
    loadPath(path);
    loadHistory();
}

void setupSignal(int sig) {
    pid_t pid;
    while (pid = waitpid(-1, NULL, 0) > 0) {
        sleep(0.1);
    }
}

void executeInputFile(char *fileName) {
    char *command = NULL;
    FILE *file;
    size_t len = 0;
    file = fopen(fileName, "r");
    int c = 0;
    if (file) {
        while (getline(&command, &len, file) != -1) {
            trim(command);
            if (strlen(command) == 0) {
                continue;
            }
            printf("%s\n", command);
            strcpy(currentLine, command);
            parse();
        }
        fclose(file);
    } else {
        printf("File doesn't exist!");
    }
}

void parse() {
    char *args[81];
    get_argument(args, aux, &dont_wait, currentLine);
    if (isHistory(args)) {
        handleHistory();
    } else if (args[0][0] == '!') {
        dont_wait = 0;
        if (isit_execute_last_History_command(args)) {
            if (isEmpty()) {
                printf("index out of bounds\n");
            } else {
                excute_history_command(args, getSize() - 1);
            }
        } else {
            int operations = 0;
            if (get_number_of_history_command(args, &operations)) {
                printf("index out of bounds\n");
            } else {
                excute_history_command(args, operations - 1);
            }
        }
    } else {
        insertIntoHistory(currentLine, 1);
        execute(args);
    }
}

bool get_number_of_history_command(char *const *args, int *operations) {
    int error = 0;
    for (int i = 1; i < strlen(args[0]); i++) {
        if (args[0][i] >= '0' && args[0][i] <= '9') {
            (*operations) = (*operations) * 10 + args[0][i] - '0';
        } else {
            error = 1;
            break;
        }
    }
    if (args[1] != NULL || (*operations) > getSize()) {
        error = 1;
    }
    return error == 1;
}

void excute_history_command(char *args[], int n) {
    char *command = getCommandFromHistory(n);
    strcpy(currentLine, command);
    puts(currentLine);
    get_argument(args, aux, &dont_wait, currentLine);
    execute(args);
}

bool isit_execute_last_History_command(char *const *args) {
    return strlen(args[0]) == 2 && args[0][1] == '!' && args[1] == NULL;
}

void handleHistory() {
    history();
    insertIntoHistory(currentLine, 1);
}

bool isHistory(char *const *args) { return strcmp(args[0], "history") == 0 && args[1] == NULL; }

void execute(char *args[81]) {
    if (strlen(currentLine) > 80) {
        printf("Error, too long command\n");
        return;
    }
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
    if (strcmp(args[0], "history") == 0 && args[1] == NULL) {
        history();
        return;
    }
    char res[1024];
    add_command_topath(args[0], res, path, &pathLength);
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        if (strcmp(res, "") == 0) {
            exit(0);
        }
        int ret_val = execv(res, args);
        printf("failed to execute command, exist value: %d\n", ret_val);
        exit(0);
    } else if (pid < 0) {
        return;
    } else {
        if (!dont_wait) {
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
