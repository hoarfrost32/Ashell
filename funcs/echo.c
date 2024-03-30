#include "../headers.h"

void echo(char* input){

    char* token;

    while (token = strtok_r(NULL, " \"\n\t\r\v\f", &input))
        printf("%s ", token);

    printf("\n");
}