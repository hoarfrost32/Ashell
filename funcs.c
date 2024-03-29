#include "headers.h"

void pwd(){
    char cwd[256]; 
    getcwd(cwd, 256);     
    printf("%s\n", cwd);
}

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

void pathFinder(char* cwd, char* path, int flag1, int flag2){
    if(strcmp(cwd, "/home")==0){ 
        chdir("/");
        free(path);
        return;
    }

    char* token2, *dummy;

    token2 = strtok(cwd, "/");
    dummy = strtok(NULL, "/");

    while(dummy != NULL){
        if(flag1==0){
            strcpy(path, "/");
            strcat(path, token2);
        }
        else{
            strcat(path, "/");
            strcat(path, token2);
        }
        flag1=1;
        strcpy(token2, dummy);
        dummy = strtok(NULL, "/");
    }

    if (flag2==1){
        strcat(path, "/");
        strcat(path, token2);
    }

    chdir(path);
    free(path);
}

void hist(char* root_dir){
    char cwd[256];
    getcwd(cwd, 256);

    chdir(root_dir);

    FILE* fptr = fopen(".history.txt", "r");
    char* line;
    size_t len = 256;
    int ret, count=0;

    line=(char*) malloc(len*sizeof(char));

    while(ret=getline(&line, &len, fptr)!=-1)
        count++;

    rewind(fptr);

    if(count<=10)
        while(ret=getline(&line, &len, fptr)!=-1)
            fwrite(line, ret, strlen(line), stdout);

    else{
        while(ret=getline(&line, &len, fptr)!=-1){
            count--;
            if(count<10)
                fwrite(line, ret, strlen(line), stdout);
        }
    }

    chdir(cwd);

    free(line);
    fclose(fptr);
}

void echo(char* input){
    char* token;
    token = strtok(input, " \"\t\n\r\v\f");

    int flag = 0;

    while (token != NULL){
        if(flag==1) printf("%s ", token);
        token = strtok(NULL, " \"\t\n\r\v\f");
        flag=1;
    }

    printf("\n");
}

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

void cd(char* input, char* root_dir, char* cwd, char* prev){

    char* token;
    token = strtok(input, " \t\n\r\v\f");

    int flag = 0;

    while (token != NULL){
        token = strtok(NULL, " \t\n\r\v\f");
        flag++; 

        if(flag==1 && token != NULL){
            //<--------------------------------------------------------->
            if(strcmp(token, "..")==0){
                char* path = (char *)malloc(strlen(cwd)*sizeof(char)+1);
                pathFinder(cwd, path, 0, 0);
            }
            //<--------------------------------------------------------->
            else if (strcmp(token, "~")==0) chdir(root_dir);
            //<--------------------------------------------------------->
            else if (strcmp(token, "-")==0){
                printf("%s\n", prev);
                chdir(prev);
            }
            //<--------------------------------------------------------->
            else {
                if(strncmp(root_dir, token, strlen(root_dir))==0){
                    chdir(token);
                }
                else{
                    if (token[0]!=126) chdir(token);

                    else if (token[0]==126){
                        char* path = (char *)malloc(strlen(root_dir)*sizeof(char));
                        strcpy(path, root_dir);
                        
                        char* source = (char *)malloc(strlen(token)*sizeof(char));
                        strcpy(source, &token[1]);
                        
                        pathFinder(source, path, 1, 1);
                    }
                }
            }
            continue;
        }
        else if (flag>=2 && token != NULL){
            printf("ERROR: Invalid number of command line arguments\n");
            chdir(cwd);
            return;
        } 
        else if (flag>=2 && token == NULL) return;
        flag=0;
    }

    if(flag==0) chdir(root_dir);
    return;
}

void clear(){
    printf("\e[1;1H\e[2J");
}