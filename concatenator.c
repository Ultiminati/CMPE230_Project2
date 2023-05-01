#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "concatenator.h"


extern char* keys[TABLE_SIZE];

str* declareAll(){
    str* old;
    str* result = NULL;
    char first = 1;
    for (int i = 0; i < TABLE_SIZE; i++){
        if (keys[i] != NULL){
            int strsize = 15 + strlen(keys[i]);
            str* new = (str*)malloc(sizeof(str)+sizeof(char)*(strsize+1));
            new->size = strsize;
            new->next = NULL;
            snprintf(new->text, strsize + 1, "%%%s = alloca i32\n", keys[i]);
            if (first == 0){
                old->next = new;
            } else {
                first = 0;
                result = new;
            }
            old = new;
        }
    }
    if (result == NULL){
        result = malloc(sizeof(str) + 1);
        result -> size = 0;
        result -> next = NULL;
        sprintf(result -> text, "%s", "");
    }
    return result;
}

char* linkStrings(str* first){
    int size = 0;
    str* string = first;
    while (string != NULL){
        size += string->size;
        string = string->next;
    }
    string = first;
    char* result = (char*) calloc(size+1,sizeof(char));
    while (string != NULL ){
        strcat(result,string->text);
        string = string->next;
    }
    return result;
}