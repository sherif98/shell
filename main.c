#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <wait.h>
#include <stdbool.h>
#include "history.h"
#include "string_utilities.h"

#define MAX_LENGTH 2000
int pathLength = 0;
char currentLine[1024];
char aux[1024];
int dont_wait = 0;
char *path[MAX_LENGTH + 1];


void find_paths(char **pString);

void execute(char *args[81]);

void history();

void setupSignal();


void initialSetups();

void execute_file(char *fileName);

void readLine();

bool empty_line();


void handle_input();

bool isHistory(char *const *args);

void handleHistory();

bool isit_execute_last_History_command(char *const *args);

void excute_history_command(char *args[], int n);


bool get_number_of_history_command(char *args[], int *operations);

bool check_before_execution(char *args[]);

void exit_if_empty_line(char *res);

int main(int argc, char **argv) {
    initialSetups();
    if (argc == 2) {
        execute_file(argv[1]);
        return 0;
    } else if (argc > 2) {
        printf("Illegal Arguments\n");
    }


    while (true) {
        printf("shell>");
        readLine();
        if (empty_line()) {
            continue;
        }
        handle_input();
    }
}

bool empty_line() { return strlen(currentLine) == 0; }

void readLine() {
    if (!gets(currentLine)) { exit(0); }
    trim(currentLine);
}

void initialSetups() {
    signal(SIGCHLD, setupSignal);
    find_paths(path);
    load_history();
}

void setupSignal() {
    pid_t pid;
    while (pid = waitpid(-1, NULL, 0) > 0) {
        sleep(0.1);
    }
}

void execute_file(char *fileName) {
    char *inp = NULL;
    FILE *f;
    size_t len = 0;
    f = fopen(fileName, "r");
    if (f) {
        while (getline(&inp, &len, f) != -1) {
            if (strlen(inp) == 0) {
                continue;
            }
            trim(inp);
            strcpy(currentLine, inp);
            printf("%s\n", inp);
            handle_input();
        }
        fclose(f);
    } else {
        printf("No File exists");
    }
}

void handle_input() {
    char *args[85];
    get_argument(args, aux, &dont_wait, currentLine);
    if (args[0][0] == '!') {
        dont_wait = 0;
        if (isit_execute_last_History_command(args)) {
            if (isEmpty()) {
                printf("index out of bounds\n");
            } else {
                excute_history_command(args, size() - 1);
            }
        } else {
            int idx = 0;
            if (get_number_of_history_command(args, &idx)) {
                printf("index out of bounds\n");
            } else {
                excute_history_command(args, idx - 1);
            }
        }
    } else if (isHistory(args)) {
        handleHistory();
    } else {
        insert(currentLine, 1);
        execute(args);
    }
}

bool get_number_of_history_command(char *args[], int *idx) {
    int error = 0;
//    printf("%d", args[0][2]);
    int number = 0;
    if (args[0][1] == '1' && args[0][2] == '0') {
        number = 10;
    }
    if (args[0][1] > '1' && args[0][1] <= '9') {
        number = args[0][1] - '0';

    } else if (args[0][1] == '1' && args[0][2] == 0) {
        number = 1;
    } else {
        error = 1;
    }
    (*idx) = number;
    if (args[1] != NULL || (*idx) > size()) {
        error = 1;
    }
    return error == 1;
}

void excute_history_command(char *args[], int n) {
    char *cmd = get_command(n);
    strcpy(currentLine, cmd);
    puts(currentLine);
    get_argument(args, aux, &dont_wait, currentLine);
    execute(args);
}

bool isit_execute_last_History_command(char *const *args) {
    return strlen(args[0]) == 2 && args[0][1] == '!' && args[1] == NULL;
}

void handleHistory() {
    history();
    insert(currentLine, 1);
}

bool isHistory(char *const *args) { return strcmp(args[0], "history") == 0 && args[1] == NULL; }

void execute(char *args[81]) {
    if (check_before_execution(args)) {
        return;
    }
    char command_to_execute[1024];
    add_command_topath(args[0], command_to_execute, path, &pathLength);
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        return;
    }
    if (pid == 0) {
        exit_if_empty_line(command_to_execute);
        trim(command_to_execute);
        int execv_return_value = execv(command_to_execute, args);
        printf("failed to execute return value: %d\n", execv_return_value);
        exit(0);
    } else {
        if (!dont_wait) {
            waitpid(pid, 1, 0);
        }
    }
}

void exit_if_empty_line(char *res) {
    if (strcmp(res, "") == 0) {
        exit(0);
    }
}

bool check_before_execution(char *args[]) {
    if (strlen(currentLine) == 0) {
        return true;
    }
    if (strcmp(currentLine, "") == 0) {
        return true;
    }
    if (strlen(currentLine) > 80) {
        printf("larger than 80 char command\n");
        return true;
    }
    if (strcmp(args[0], "history") == 0 && args[1] == NULL) {
        history();
        return true;
    }
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    if (strcmp(currentLine, "\n") == 0) {
        return true;
    }
    return false;
}


void history() {
    if (isEmpty()) {
        printf("No History!\n");
        return;
    }
    print();
}

void find_paths(char **pString) {
    char *path = getenv("PATH");
    char delim[] = ":";
    int len = 0;
    char *tmp;
    tmp = strtok(path, delim);
    while (tmp != NULL) {
        pString[len] = tmp;
        tmp = strtok(NULL, delim);
        len++;
    }
    pString[len] = NULL;
    len++;
    pathLength = len;
}
