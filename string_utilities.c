//
// Created by sherif on 15/10/16.
//
#include <string.h>
#include <ctype.h>
#include "string_utilities.h"


int check_for_wait(char *args[], int argsLen, int *noWait);

bool can_access(char *cmd, char *res);

char *append(const char *command, char *res, char *const *path, int i);

int handle_if_empty(char *args[], int arg_length);

int add_command_topath(char *command, char *res, char *path[], int *pathLength) {
    if (can_access(command, res)) {
        return 0;
    }
    for (int i = 0; i < *pathLength; ++i) {
        res = append(command, res, path, i);
        if (access(res, F_OK) == 0) {
            return 0;
        }
    }
    strcpy(res, "");
    return -1;
}

char *append(const char *command, char *res, char *const *path, int i) {
    sprintf(res, "%s/%s", path[i], command);
    while (*res != '/') {
        res++;
    }
    return res;
}

bool can_access(char *cmd, char *res) {
    if (access(cmd, F_OK) == 0) {
        strcpy(res, cmd);
        return true;
    }
    return false;
}


char *ltrim(char *s) {
    while (isspace(*s)) s++;
    return s;
}

char *rtrim(char *s) {
    char *back = s + strlen(s);
    while (isspace(*--back));
    *(back + 1) = '\0';
    return s;
}

void trim(char *s) {
    rtrim(ltrim(s));
}

void get_argument(char **args, char *tmp, int *noWait, char *currnent_line) {
    *noWait = 0;
    char delim[] = " \n\t";
    int arg_length = 0;
    char *temp;
    strcpy(tmp, currnent_line);
    trim(tmp);
    temp = strtok(tmp, delim);
    while (temp != NULL) {
        args[arg_length] = temp;
        temp = strtok(NULL, delim);
        arg_length++;
    }
    if (arg_length > 0) {
        if (check_for_wait(args, arg_length, noWait) == 0) {
            return;
        }
    }
    arg_length = handle_if_empty(args, arg_length);
    args[arg_length] = NULL;
}

int handle_if_empty(char *args[], int arg_length) {
    if (arg_length == 0) {
        args[0] = "";
        arg_length++;
    }
    return arg_length;
}


int check_for_wait(char *args[], int argsLen, int *noWait) {
    char amb = '&';
    if (strlen(args[argsLen - 1]) > 0 && args[argsLen - 1][strlen(args[argsLen - 1]) - 1] == amb) {
        *noWait = 1;
        if (strlen(args[argsLen - 1]) == 1) {
            args[argsLen - 1] = NULL;
            return 0;
        }
        args[argsLen - 1][strlen(args[argsLen - 1]) - 1] = NULL;
        return 1;
    }
    return 1;
}