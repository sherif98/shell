//
// Created by sherif on 15/10/16.
//
#include <string.h>
#include "string_utilities.h"


int check_for_wait(char *args[], int argsLen, int *noWait);

bool can_access(char *cmd, char *res);

void add_command_topath(char *command, char *res, char *path[], int *pathLength) {
    if (can_access(command, res)) {
        return;
    }
    for (int i = 0; i < *pathLength; ++i) {
        sprintf(res, "%s/%s", path[i], command);
        while (*res != '/') {
            res++;
        }
        if (access(res, F_OK) == 0) {
            return;
        }
    }
    strcpy(res, "");
}

bool can_access(char *cmd, char *res) {
    if (access(cmd, F_OK) == 0) {
        strcpy(res, cmd);
        return true;
    }
    return false;
}


void trim(char *line) {
    char *c = line;
    while (*c && (*c == ' ' || *c == '\n' || *c == '\t')) {
        c++;
    }
    size_t len = strlen(c);
    for (size_t i = len - 1; i >= 0; i--) {
        if (c[i] != ' ' && c[i] != '\t' && c[i] != '\n')break;
        c[i] = NULL;
    }
    strcpy(line, c);
}

void get_argument(char **args, char *tmp, int *noWait, char *currentLine) {
    *noWait = 0;
    char delim[] = " \n\t";
    int argsLen = 0;
    char *token;
    strcpy(tmp, currentLine);
    trim(tmp);
    token = strtok(tmp, delim);
    while (token != NULL) {
        args[argsLen] = token;
        token = strtok(NULL, delim);
        argsLen++;
    }
    if (argsLen == 0) {
        args[0] = "";
        argsLen++;
    } else if (argsLen > 0) {
        if (check_for_wait(args, argsLen, noWait) == 0) {
            return;
        }
    }
    args[argsLen] = NULL;
}


int check_for_wait(char *args[], int argsLen, int *noWait) {
    size_t len = strlen(args[argsLen - 1]);
    char amb = '&';
    if (len > 0 && args[argsLen - 1][len - 1] == amb) {
        *noWait = 1;
        if (len == 1) {
            args[argsLen - 1] = NULL;
            return 0;
        }
        args[argsLen - 1][len - 1] = NULL;
        return 1;
    }
    return 1;
}