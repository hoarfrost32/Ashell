#ifndef __P_INFO
    #define __P_INFO
    void pinfo(char* cpy_tok2);
#endif

#ifndef __PWD
    #define __PWD
    void pwd();
#endif

#ifndef __CD
    #define __CD
    void cd(char* input, char* root_dir, char* cwd, char* prev);
#endif

#ifndef __ECHO
    #define __ECHO
    void echo(char* input);
#endif

#ifndef __HIST
    #define __HIST
    void hist(char* root_dir);
#endif

#ifndef __CLEAR
    #define __CLEAR
    void clear();
#endif

#ifndef __SYS_COM
    #define __SYS_COM
    void fork_exec(char* token3, char* copy_tok2, char* cwd, char* copy_root, char* prev);
#endif