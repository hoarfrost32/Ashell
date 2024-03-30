#include "headers.h"

void format(char* input, char* fmt, char* root){

    if(strcmp(input, "ls\n")==0){
        sprintf(fmt, " ");
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
        sprintf(fmt, " ");
        return;
    }

    char flags[4];
    sprintf(flags, "");

    char* inp_cpy = (char*)malloc((strlen(input)+1)*sizeof(char));
    strcpy(inp_cpy, input);

    char* token;
    int a_flg=0, l_flg=0;

    while(token = strtok_r(inp_cpy, " \n\r\t\v\f", &inp_cpy)){
        if(strncmp(token, "-", 1) == 0){
            for(int i=1; i<=strlen(token); i++){
                if(token[i] == 'l')
                    l_flg=1;
                else if(token[i] == 'a')
                    a_flg=1;
            }
        }
    }

    if(a_flg == 1)
        if(l_flg == 1) sprintf(flags, "-la");
        else sprintf(flags, "-a");

    else if (l_flg == 1) sprintf(flags, "-l");

    char* direct = (char*)malloc(__PATH_MAX__*sizeof(char));
    char* abs_path = (char*)malloc(__PATH_MAX__*sizeof(char));

    sprintf(direct, "");  
    sprintf(abs_path, "");  
    
    char* token2;
    int mark=0;

    while(token2 = strtok_r(input, " \n\r\t\v\f", &input)){
        if(mark == 0){
            if(strncmp(token2, "-", 1) != 0 && strncmp(token2, "ls", 2) != 0){
                
                if (strncmp(token2, "~", 1) == 0)
                    sprintf(abs_path, "%s%s", root, &token2[1]);
                
                else sprintf(abs_path, "%s", token2);
                
                sprintf(direct, "%s ", abs_path);
                mark=1;
            }
        }

        else{
            if(strncmp(token2, "-", 1) != 0){
                
                if (strncmp(token2, "~", 1) == 0)
                    sprintf(abs_path, "%s%s", root, &token2[1]);
                
                else sprintf(abs_path, "%s", token2);
                
                sprintf(direct, "%s%s ", direct, abs_path);
            }
        }
    }

    if (strcmp(direct, "") == 0) sprintf(fmt, "%s", flags);
    else sprintf(fmt, "%s %s", flags, direct);
}