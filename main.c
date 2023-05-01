#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "stacks.h"
#include "concatenator.h"



char msgbuf[257];
regex_t regex;

int tokenIndex = 0;
int intermediateVariableIndex = 0;
int isAssigned = 0;
int line = 0;

char* keys[TABLE_SIZE];
str firstString = {125,NULL,
                   "; ModuleID = 'advcalc2ir'\n"
                   "declare i32 @printf(i8*, ...)\n"
                   "@print.str = constant [4 x i8] c\"%d\\0A\\00\""
                   "\n\ndefine i32 @main() {\n"};
str* lastString;

int errorFoundUninitVar = 0;



const int parsingTable[75][16][2] =
        {{{1, 3}, {0,0}, {1, 4}, {0,0}, {1, 5}, {0,0}, {1, 6}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 7}, {0,0}, {0,0}, {3, 1}, {3, 2}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {-1,-1}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 8}, {1, 9}, {1, 10}, {1, 11}, {0,0}, {2, 1}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {1, 12}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 10}, {2, 10}, {2, 10}, {2, 10}, {0,0}, {2, 10}, {0,0}, {0,0}, {0,0}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 13}},
         {{0,0}, {0,0}, {1, 19}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {1, 20}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 11}, {2, 11}, {2, 11}, {2, 11}, {0,0}, {2, 11}, {0,0}, {0,0}, {0,0}},
         {{1, 22}, {0,0}, {1, 4}, {0,0}, {1, 5}, {0,0}, {1, 6}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 7}, {0,0}, {0,0}, {0,0}, {3, 21}},
         {{1, 22}, {0,0}, {1, 4}, {0,0}, {1, 5}, {0,0}, {1, 6}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 7}, {0,0}, {0,0}, {0,0}, {3, 23}},
         {{1, 22}, {0,0}, {1, 4}, {0,0}, {1, 5}, {0,0}, {1, 6}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 7}, {0,0}, {0,0}, {0,0}, {3, 24}},
         {{1, 22}, {0,0}, {1, 4}, {0,0}, {1, 5}, {0,0}, {1, 6}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 7}, {0,0}, {0,0}, {0,0}, {3, 25}},
         {{1, 22}, {0,0}, {1, 4}, {0,0}, {1, 5}, {0,0}, {1, 6}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 7}, {0,0}, {0,0}, {0,0}, {3, 26}},
         {{0,0}, {0,0}, {0,0}, {1, 27}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {1, 31}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 32}},
         {{0,0}, {0,0}, {1, 33}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {1, 34}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {2, 10}, {0,0}, {0,0}, {0,0}, {2, 10}, {2, 10}, {2, 10}, {2, 10}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {2, 11}, {0,0}, {0,0}, {0,0}, {2, 11}, {2, 11}, {2, 11}, {2, 11}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{1, 39}, {0,0}, {1, 36}, {0,0}, {1, 37}, {0,0}, {1, 38}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 40}, {0,0}, {0,0}, {0,0}, {3, 35}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 41}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 6}, {2, 6}, {2, 6}, {2, 6}, {0,0}, {2, 6}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 10}, {2, 10}, {2, 10}, {2, 10}, {0,0}, {2, 10}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 8}, {2, 7}, {2, 7}, {2, 7}, {0,0}, {2, 7}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 8}, {1, 9}, {2, 8}, {2, 8}, {0,0}, {2, 8}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 8}, {1, 9}, {1, 10}, {2, 9}, {0,0}, {2, 9}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 8}, {1, 9}, {1, 10}, {1, 11}, {0,0}, {2, 2}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 3}, {2, 3}, {2, 3}, {2, 3}, {0,0}, {2, 3}, {0,0}, {0,0}, {0,0}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 42}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 43}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 44}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 45}},
         {{0,0}, {0,0}, {0,0}, {1, 46}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {1, 31}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{1, 39}, {0,0}, {1, 36}, {0,0}, {1, 37}, {0,0}, {1, 38}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 40}, {0,0}, {0,0}, {0,0}, {3, 47}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 48}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 49}, {0,0}, {1, 50}, {1, 51}, {1, 52}, {1, 53}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 54}},
         {{0,0}, {0,0}, {1, 55}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {1, 56}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 10}, {0,0}, {2, 10}, {2, 10}, {2, 10}, {2, 10}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 11}, {0,0}, {2, 11}, {2, 11}, {2, 11}, {2, 11}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {1, 57}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {1, 31}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {2, 6}, {0,0}, {0,0}, {0,0}, {2, 6}, {2, 6}, {2, 6}, {2, 6}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {2, 7}, {0,0}, {0,0}, {0,0}, {1, 28}, {2, 7}, {2, 7}, {2, 7}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {2, 8}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {2, 8}, {2, 8}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {2, 9}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {2, 9}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {2, 3}, {0,0}, {0,0}, {0,0}, {2, 3}, {2, 3}, {2, 3}, {2, 3}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 58}, {0,0}, {1, 50}, {1, 51}, {1, 52}, {1, 53}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {1, 59}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {1, 31}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 60}},
         {{1, 39}, {0,0}, {1, 36}, {0,0}, {1, 37}, {0,0}, {1, 38}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 40}, {0,0}, {0,0}, {0,0}, {3, 61}},
         {{1, 39}, {0,0}, {1, 36}, {0,0}, {1, 37}, {0,0}, {1, 38}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 40}, {0,0}, {0,0}, {0,0}, {3, 62}},
         {{1, 39}, {0,0}, {1, 36}, {0,0}, {1, 37}, {0,0}, {1, 38}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 40}, {0,0}, {0,0}, {0,0}, {3, 63}},
         {{1, 39}, {0,0}, {1, 36}, {0,0}, {1, 37}, {0,0}, {1, 38}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 40}, {0,0}, {0,0}, {0,0}, {3, 64}},
         {{0,0}, {0,0}, {0,0}, {1, 65}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {1, 31}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{1, 39}, {0,0}, {1, 36}, {0,0}, {1, 37}, {0,0}, {1, 38}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 40}, {0,0}, {0,0}, {0,0}, {3, 66}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 67}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 5}, {2, 5}, {2, 5}, {2, 5}, {0,0}, {2, 5}, {0,0}, {0,0}, {0,0}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 68}},
         {{0,0}, {0,0}, {0,0}, {2, 5}, {0,0}, {0,0}, {0,0}, {2, 5}, {2, 5}, {2, 5}, {2, 5}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {1, 69}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {1, 31}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 6}, {0,0}, {2, 6}, {2, 6}, {2, 6}, {2, 6}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 7}, {0,0}, {1, 50}, {2, 7}, {2, 7}, {2, 7}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 8}, {0,0}, {1, 50}, {1, 51}, {2, 8}, {2, 8}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 9}, {0,0}, {1, 50}, {1, 51}, {1, 52}, {2, 9}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 3}, {0,0}, {2, 3}, {2, 3}, {2, 3}, {2, 3}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 70}, {0,0}, {1, 50}, {1, 51}, {1, 52}, {1, 53}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {1, 71}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {1, 31}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {1, 72}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {1, 31}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 4}, {2, 4}, {2, 4}, {2, 4}, {0,0}, {2, 4}, {0,0}, {0,0}, {0,0}},
         {{1, 17}, {0,0}, {1, 14}, {0,0}, {1, 15}, {0,0}, {1, 16}, {0,0}, {0,0}, {0,0}, {0,0}, {1, 18}, {0,0}, {0,0}, {0,0}, {3, 73}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 5}, {0,0}, {2, 5}, {2, 5}, {2, 5}, {2, 5}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {2, 4}, {0,0}, {0,0}, {0,0}, {2, 4}, {2, 4}, {2, 4}, {2, 4}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {1, 74}, {0,0}, {0,0}, {0,0}, {1, 28}, {1, 29}, {1, 30}, {1, 31}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}},
         {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {2, 4}, {0,0}, {2, 4}, {2, 4}, {2, 4}, {2, 4}, {0,0}, {0,0}, {0,0}, {0,0},{0,0}}
};





