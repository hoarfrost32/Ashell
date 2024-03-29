#include "headers.h"

void commands(char* token, char* cwd, char* copy_cwd, char* copy_root, char* prev){
    char* token2 = (char*)malloc((strlen(token)+1)*sizeof(char));
    strcpy(token2, token);

    char* token3;

    char* cpy_tok2 = (char*)malloc((strlen(token2)+1)*sizeof(char));
    strcpy(cpy_tok2, token2);

    token3 = strtok_r(token2, " \n\t\r\v\f", &token2);

    if((strcmp(token3, "pwd")==0)) 
        pwd();

    else{ 
        if (strcmp(token3, "echo")==0) 
            echo(cpy_tok2);

        else{ 
            if (strcmp(token3, "cd")==0){
                cd(cpy_tok2, copy_root, cwd, prev);
                strcpy(prev, copy_cwd);
            }
            else{ 
                if(strcmp(token3, "ls")==0){
                    ls(cpy_tok2, copy_root);
                }
                else{
                    if (strcmp(token3, "clear")==0) 
                        clear();
                    else{
                        if(strcmp(token3, "discover")==0){
                            discover(cpy_tok2, copy_root);
                        }
                        else{
                            if(strcmp(token3, "history")==0){
                                hist(copy_root);
                            }
                            else{
                                if(strcmp(token3, "pinfo")==0){
                                    pinfo(cpy_tok2);
                                }

                                else{
                                    fork_exec(token3, cpy_tok2, cwd, copy_root, prev);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void parse(char* cwd, char* copy_cwd, char* copy_root, char* prev){
    char* input; size_t inp_len=1000;
    input = (char *)malloc(inp_len*sizeof(char)+1);
    getline(&input, &inp_len, stdin);

    int flagger=0;

    for(int i = 0; i<strlen(input); i++){
        if((input[i]>=9 && input[i]<=13) || (input[i]==32))
            flagger++;
    }

    if(flagger==strlen(input))
        return;

    chdir(copy_root);

    FILE* fptr = fopen(".buffer.txt", "a+");

    char* line;
    size_t len = 256;
    int ret, count=0;

    line=(char*) malloc(len*sizeof(char));

    while(ret=getline(&line, &len, fptr)!=-1)
        count++;

    if(strcmp(line, input)!=0)
        fprintf(fptr, "%s", input);
    
    rewind(fptr);

    FILE *fptr2 = fopen(".history.txt", "w+");

    while(ret=getline(&line, &len, fptr)!=-1){
        count--;
        if(count<19)
            fprintf(fptr2, "%s", line);
    }

    rewind(fptr), rewind(fptr2);

    // remove(".temp.txt");

    chdir(cwd);

    char* token, *cpy_tok1, *token2;
    while(token = strtok_r(input, ";", &input)){
        cpy_tok1 = (char*) malloc((strlen(token)+1)*sizeof(char));
        strcpy(cpy_tok1, token);
        commands(cpy_tok1, cwd, copy_cwd, copy_root, prev);    
    }
}