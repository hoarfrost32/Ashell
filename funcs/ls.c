#include "../headers.h"

struct passwd* user;
struct group* grp;

void perms(struct stat sc){
    printf(  S_ISDIR (sc.st_mode)  ? "d" : ".");
    printf( (sc.st_mode & S_IRUSR) ? "r" : "-");
    printf( (sc.st_mode & S_IWUSR) ? "w" : "-");
    printf( (sc.st_mode & S_IXUSR) ? "x" : "-");
    printf( (sc.st_mode & S_IRGRP) ? "r" : "-");
    printf( (sc.st_mode & S_IWGRP) ? "w" : "-");
    printf( (sc.st_mode & S_IXGRP) ? "x" : "-");
    printf( (sc.st_mode & S_IROTH) ? "r" : "-");
    printf( (sc.st_mode & S_IWOTH) ? "w" : "-");
    printf( (sc.st_mode & S_IXOTH) ? "x" : "-");
}

void verbose(char* fstr, struct dirent* namelist){
    struct stat sc;
    stat(fstr, &sc);

    perms(sc);
    printf(" %d", sc.st_nlink);
    user = getpwuid(sc.st_uid);
    printf(" %s", user->pw_name);
    grp = getgrgid(sc.st_gid);
    printf(" %s", grp->gr_name);
    printf(" %d", sc.st_size);

    char* date_time = ctime(&sc.st_mtime);
    char* token_n = strtok_r(date_time, "\n", &date_time); 
    printf(" %s ", token_n); 
    printf("%s", namelist->d_name);     
    printf("\n");
}

void file(char* token){
    FILE* f = fopen(token, "r"); 

    if(f == NULL) {
        printf("%s: No such file or directory\n", token);
        return;
    }

    else{
        char* bname = basename(token);
        printf(YELLOW);
        printf("%s\n", bname);
        printf(DEFCOL); 
    }

    fclose(f);
}

void do_ls(char* format, int flag, int mode){

    DIR *dptr=opendir(format);

    struct stat sc;

    struct dirent **namelist;
    int dir_size;

    if (dptr==NULL){ 
        file(format);
        return;
    }

    closedir(dptr);

    printf(RED);
    printf("%s:\n", format);
    printf(DEFCOL);

    dir_size = scandir(format, &namelist, NULL, alphasort);

    for(int i=0; i<dir_size; i++){

        char* fstr = (char*)malloc((strlen(format) + 256)*sizeof(char));
        // char* to_free_fstr_blk = fstr;

        sprintf(fstr, "%s/%s", format, namelist[i]->d_name);
  
        if(stat(fstr, &sc)==0){
            if(S_ISDIR(sc.st_mode))
                printf(RED);
            else{
                if(stat(fstr, &sc)== 0 && sc.st_mode&S_IXUSR) printf(CYAN);
                else
                    printf(YELLOW);
            }
        }

        if(flag==0){
            if(namelist[i]->d_name[0]!='.'){ 
                if(mode==0) printf("%s\n", namelist[i]->d_name);
                else
                    verbose(fstr, namelist[i]);
                free(namelist[i]);
            }
        }

        else{
            if(mode==0) printf("%s\n", namelist[i]->d_name);
            else
                verbose(fstr, namelist[i]);
            free(namelist[i]);
        }

        free(fstr);
    }

    free(namelist);
}

void check_flgs(char* input){

    // Set a_flg and l_flg to mark if they're used or not, then call do_ls.

    char* token;
    int a_flg=0, l_flg=0;

    token = strtok_r(input, " \n\r\t\f\v", &input);
    for(int i=1; i<strlen(token); i++){
        if(token[i]=='a')
            a_flg=1;
        else if (token[i]=='l')
            l_flg=1;
        else{
            printf("ERROR: Invalid Option \"-%c\" For Command \"ls\"\n", token[i]);
            return;
        }
    }

    // It is possible that while flags are used no path to a file/directory
    // is provided.
    token = strtok_r(NULL, " \n\r\t\f\v", &input);
    if (token == NULL){ 
        char cwd[__PATH_MAX__];
        getcwd(cwd, __PATH_MAX__);
        do_ls(cwd, a_flg, l_flg);
        return;
    }
    
    do_ls(token, a_flg, l_flg);
    while(token = strtok_r(NULL, " \n\r\t\f\v", &input))
        token = strtok_r(NULL, " \n\r\t\f\v", &input);    

    return;
}

void ls(char* input, char* root){ 

    /* 
     The structure of the ls command as seen by the user is somewhat erratic;
     the flags can be positioned either side of the file/directory argument,
     There is a need to handle multiple repeat uses of a flag, the directory
     argument can be provided as a relative path, etc. To deal with this, the 
     `format` function is provided the input command, and it returns a formatted
     input adhering to the following guidelines:
     
     - The fmt_input contains the formatted arguments in the following format;
            -<flags> <file_1/directory_1> <file_2/directory_2> ...
     - Repeat instances of the flags are dealt with and the formatted input contains
       only one instance of each flag.
     - The path to the file or directory is now an absolute path.
    */

    char* fmt_input;
    fmt_input = (char *)malloc((__PATH_MAX__ + 4)*sizeof(char));
    char* to_free_fmt_blk = fmt_input;

    format(input, fmt_input, root);

    // printf("%s\n", fmt_input);

    // If the fmt_input is empty, that means the user intended to execute ls with no
    // flags on the current working directory.

    if(strcmp(fmt_input, " ")==0){
        char cwd[__PATH_MAX__];
        
        getcwd(cwd, __PATH_MAX__);
        do_ls(cwd, 0, 0);

        free (to_free_fmt_blk);
        return;
    }

    // If the first character is a hyphen, flags were used. Here we call the check_flgs 
    // function, which sets the flags for us and calls the do_ls function.

    if(fmt_input[0]=='-'){ 
        check_flgs(fmt_input);

        free(to_free_fmt_blk);
        return;
    }

    // Falling to this part of the code means that;
    //  - no flags were used.
    //  - A file/directory path was provided.
    //
    // tokenize the string to get rid of any trailing whitespaces and call do_ls.

    char* token;

    while(token = strtok_r(fmt_input, " \n\r\v\f\t", &fmt_input)){
        do_ls(token, 0, 0);
        printf("\n");
    }

    free(to_free_fmt_blk);

    return;
}