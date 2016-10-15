//
// Created by sherif on 15/10/16.
//

#ifndef SHELL_STRING_UTILITIES_H
#define SHELL_STRING_UTILITIES_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "history.h"

void trim(char *line);

void get_argument(char **args, char *tmp, int *needwait, char *currnent_line);

void add_command_topath(char *command, char *res, char *path[], int *pathLength);

#endif //SHELL_STRING_UTILITIES_H
