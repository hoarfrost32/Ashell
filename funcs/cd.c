#include "../headers.h"

void pathFinder(char* source, char* root){

    // Start by writing into the destination path string 
    // the absolute path to the home directory.
    char dest[__PATH_MAX__];
    strcpy(dest, root);
    
    char* substring;

    // strchr() is a function that returns a pointer to
    // the first instance of a character in a string.
    substring = strchr(source, '/') + 1;
    strcpy(dest, substring);

    // Through strchr() we obtain a char* ptr to the substring
    // in the source path that describes the dest after the ~.
    // Appending this to the dest path gives us our need absolute
    // path.
    chdir(dest);
    free(dest);
}

char* cd(char* input, char* root_dir, char* cwd, char* prev){

    char new_cwd[__PATH_MAX__];
    char* token;
    int flag = 0;

    while (token = strtok_r(NULL, " \t\n\r\v\f", &input)){
        
        // The flag adjusts the output depending on the number 
        // of arguments provided:
        // - 1 arg is handled by the if condition below.
        // - 2 args mean an error is to be thrown.
        // - 0 args mean that we need to change to the home directory.  

        flag++; 

        if(flag==1 && token != NULL){

            // The cd command implemented handles the flags '.', '..', '-', '~'.

            //<--------------------------------------------------------->
            
            // Handles the flags '..' and '.'
            if((strcmp(token, "..")==0) || (strcmp(token, ".")==0))
                chdir(token);
            
            //<--------------------------------------------------------->
            
            // Handles the '~' flag by just changing to root_dir.
            else if (strcmp(token, "~")==0) chdir(root_dir);

            //<--------------------------------------------------------->
            
            // Changes to the previous working directory.
            else if (strcmp(token, "-")==0){
                printf("%s\n", prev);
                chdir(prev);
            }
            
            //<--------------------------------------------------------->

            // Changes to the specified directory. Does work with relative paths.

            else {
                // Checks if the path is specified relative to the home directory
                // via the `~` flag. If its not then chdir can directly be used.
                if (token[0]!=126) chdir(token);

                // If however the path given is relative...
                else if (token[0]==126){

                    //...Then copy the root_dir into the path being constructed to start off... 
                    
                    char source[__PATH_MAX__];
                    strcpy(source, &token[1]);
                    
                    // ...and call this function to investigate the absolute path
                    // to the target directory. And change to said directory.
                    pathFinder(source, root_dir);
                }
            }
            
            continue;
        }

        else if (flag>=2){
            if( token != NULL){
                printf("ERROR: Invalid number of command line arguments\n");
                chdir(cwd);
            }
            return cwd;
        }
        
        flag=0;
    }

    if(flag==0) chdir(root_dir);
    
    getcwd(new_cwd, __PATH_MAX__);
    return new_cwd;
}