#include "../headers.h"

void hist(char* root_dir){
    char cwd[__PATH_MAX__];
    getcwd(cwd, __PATH_MAX__);

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