#include "../headers.h"

void pinfo(char* cpy_tok2){
    int flag=0;

    char* token2;
    while(token2=strtok_r(cpy_tok2, " \n\r\t\v\f", &cpy_tok2)){
        if(flag==0)
            flag=1;
        else{
            flag=2;

            char path[256];
            sprintf(path, "/proc/%s/stat", token2);

            FILE* fp = fopen(path, "r");

            if(fp==NULL){
                printf("No Process with pid %s exists\n", token2);
            }

            else{
                printf("pid: %s\n", token2);

                int count = 0; char dummy[100], status[100], VM[100];

                while(fscanf(fp, "%s", &dummy)!=EOF){
                    count++;
                    if(count==3)
                        strcpy(status, dummy);
                    else if (count==23)
                        strcpy(VM, dummy);
                    else if (count>23)
                        break;
                }

                printf("Process Status: %s", status);

                pid_t tg_pid = tcgetpgrp(STDOUT_FILENO);
                pid_t pg_pid = getpgrp();

                if(tg_pid==pg_pid){
                    printf("+\n");
                }

                printf("Memory: %s\n", VM);

                char* buffer;
                sprintf(path, "/proc/%s/exe", token2);

                buffer = (char* )malloc(256*sizeof(char));

                readlink(path, buffer, 256);

                printf("Executable Path: %s\n", buffer);
            }
        }
    }

    if (flag==1){
        pid_t pid = getpid();
        printf("pid: %d\n", pid);

        char path[256];
        sprintf(path, "/proc/%d/stat", pid);

        FILE* fp = fopen(path, "r");

        int count = 0; char dummy[100], status[100], VM[100];

        while(fscanf(fp, "%s", &dummy)!=EOF){
            count++;
            if(count==3)
                strcpy(status, dummy);
            else if (count==23)
                strcpy(VM, dummy);
            else if (count>23)
                break;
        }

        printf("Process Status: %s", status);
        
        pid_t tg_pid = tcgetpgrp(STDOUT_FILENO);
        pid_t pg_pid = getpgrp();

        if(tg_pid==pg_pid){
            printf("+\n");
        }

        printf("Memory: %s\n", VM);

        char* buffer;
        sprintf(path, "/proc/%d/exe", pid);

        buffer = (char* )malloc(256*sizeof(char));

        readlink(path, buffer, 256);

        printf("Executable Path: %s\n", buffer);
    }
}