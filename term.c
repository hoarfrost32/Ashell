#include "headers.h"

char re[__PATH_MAX__], prev[__PATH_MAX__], root_dir[__PATH_MAX__];

void prompt(int exec_time) {
    
    if (exec_time < 1)
        printf("%s>%s", GREEN, DEFCOL);
    else
        printf("%s took %ds %s>%s", 
                RED, exec_time, GREEN, DEFCOL);

    return;
}

int main() {

    getcwd(root_dir, __PATH_MAX__);
    int time = 0;

    while`(1) {

        // Fetch the hostname.

        char sysname[HOSTNAME_MAX];
        gethostname(sysname, HOSTNAME_MAX);

        // Fetch the username.
        
        struct passwd *username = getpwuid(getuid());
        char* uname = username->pw_name;

        printf("%s<%s@%s:", GREEN, uname, sysname);

        // Now, get the cwd. The path MUST be relative to the home directory,
        // Which itself is represented by a `~`.

        char cwd[__PATH_MAX__]; 
        getcwd(cwd, __PATH_MAX__);

        // Check if the cwd is a subdirectory of the root_dir, 
        // and if it is, then replcae the root_dir substring 
        // with a ~. 

        if (strncmp(root_dir, cwd, strlen(root_dir))==0) 
            sprintf(re, "~%s", &cwd[strlen(root_dir)]);

        else sprintf(re, "%s", cwd);

        // With the prompt defined we move on to actually doing 
        // something in this terminal.

        printf("%s%s%s", BLUE, re, DEFCOL);
        prompt(time);

        time = parse(cwd, root_dir, prev);

        /* Functions that have been built into this shell are:
          <--------------------------------------------------->
           - cd
           - ls
           - pwd
           - echo
           - clear 
           - history
           - pinfo
           - discover
           Additionally, system commands (vim, mkdir, touch) 
           can also be executed in this shell.
          <--------------------------------------------------->*/
    }

    return 0;
}