#include "../headers.h"


void print_pinfo(int pid){
    
    // Print the Process ID.
    printf("pid: %d\n", pid);

    //Access the proc_info from the proc file.
    char path[__PATH_MAX__];
    sprintf(path, "/proc/%d/stat", pid);

    FILE* fp = fopen(path, "r");

    if(fp==NULL){
        printf("No Process with pid %d exists\n", pid);
    }

    else {
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

        buffer = (char* )malloc(__PATH_MAX__*sizeof(char));

        readlink(path, buffer, __PATH_MAX__ - 1);

        printf("Executable Path: %s\n", buffer);

        free(buffer);
    }
}

void pinfo(char* input){
    char* token;
    token = strtok_r(input, " \n\r\t\v\f", &input);

    pid_t pid = 0;

    if(token=strtok_r(NULL, " \n\r\t\v\f", &input))
        pid = atoi(token);
    else pid = getpid();

    print_pinfo(pid);
}