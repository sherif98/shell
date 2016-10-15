//
// Created by sherif on 15/10/16.
//

#ifndef SHELL_HISTORY_H
#define SHELL_HISTORY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insert(char *command, int can_rewrite);
void print();
char * get_command(int index);
int isEmpty();
int size();
void load_history();
#endif //SHELL_HISTORY_H
