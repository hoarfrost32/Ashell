#ifndef __PWD
    #define __PWD
        void pwd();
#endif

#ifndef __PINFO
    #define __PINFO
        void pinfo(char* token);
#endif

#ifndef __ECHO
    #define __ECHO
        void echo(char* input);
#endif

#ifndef __HIST
    #define __HIST
        void hist(char* root_dir);
#endif

#ifndef __CD
    #define __CD
    char* cd(char* input, char* root_dir, char* cwd, char* prev);
#endif

#ifndef __FORK_EXEC
    #define __FORK_EXEC
        void fork_exec(char* token3, char* cpy_tok2, char* cwd, char* copy_root, char* prev);
#endif