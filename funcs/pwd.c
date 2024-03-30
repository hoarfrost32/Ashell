#include "../headers.h"

void pwd(){
    char cwd[__PATH_MAX__]; 
    getcwd(cwd, __PATH_MAX__);     
    printf("%s\n", cwd);
}