//
// Created by sherif on 14/10/16.
//
#include <stdbool.h>
#include <string.h>
#include <bits/string2.h>
#include "history.h"

FILE *file;
int front = 0;
const int MAX_SIZE = 10;
int items = 0;
char my_history[10][85] = {"", "", "", "", "", "", "", "", "", ""};

bool is_empty_line(const char *command);

void add(const char *command);

void save_in_file();

void make_space(const char *command);

void handle_new_line(char *command);

void load_history() {
    file = fopen("history.txt", "r");
    char *command = NULL;
    size_t x = 0;
    ssize_t read;
    while ((read = getline(&command, &x, file)) != -1) {
        if (is_empty_line(command))continue;
        insert(command, 0);
    }
    fclose(file);
}

bool is_empty_line(const char *command) { return strlen(command) == 0 || strcmp(command, "\n") == 0; }

void insert(char *command, int can_rewrite) {
    if (strlen(command) == 0) {
        return;
    }
    handle_new_line(command);
    add(command);
    if (can_rewrite) {
        save_in_file();
    }
}

void handle_new_line(char *command) {
    if (command[strlen(command) - 1] == '\n') {
        command[strlen(command) - 1] = NULL;
    }
}

void save_in_file() {
    file = fopen("history.txt", "w");
    for (int i = 0; i < size(); i++) {
        fputs(my_history[i], file);
        fputs("\n", file);
    }
    fclose(file);
}

void add(const char *command) {
    if (front == MAX_SIZE) {
        make_space(command);
    } else {
        strcpy(my_history[front++], command);
        items++;
    }
}

void make_space(const char *command) {
    for (int i = 0; i < MAX_SIZE - 1; i++) {
        strcpy(my_history[i], my_history[i + 1]);
    }
    strcpy(my_history[MAX_SIZE - 1], command);
}

void print() {
    for (int i = 0; i < size(); i++) {
        printf("%d- %s\n", i + 1, my_history[i]);
    }
}

char *get_command(int index) {
    return my_history[index];
}

int isEmpty() {
    return items == 0;
}

int size() {
    return items;
}
