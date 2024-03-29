#include "headers.h"

struct passwd* user;
struct group* grp;

void perms(struct stat sc){
    printf( S_ISDIR(sc.st_mode) ? "d" : ".");
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

void _l_shit(char* fstr, struct dirent* namelist){
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

void fr_ls(char* format, int flag, int mode){
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
        char* fstr;
        fstr=(char*)malloc((strlen(format)+101)*sizeof(char));
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
                    _l_shit(fstr, namelist[i]);
                free(namelist[i]);
            }
        }
        else{
            if(mode==0) printf("%s\n", namelist[i]->d_name);
            else
                _l_shit(fstr, namelist[i]);
            free(namelist[i]);
        }
    }
    free(namelist);
}

void flags(char* format, char* cpy_for){
    char* token;
    int flag=0, flag1=0, flag2=0;

    while(token=strtok_r(cpy_for, " \n\r\t\f\v", &cpy_for)){
        if(flag==0){
            for(int i=1; i<strlen(token); i++){
                if(token[i]=='a')
                    flag1=1;
                else if (token[i]=='l')
                    flag2=1;
                else{
                    printf("ERROR: Invalid Option \"-%c\" For Command \"ls\"\n", token[i]);
                    return;
                }
            }
            flag=1;
        }

        else{
            fr_ls(token, flag1, flag2);
            flag=2;
        }
    }

    if(flag==1){
        char cwd[256];
        getcwd(cwd, 256);
        fr_ls(cwd, flag1, flag2);
        return;
    }
}

void ls(char* cpy_tok2, char* root_dir){ 

    char* format;
    format = (char *)malloc(516*sizeof(char));

    Format(cpy_tok2, format, root_dir);

    if(strcmp(format, " ")==0){
        char cwd[256];
        getcwd(cwd, 256);
        fr_ls(cwd, 0, 0);
    }

    char* cpy_for = (char *)malloc((strlen(format)+1)*sizeof(char));
    strcpy(cpy_for, format);

    if(format[0]=='-'){ 
        flags(format, cpy_for);
        return;
    }
    char* token;

    while(token = strtok_r(format, " \n\r\v\f\t", &format))
        fr_ls(token, 0, 0);
}