#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "lexer.h"


const char *keywords[] = {"not", "xor", "ls", "rs", "lr", "rr"};
extern regex_t regex;
regmatch_t match[1];
int regexVal;
extern char msgbuf[257];

int isNumber(char *string){
    while (*string != '\0'){
        if (isdigit(*string) == 0) return 0;
        string++;
    }
    return 1;
}



//very dangerous return value on failure
//explicitly check failure with type == -1
enum type getType(char *string, int len){
    if (isNumber(string)) return I;
    else{
        if (len > 1){
            for (int i = 0; i < 6; i++){
                if (strcmp((const char*)string, (const char*)keywords[i]) == 0){
                    if (i == 0) return NOT; else return F;}
            }
            return V;
        } 
        //either f or v
        else{
            char eol[2] = "$";
            int x = strcmp((const char*)string, eol);
            if (x == 0) return EOL;
            switch(*string){
                case ',':
                    return COMM;
                case '|':
                    return OR;
                case '&':
                    return AND;
                case '=':
                    return EQ;
                case '(':
                    return OBR;
                case ')':
                    return CBR;
                case '*':
                    return MULT;
                case '/':
                    return MULT;
                case '%':
                    return MULT;
                case '+':
                    return ADD;
                case '-':
                    return ADD;
            }
        }   return V;
    }
}

//tokenizes a given string.
//does not check for whitespaces and the behavior is 
//undefined for whitespaces.
struct token *tokenize(char *string, int len){
    enum type type = getType(string, len);
    struct token *token = malloc(sizeof(struct token) + len*sizeof(char));
    token->type = type;
    strncpy(token->value, string, len+1);
    return token;
}

int compileregex(){
    //compile the regex pattern
    regexVal = regcomp(&regex, "[a-zA-Z]+|[0-9]+|[^[:alnum:]]", REG_EXTENDED);
    if (regexVal) {
        fprintf(stderr, "Regex compilation error\n");
        return 1;
    }
    return 0;
}

void lexer(struct token* tokens){
    //execute search
    regexVal = regexec(&regex, msgbuf, 1, match, 0);
    int lastPtr = 0;
    int i = 0;
    //look for all matches in the line
    while (regexVal == 0) {

        int startBuffer = match[0].rm_so;
        int endBuffer = match[0].rm_eo;
        int len = endBuffer - startBuffer;

        //copy the match to a string
        char tokenStr[len + 1];
        strncpy(tokenStr, msgbuf + lastPtr + startBuffer, len);
        tokenStr[len] = '\0';

        //check for beginning of the comment
        const char *comment = "%";
        int cmp = strcmp(tokenStr, comment);
        if (cmp == 0) break;
        //tokenize and add to array
        if (!isspace((int) tokenStr[0])) {
            struct token *token = tokenize(tokenStr, len);
            tokens[i] = *token;
            i++;
        }

        //go to next match
        lastPtr += endBuffer;
        regexVal = regexec(&regex, msgbuf + lastPtr, 1, match, 0);
    }
    struct token *eol = tokenize("$", 1);
    tokens[i] = *eol;
}

//code used for unit testing
/*
regex_t regex;
const char *strings[] = {"V", "EQ", "OBR", "CBR", "F", "COMM", "NOT", "MULT", "ADD", "AND", "OR", "I", "EOL", "Sp", "S", "E"};
regmatch_t match[1];

int regexVal;
char msgbuf[257];

struct token tokens[256];

int main(){

    regexVal = regcomp(&regex, "[a-zA-Z0-9]+|[^[:alnum:]]", REG_EXTENDED);

    if (regexVal) {
    fprintf(stderr, "Regex compilation error\n");
    return 1;
}

    while (fgets(msgbuf, sizeof(msgbuf), stdin)){
        if (msgbuf == NULL) break;
        

        regexVal = regexec(&regex, msgbuf, 1, match, 0);

        int lastPtr = 0;
        
        int i = 0;
        while (regexVal == 0){
            int startBuffer = match[0].rm_so;
            int endBuffer = match[0].rm_eo;
            int len = endBuffer - startBuffer;

            char tokenStr[len+1];
            strncpy(tokenStr, msgbuf+lastPtr+startBuffer, len);
            tokenStr[len] = '\0';
            if (!isspace((int) tokenStr[0])){
                struct token token = *tokenize(tokenStr, len);
                tokens[i] = token;
                i++;
            }

            lastPtr += endBuffer;
            regexVal = regexec(&regex, msgbuf + lastPtr, 1, match, 0);
        }

        for (int j = 0; j < i; j++){
            struct token tok = tokens[j];
            if (0<tok.type<15){
                const char *type = strings[tok.type];
                printf("type: %s value: %s\n", type, tok.value);
            }
        }
    }
    regfree(&regex);
    return 0;

}

#*/
