#include "headers.h"

void print(char* target, char* filename){
    printf("%s/%s\n", target, filename);
}

void recur_ls(char* target, char* filename, int f_flag, int d_flag, char* cwd, int recur_flag){
    chdir(target);

    struct stat st;
    struct dirent **namelist;
    
    int dir_size;
    dir_size = scandir(".", &namelist, NULL, alphasort);

    for(int i=0; i<dir_size; i++){
        if(strcmp(filename, "")==0){
            if(d_flag){
                if(stat(namelist[i]->d_name, &st)==0 && S_ISDIR(st.st_mode)){
                    print(target, namelist[i]->d_name);
                    if(strcmp(namelist[i]->d_name, ".")!=0 && strcmp(namelist[i]->d_name, "..")!=0)
                        recur_ls(namelist[i]->d_name, filename, f_flag, d_flag, cwd, 1);
                }
            }
            if(f_flag){
                if(stat(namelist[i]->d_name, &st)==0 && !S_ISDIR(st.st_mode))
                    print(target, namelist[i]->d_name);
                else if (stat(namelist[i]->d_name, &st)==0 && S_ISDIR(st.st_mode))
                    if(strcmp(namelist[i]->d_name, ".")!=0 && strcmp(namelist[i]->d_name, "..")!=0){
                        recur_ls(namelist[i]->d_name, filename, f_flag, d_flag, cwd, 1);
                }
            }
        }
        else{
            if(strcmp(namelist[i]->d_name, filename)==0){
                if(d_flag){
                    if(stat(namelist[i]->d_name, &st)==0 && S_ISDIR(st.st_mode)){
                        print(target, filename);
                        if(strcmp(namelist[i]->d_name, ".")!=0 && strcmp(namelist[i]->d_name, "..")!=0)
                            recur_ls(namelist[i]->d_name, filename, f_flag, d_flag, cwd, 1);
                    }
                }
                if(f_flag){
                    if(stat(namelist[i]->d_name, &st)==0 && !S_ISDIR(st.st_mode))
                        print(target, filename);
                    else if (stat(namelist[i]->d_name, &st)==0 && S_ISDIR(st.st_mode)){
                        recur_ls(namelist[i]->d_name, filename, f_flag, d_flag, cwd, 1);
                    }
                }
            }
            else{
                if(stat(namelist[i]->d_name, &st)==0 && S_ISDIR(st.st_mode)){
                    if(strcmp(namelist[i]->d_name, ".")!=0 && strcmp(namelist[i]->d_name, "..")!=0)
                        recur_ls(namelist[i]->d_name, filename, f_flag, d_flag, cwd, 1);
                }
            }
        }
        free(namelist[i]);
    }

    if(!recur_flag) chdir(cwd);
    else chdir("..");
    return;
}

void discover(char* input, char* root_dir){
    char cwd[256];
    getcwd(cwd, sizeof(cwd));

    char* token;
    char* target, *filename;

    sprintf(target, "");
    sprintf(filename, "");

    int flag=0, d_flag=0, f_flag=0;

    sprintf(target, "%s", cwd);

    token = strtok_r(input, " \n\r\t\v\f", &input);

    while(token = strtok_r(input, " \n\r\t\v\f", &input)){
        if(flag==0 && token[0]!='-' && token[0]!='\"'){
            if(token[0]=='~')
                sprintf(target, "%s%s", root_dir, &token[1]);
            else
                sprintf(target, "%s", token);
            flag=1;
        }

        else if (token[0]=='-'){
            for(int i=1; i<strlen(token); i++){
                if(token[i]=='d')
                    d_flag=1;
                else if (token[i]=='f')
                    f_flag=1;
            }
        }

        else if (token[0]=='\"'){
            snprintf(filename, strlen(token)-1, &token[1]);
        }
    }

    if(f_flag==0 && d_flag==0){
        f_flag=1;
        d_flag=1;
    }

    recur_ls(target, filename, f_flag, d_flag, cwd, 0);
}