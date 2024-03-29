#include "headers.h"

char re[256], prev[256], root_dir[256], copy_root[256];

void prompt(char* cwd, char* copy_cwd, char* copy_root, char* prev, int exec_time){
    if(exec_time<1){
        printf(GREEN);
        printf(">");
        printf(DEFCOL);
    }

    else{
        printf(RED);
        printf("took %d seconds\n", exec_time);
        printf(GREEN);
        printf(">");
        printf(DEFCOL);

        return;
    }

    parse(cwd, copy_cwd, copy_root, prev);
}

int main(){
    getcwd(root_dir, 256);

    while(1){
        strcpy(copy_root, root_dir);

        char sysname[50];
        gethostname(sysname, 50);
        
        struct passwd *username = getpwuid(getuid());
        char* uname = username->pw_name;

        char cwd[256], copy_cwd[256]; 
        getcwd(cwd, 256);

        strcpy(copy_cwd, cwd);

        if(strncmp(root_dir, cwd, strlen(root_dir))==0) sprintf(re, "~%s", &cwd[strlen(root_dir)]);
        else sprintf(re, "%s", cwd);

        printf(GREEN);
        printf("<%s@%s:", uname, sysname);

        printf(BLUE);
        printf("%s", re);

        prompt(cwd, copy_cwd, copy_root, prev, 0);

        //Functions that have been built into this shell are:
        //<------------------------------------------------->
        // - cd
        // - ls
        // - pwd
        // - echo
        // - clear 
        // - history
        // - pinfo
        // - discover
        // Additionally, system commands (vim, mkdir, touch) can also be executed in this shell.
        //<------------------------------------------------->
    }

    return 0;
}