char exists(char* string){
    int i;
    for (i = 0; i < TABLE_SIZE; i++){
        if (keys[i] != NULL){
            if (strcmp(keys[i],string) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

void put(char* string){
    int i;
    if (!exists(string)) {
        for (i = 0; i < TABLE_SIZE; i++) {
            if (keys[i] == NULL) break;
        }
        char* keyspace = (char*)malloc((TOKEN_SIZE+1)*sizeof(char));
        strcpy(keyspace,string);
        keys[i] = keyspace;
    }
}


int countDigits(int num) {
    int count = 0;
    while (num != 0) {
        num /= 10;
        count++;
    }
    return count;
}

struct intStack *stateStack;
struct Stack *tokenStack;


char* arithmetic(struct token* operator, struct token* leftoperand, struct token* rightoperand){
    char* left = leftoperand->value;
    char* right = rightoperand->value;
    int digit = countDigits(intermediateVariableIndex);
    intermediateVariableIndex += 1;
    char* result = (char*)calloc(2 + countDigits(intermediateVariableIndex),sizeof(char));
    int strsize = countDigits(intermediateVariableIndex) + (int)strlen(left) + (int)strlen(right);
    str* resulting = (str*)malloc(sizeof(str)+sizeof(char)*(strsize+17));
    switch(operator->type){
        case MULT:{
            switch (operator->value[0]){
                case '*':
                    strsize += 14;
                    sprintf(resulting->text,"%%%d = mul i32 %s,%s\n",intermediateVariableIndex, left, right);
                    break;
                case '/':
                    strsize += 15;
                    sprintf(resulting->text,"%%%d = sdiv i32 %s,%s\n",intermediateVariableIndex, left, right);
                    break;
                case '%':
                    strsize += 15;
                    sprintf(resulting->text,"%%%d = srem i32 %s,%s\n",intermediateVariableIndex, left, right);
                    break;
            }
            break;
        }
        case ADD:{
            switch (operator->value[0]){
                case '+':
                    strsize += 14;
                    sprintf(resulting->text,"%%%d = add i32 %s,%s\n",intermediateVariableIndex, left, right);
                    break;
                case '-':
                    strsize += 14;
                    sprintf(resulting->text,"%%%d = sub i32 %s,%s\n",intermediateVariableIndex, left, right);
                    break;
            }
            break;
        }
        case AND:{
            strsize += 14;
            sprintf(resulting->text,"%%%d = and i32 %s,%s\n",intermediateVariableIndex, left, right);
            break;
        }
        case OR:{
            strsize += 13;
            sprintf(resulting->text,"%%%d = or i32 %s,%s\n",intermediateVariableIndex, left, right);
            break;
        }
        default:{
            printf("%s","Something has gone really, really wrong.");
            break;
        }
    }
    resulting->size = strsize;
    resulting->next = NULL;
    lastString->next = resulting;
    lastString = resulting;
    sprintf(result,"%%%d",intermediateVariableIndex);
    return result;
}

int getFunction(const char* function){
    if (*function == '\0') return -1;
    switch (*function){
        case 'n':
            return 0;
        case 'x':
            return 1;
        case 'l':{
            if (*(function+1) == 's'){
                return 2;
            }
            if (*(function+1) == 'r'){
                return 3;
            }
        }
        case 'r':{
            if (*(function+1) == 's'){
                return 4;
            }
            if (*(function+1) == 'r'){
                return 5;
            }
        }
    }
    return -1;
}

int strsizeof(char* str){
    int size = 0;
    while (str != NULL) {
        size++;
        str++;
    }
    return size;
}


char* evaluate(struct token* function, struct token* leftoperand, struct token* rightoperand){
    char* left = leftoperand->value;
    int strsize = 0;
    str* resulting = NULL;
    intermediateVariableIndex += 1;
    if (rightoperand == NULL){
        strsize = snprintf(NULL, 0, "%%%d = xor i32 %s,-1\n",intermediateVariableIndex, left) + 1;
        resulting = malloc(sizeof(str) + strsize + 1);
        snprintf(resulting->text, strsize + 1, "%%%d = xor i32 %s,-1\n",intermediateVariableIndex, left);
    }
    char* right = rightoperand->value;
    switch (getFunction(function->value)){
        case 0:
            break;
        case 1:
            strsize = snprintf(NULL, 0, "%%%d = xor i32 %s,%s\n",intermediateVariableIndex, left, right) + 1;
            resulting = malloc(sizeof(str) + sizeof(char)*strsize);
            snprintf(resulting->text, strsize + 1, "%%%d = xor i32 %s,%s\n",intermediateVariableIndex, left, right);
            break;
        case 2:
            strsize = snprintf(NULL, 0, "%%%d = shl i32 %s,%s\n",intermediateVariableIndex, left, right) + 1;
            resulting = malloc(sizeof(str) + sizeof(char)*strsize);
            snprintf(resulting->text, strsize + 1, "%%%d = shl i32 %s,%s\n",intermediateVariableIndex, left, right);
            break;
        case 3:
            strsize = snprintf(NULL, 0, "%%%d = shl i32 %s,%s\n",intermediateVariableIndex, left, right);
            resulting = malloc(sizeof(str) + sizeof(char)*(strsize + 1));
            //snprintf does not count for the trailing null, so add it to the sum
            int s1 = snprintf(resulting->text, strsize + 1, "%%%d = shl i32 %s,%s\n",intermediateVariableIndex, left, right);
            int result1 = intermediateVariableIndex;
            intermediateVariableIndex++;

            int res2 = snprintf(NULL, 0, "%%%d = sub i32 32,%s\n",intermediateVariableIndex, right) + 1;
            strsize += res2;
            resulting = realloc(resulting, sizeof(str) + sizeof(char)*(strsize));
            int s2 = snprintf(resulting->text + s1, res2 + 1, "%%%d = sub i32 32,%s\n",intermediateVariableIndex, right);
            intermediateVariableIndex++;

            int res3 = snprintf(NULL, 0, "%%%d = lshr i32 %s,%%%d\n",intermediateVariableIndex, left, intermediateVariableIndex - 1) + 1;
            strsize += res3;
            resulting = realloc(resulting, sizeof(str) +  sizeof(char)*(strsize));
            int s3 = snprintf(resulting->text + s1 + s2, res3 + 1, "%%%d = lshr i32 %s,%%%d\n",intermediateVariableIndex, left, intermediateVariableIndex - 1);
            int result2 = intermediateVariableIndex;
            intermediateVariableIndex++;

            int res4 = snprintf(NULL, 0, "%%%d = or i32 %%%d,%%%d\n",intermediateVariableIndex, result1, result2) + 1;
            strsize += res4;
            resulting = realloc(resulting, sizeof(str) +  sizeof(char)*(strsize));
            snprintf(resulting->text + s1 + s2 + s3, res4 + 1, "%%%d = or i32 %%%d,%%%d\n",intermediateVariableIndex, result1, result2) + 1;
            break;
        case 4:
            strsize = snprintf(NULL, 0, "%%%d = ashr i32 %s,%s\n",intermediateVariableIndex, left, right);
            resulting = malloc(sizeof(str) + sizeof(char)*(strsize + 1));
            snprintf(resulting->text, strsize + 1, "%%%d = ashr i32 %s,%s\n",intermediateVariableIndex, left, right);
            break;
        case 5:
            strsize = snprintf(NULL, 0, "%%%d = lshr i32 %s,%s\n",intermediateVariableIndex, left, right);
            resulting = malloc(sizeof(str) + sizeof(char)*(strsize + 1));
            //snprintf does not count for the trailing null, so add it to the sum
            int s4 = snprintf(resulting->text, strsize + 1, "%%%d = lshr i32 %s,%s\n",intermediateVariableIndex, left, right);
            int result3 = intermediateVariableIndex;
            intermediateVariableIndex++;

            int res5 = snprintf(NULL, 0, "%%%d = sub i32 32,%s\n",intermediateVariableIndex, right) + 1;
            strsize += res5;
            resulting = realloc(resulting, sizeof(str) +  sizeof(char)*(strsize));
            int s5 = snprintf(resulting->text + s4, res5 + 1, "%%%d = sub i32 32,%s\n",intermediateVariableIndex, right);
            intermediateVariableIndex++;

            int res6 = snprintf(NULL, 0, "%%%d = shl i32 %s,%%%d\n",intermediateVariableIndex, left, intermediateVariableIndex - 1) + 1;
            strsize += res6;
            resulting = realloc(resulting, sizeof(str) +  sizeof(char)*(strsize));
            int s6 = snprintf(resulting->text + s4 + s5, res6 + 1, "%%%d = shl i32 %s,%%%d\n",intermediateVariableIndex, left, intermediateVariableIndex - 1);
            int result4 = intermediateVariableIndex;
            intermediateVariableIndex++;

            int res7 = snprintf(NULL, 0, "%%%d = or i32 %%%d,%%%d\n",intermediateVariableIndex, result3, result4) + 1;
            strsize += res7;
            resulting = realloc(resulting, sizeof(str) +  sizeof(char)*(strsize));
            snprintf(resulting->text + s4 + s5 + s6, res7 + 1, "%%%d = or i32 %%%d,%%%d\n",intermediateVariableIndex, result3, result4) + 1;
            break;
    }
    resulting->size = strsize;
    resulting->next = NULL;
    lastString->next = resulting;
    lastString = resulting;
    char* result = (char*)calloc(2 + countDigits(intermediateVariableIndex), sizeof(char));
    sprintf(result,"%%%d",intermediateVariableIndex);
    return result;
}

void goTo(int state){
    i_push(stateStack, state);
}

void shift(int state, struct token *token){
    struct token *newtoken = token;
    push(tokenStack, newtoken);
    token = NULL;
    goTo(state);
}

void reduce(int rule){
    switch (rule){
        case 0:{
            struct token* token = (struct token*) pop(tokenStack);
            token->type = Sp;
            push(tokenStack, token);
            break;
        }
        case 1:{
            struct token* token = (struct token*) pop(tokenStack);
            token->type = S;
            push(tokenStack, token);
            break;
        }
        case 2:{
            struct token* expression = (struct token*) pop(tokenStack);
            pop(tokenStack);
            struct token* var = (struct token*) peek(tokenStack);
            put(var->value);
            var->type = S;
            isAssigned = 1;
            i_pop(stateStack);
            i_pop(stateStack);
            int strsize = 19 + (int)strlen(expression->value) + (int)strlen(var->value);
            str* resulting = (str*)malloc(sizeof(str)+sizeof(char)*(strsize+1));
            sprintf(resulting->text,"store i32 %s, i32* %%%s\n", expression->value, var->value);
            resulting->size = strsize;
            resulting->next = NULL;
            lastString->next = resulting;
            lastString = resulting;
            break;
        }
        case 3:{
            pop(tokenStack);
            struct token* token = (struct token*) pop(tokenStack);
            pop(tokenStack);
            push(tokenStack, token);
            i_pop(stateStack);
            i_pop(stateStack);
            break;
        }
        case 4:{
            pop(tokenStack);
            struct token* rightOperand = (struct token*) pop(tokenStack);
            pop(tokenStack);
            struct token* leftOperand = (struct token*) pop(tokenStack);
            pop(tokenStack);
            struct token* function = (struct token*) pop(tokenStack);
            char* value = evaluate(function, leftOperand, rightOperand);
            int len = (int)strlen(value);
            struct token *newtoken = tokenize(value, len);
            newtoken->type = E;
            push(tokenStack, newtoken);
            i_pop(stateStack);
            i_pop(stateStack);
            i_pop(stateStack);
            i_pop(stateStack);
            i_pop(stateStack);
            break;
        }
        case 5:{
            pop(tokenStack);
            struct token* leftOperand = (struct token*) pop(tokenStack);
            pop(tokenStack);
            struct token* function = (struct token*) pop(tokenStack);
            char* value = evaluate(function, leftOperand, NULL);
            struct token *newtoken = tokenize(value, (int) strlen(value));
            newtoken->type = E;
            push(tokenStack, newtoken);
            i_pop(stateStack);
            i_pop(stateStack);
            i_pop(stateStack);
            break;
        }
        case 6:
        case 7:
        case 8:
        case 9:{
            struct token* rightOperand = (struct token*) pop(tokenStack);
            struct token* operator = (struct token*) pop(tokenStack);
            struct token* leftOperand = (struct token*) pop(tokenStack);
            //printf("%s %s %s\n",rightOperand->value,operator->value,leftOperand->value);
            char* value = arithmetic(operator, leftOperand, rightOperand);
            struct token *newtoken = tokenize(value, (int) strlen(value));
            newtoken->type = E;
            push(tokenStack, newtoken);
            i_pop(stateStack);
            i_pop(stateStack);
            break;
        }
        case 10:{
            struct token* var = (struct token*) peek(tokenStack);
            if (exists(var->value) == 0) {
                printf("Error in line %d!\n", line);
                errorFoundUninitVar = 1;
                var->type = E;
                break;
            }
            intermediateVariableIndex += 1;
            int strsize = 21 + countDigits(intermediateVariableIndex) + (int)strlen(var->value);
            str* resulting = (str*)malloc(sizeof(str)+sizeof(char)*(strsize+1));
            sprintf(resulting->text,"%%%d = load i32, i32* %%%s\n", intermediateVariableIndex,var->value);
            resulting->size = strsize;
            resulting->next = NULL;
            lastString->next = resulting;
            lastString = resulting;
            var->type = E;
            sprintf(var->value,"%%%d",intermediateVariableIndex);
            break;
        }
        case 11:{
            struct token* token = (struct token*) pop(tokenStack);
            token->type = E;
            push(tokenStack, token);
            break;
        }
        default:{
            printf("%s","Something has gone really, really wrong.");
            break;
        }
    }
}

int hasError = 0;

int main(int argc, char** argv){
    for (int i = 0; i < TABLE_SIZE; i++){
        keys[i] = NULL;
    }
    compileregex();

    str* middleString = calloc(1, sizeof(str) + 1);
    middleString -> size = 0;
    middleString -> next = NULL;
    sprintf(middleString -> text, "%s", "");
    lastString = middleString;


    FILE *fp = fopen("test.ll", "w");
    FILE *fr = fopen("test.adv", "r");


    //start reading input
    while (fgets(msgbuf, sizeof(msgbuf), fr)) {

        errorFoundUninitVar = 0;


        //allocate memory for tokens
        struct token* tokens = calloc(TOKEN_SIZE, sizeof(struct token));
        lexer(tokens);
        line++;
        //parsing block
        i_init(&stateStack);
        init(&tokenStack);
        i_push(stateStack, 0);
        int condition = 1;
        int reduced = 0;
        int step = 0;

        while (condition) {
            if ((step == 0) && (tokens[0].type == EOL)){
                break;
            }
            if (tokenIndex == TOKEN_SIZE-1) {
                //printf("Error!\n");
                break;
            }

            if(errorFoundUninitVar) break;


            struct token *nextToken;
            if (reduced) {
                nextToken = ((struct token *) peek(tokenStack));
            } else {
                nextToken = &tokens[tokenIndex];
            }

            int type = (*nextToken).type;
            int currentState = i_pop(stateStack);

            int action = parsingTable[currentState][type][0];
            int targetState = parsingTable[currentState][type][1];

            reduced = 0;
            step++;
            switch (action) {
                //accept the statement
                case -1: {
                    if (isAssigned == 0) {
                        int strsize = snprintf(NULL, 0, "call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s )\n", peek(tokenStack)->value) + 1;
                        str* resulting = (str*)malloc(sizeof(str)+sizeof(char)*(strsize+1));
                        sprintf(resulting->text,"call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s )\n", peek(tokenStack)->value);
                        resulting->size = strsize;
                        resulting->next = NULL;
                        lastString->next = resulting;
                        lastString = resulting;
                        intermediateVariableIndex++;
                    }
                    condition = 0;
                    break;
                }
                //error
                case 0:
                    if (errorFoundUninitVar == 0) printf("Error in line %d!\n", line);
                    hasError = 1;
                    condition = 0;
                    break;
                //shift
                case 1:
                    i_push(stateStack, currentState);
                    shift(targetState, nextToken);
                    nextToken = NULL;
                    tokenIndex++;
                    break;
                //reduce
                case 2:
                    reduce(targetState);
                    reduced = 1;
                    break;
                //goto
                case 3:
                    i_push(stateStack, currentState);
                    goTo(targetState);
                    break;
                default:{
                    printf("%s","Something has gone really, really wrong.");
                    break;
                }
            }
        }
        free(stateStack);
        free(tokenStack);
        free(tokens);
        tokenIndex = 0;
        isAssigned = 0;
    }
    regfree(&regex);
    str* variableDeclarations = declareAll();
    firstString.next = variableDeclarations;
    while (variableDeclarations->next != NULL) variableDeclarations = variableDeclarations->next;
    variableDeclarations->next = middleString;
    str* endString = (str*)malloc(sizeof(str)+sizeof(char)*12);
    endString->size = 11;
    endString->next = NULL;
    strcpy(endString->text,"ret i32 0\n}");
    lastString->next = endString;
    if (hasError) return 0;
    fprintf(fp, "%s",linkStrings(&firstString));
}

