#include "headers.h"

time_t tick, tock;

void commands(char* token, char* cwd, char* root, char* prev){

    char* cpy_tok1 = (char*)malloc((strlen(token)+1)*sizeof(char));
    strcpy(cpy_tok1, token);

    char* to_free_cpytok_blk = cpy_tok1;

    char* token2;
    token2 = strtok_r(cpy_tok1, " \n\t\r\v\f", &cpy_tok1);

    if((strcmp(token2, "pwd")==0))
        pwd();

    else{ 
        if (strcmp(token2, "echo")==0)
            echo(cpy_tok1);
        else{ 
            if (strcmp(token2, "cd")==0){
                
                char* new_cwd = (char *)malloc(__PATH_MAX__ * sizeof(char)); 
                cd(cpy_tok1, root, cwd, prev, new_cwd);
                
                if (strcmp(new_cwd, cwd) != 0)
                    strcpy(prev, cwd);

                free(new_cwd);
            }
            else{ 
                if(strcmp(token2, "ls")==0)
                    ls(token, root);
                else{
                    if (strcmp(token2, "clear")==0) 
                        printf("\e[1;1H\e[2J");
                    else{
                        if(strcmp(token2, "discover")==0){
                            discover(token, root);
                        }
                        else{
                            if(strcmp(token2, "history")==0){
                                hist(root);
                            }
                            else{
                                if(strcmp(token2, "pinfo")==0){
                                    pinfo(token);
                                }
                                else{
                                    fork_exec(token2, token, cwd, root, prev);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    free(to_free_cpytok_blk);
}

int parse(char* cwd, char* root, char* prev){

    time_t tm = 0;
    
    // The input to the shell can be no longer than 1000 characters.
    // This is not the restriction on the length of any one command;
    // rather the restriction is on an `input`, meaning situations 
    // like passing in cmds separated by `;`, `&` are included.

    char* input; size_t inp_len=1000;
    input = (char *)malloc((inp_len + 1) * sizeof(char));
    getline(&input, &inp_len, stdin);

    char* to_free_inp_blk = input;

    int flagger=0;

    for(int i = 0; i<strlen(input); i++)
        if((input[i]>=9 && input[i]<=13) || (input[i]==32))
            flagger++;

    if(flagger==strlen(input)){
        free(to_free_inp_blk);
        return 0;
    }

    chdir(root);

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

        free(line);

    chdir(cwd);

    char* token, *cpy_tok, *token2;
    char* saveptr;

    while(token = strtok_r(input, ";", &saveptr)){
        input = NULL;
        cpy_tok = (char*) malloc((strlen(token)+1)*sizeof(char));
        strcpy(cpy_tok, token);
        
        char* to_free_cpy_tok = cpy_tok;

        // Need to calculate time taken to execute fg processes.
        // This is then summed up and returned to main(), which updates
        // a local variable there and in every execution of the while loop
        // passes the cumulative fg processes run time in the previous call 
        // to parse() to the prompt function.
        tick = time(NULL);
            commands(cpy_tok, cwd, root, prev);    
        tock = time(NULL);

        tm += tock - tick;

        free(to_free_cpy_tok);
    }

    free(to_free_inp_blk);

    return (tm);
}