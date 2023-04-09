#ifndef CMPE230_PROJECT2_CONCATENATOR_H
#define CMPE230_PROJECT2_CONCATENATOR_H

#define TABLE_SIZE 128

typedef struct linked_strings {
    int size;
    struct linked_strings* next;
    char text[];
} str;

str* declareAll();
char* linkStrings(str* string);

#endif //CMPE230_PROJECT2_CONCATENATOR_H
