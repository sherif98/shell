//
// Created by sherif on 15/10/16.
//

#ifndef SHELL_HISTORY_H
#define SHELL_HISTORY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertIntoHistory(char command[],int mode);
void printAllHistory();
char * getCommandFromHistory(int index);
int isEmpty();
int getSize();
void loadHistory();
#endif //SHELL_HISTORY_H
