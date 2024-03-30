#include "../headers.h"

void fork_exec(char* token3, char* cpy_tok2, char* cwd, char* copy_root, char* prev){
    char* argv[21];
    char* token2;
    int count = 0, pid=getpid();

    while(token2=strtok_r(cpy_tok2, " \n\r\t\v\f", &cpy_tok2))
        argv[count++]=token2;

    argv[count]=NULL;

    int status_code;

    if (fork() == 0) {    
        status_code = execvp(token3, argv);

        if (status_code == -1) {
            printf("Error: %s is invalid input\n", token3);
            return;
        }
    }

    wait(0);

    return;
}