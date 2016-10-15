//
// Created by sherif on 15/10/16.
//
#include <string.h>
#include "string_utilities.h"


void add_command_topath(char *command, char *res, char *path[], int *pathLength) {
    if (access(command, F_OK) == 0) {
        strcpy(res, command);
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


void trim(char *line) {
    char *c = line;
    while (*c && (*c == ' ' || *c == '\n' || *c == '\t')) {
        c++;
    }
    int len = strlen(c);
    for (int i = len - 1; i >= 0; i--) {
        if (c[i] != ' ' && c[i] != '\t' && c[i] != '\n')break;
        c[i] = NULL;
    }
    strcpy(line, c);
}

void get_argument(char **args, char *aux, int *noWait, char *currentLine) {
    *noWait = 0;
    char seps[] = " \t\n";
    int argsLen = 0;
    char *token;
    strcpy(aux, currentLine);
    trim(aux);
    token = strtok(aux, seps);
    while (token != NULL) {
        args[argsLen] = token;
        token = strtok(NULL, seps);
        argsLen++;
    }
    if (argsLen == 0) {
        args[0] = "";
        argsLen++;
    }
    if (argsLen > 0) {
        int len = strlen(args[argsLen - 1]);
        if (len > 0 && args[argsLen - 1][len - 1] == '&') {
            *noWait = 1;
            if (len == 1) {
                args[argsLen - 1] = NULL;
                return;
            }
            args[argsLen - 1][len - 1] = NULL;
        }
    }
    args[argsLen] = NULL;
}