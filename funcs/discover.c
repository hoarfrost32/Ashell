#include "../headers.h"

void print(char* filename){
    char cwd[__PATH_MAX__];
    getcwd(&cwd[0], __PATH_MAX__);
    printf("%s/%s\n", cwd, filename);
}

void recur_ls(char* target, char* filename, int f_flag, int d_flag, char* cwd, int recur_flag){
    chdir(target);

    struct stat st;
    struct dirent **namelist;

    int dir_size;
    dir_size = scandir(".", &namelist, NULL, alphasort);

    for(int i=0; i<dir_size; i++){
        if (lstat(namelist[i]->d_name, &st) == 0){
            if(S_ISDIR(st.st_mode)){
                if(strcmp(filename, "")==0 && d_flag) print(namelist[i]->d_name);
                else if (strcmp(namelist[i]->d_name, filename) == 0) print(filename);

                if(!S_ISLNK(st.st_mode) && strcmp(namelist[i]->d_name, ".")!=0 && strcmp(namelist[i]->d_name, "..")!=0){
                    char go_back_to[__PATH_MAX__];
                    getcwd(go_back_to, sizeof(go_back_to)); 
                    recur_ls(namelist[i]->d_name, filename, f_flag, d_flag, go_back_to, 1);
                }
            }
            else{
                if(strcmp(filename, "")==0 && f_flag) print(namelist[i]->d_name);
                else if (strcmp(namelist[i]->d_name, filename) == 0) print(filename);
            }
        }

        free(namelist[i]);
    }

    free(namelist);
    chdir(cwd);

    return;
}

void discover(char* input, char* root_dir){
    char cwd[__PATH_MAX__];
    getcwd(cwd, sizeof(cwd));

    char* target = (char *)malloc(__PATH_MAX__ * sizeof(char));
    char* filename = (char *)malloc(__PATH_MAX__ * sizeof(char));

    int d_flag = 0, f_flag = 0;

    sprintf(filename, "");
    sprintf(target, "%s", cwd);

    int flag=0;

    char* token;
    token = strtok_r(input, " \n\r\t\v\f", &input);

    while(token = strtok_r(input, " \n\r\t\v\f", &input)){

        // Checks to see if a target directory has been provided.
        // If that is the case, then constructs an absolute path
        // to the target directory.
        if(token[0]!='-' && token[0]!='\"'){
            if(token[0]=='~')
                sprintf(target, "%s%s", root_dir, &token[1]);
            else
                sprintf(target, "%s", token);
        }

        // Checks to see if either of the -d/-f flags are passed.        
        else if (token[0]=='-')
            for(int i=1; i<strlen(token); i++){
                if(token[i]=='d') d_flag=1;
                else if (token[i]=='f') f_flag=1;
            }

        // Checks whether a target filename has been passed. 
        else if (token[0]=='\"')
            snprintf(filename, strlen(token)-1, &token[1]);
    }

    if((strcmp(filename, "") == 0) && !(d_flag | f_flag)){
        f_flag=1;
        d_flag=1;
    }

    recur_ls(target, filename, f_flag, d_flag, cwd, 0);

    free(filename);
    free(target);
}