#include "headers.h"

void Format(char* input, char* format, char* root_dir){
    if(strcmp(input, "ls\n")==0){
        sprintf(format, " ");
        return;
    }

    int flagger=1;

    for(int i = 2; i<strlen(input); i++){
        if((input[i]>=9 && input[i]<=13) || (input[i]==32)){
            flagger=1;
        }
        else{
            flagger=0;
            break;
        }
    }

    if(flagger==1){
        sprintf(format, " ");
        return;
    }

    char flags[4];
    sprintf(flags, "");

    int mark=0;

    char* inp_cpy = (char*)malloc((strlen(input)+1)*sizeof(char));
    strcpy(inp_cpy, input);

    char* token;

    int flag=0, flag2=0;

    while(token = strtok_r(input, " \n\r\t\v\f", &input)){
        if(mark == 0){
            if(strncmp(token, "-", 1) == 0){
                for(int i=1; i<=strlen(token); i++){
                    if(token[i] == 'l')
                        flag=1;
                    else if(token[i] == 'a')
                        flag2=1;
                }
            }
        }
    }

    if(flag == 1 && flag2 == 0){
        sprintf(flags, "-l");
    }

    else if(flag == 0 && flag2 == 1){
        sprintf(flags, "-a");
    }

    else if(flag == 1 && flag2 == 1){
        sprintf(flags, "-la");
    }


    if (mark==1)
        sprintf(flags, "%s ", flags);

    char* direct = (char*)malloc(512*sizeof(char));
    char* abs_path = (char*)malloc(256*sizeof(char));

    sprintf(direct, "");  
    sprintf(abs_path, "");  
    
    char* token2;
    mark=0;

    while(token2 = strtok_r(inp_cpy, " \n\r\t\v\f", &inp_cpy)){
        if(mark == 0){
            if(strncmp(token2, "-", 1) != 0 && strncmp(token2, "ls", 2) != 0){
                if (strncmp(token2, "~", 1) == 0)
                    sprintf(abs_path, "%s%s", root_dir, &token2[1]);
                else
                    sprintf(abs_path, "%s", token2);
                sprintf(direct, "%s ", abs_path);
                mark=1;
            }
        }

        else{
            if(strncmp(token2, "-", 1) != 0){
                if (strncmp(token2, "~", 1) == 0)
                    sprintf(abs_path, "%s%s", root_dir, &token2[1]);
                else
                    sprintf(abs_path, "%s", token2);
                sprintf(direct, "%s%s ", direct, abs_path);
            }
        }
    }

    if(strcmp(direct, "") == 0) sprintf(format, "%s", flags);
    else sprintf(format, "%s %s", flags, direct);
}