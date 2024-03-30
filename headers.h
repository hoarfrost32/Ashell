#define _GNU_SOURCE

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <locale.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <langinfo.h>
#include <sys/types.h>

#include "term.h"
#include "main.h"
#include "colors.h"
#include "format.h"
#include "discover.h"
#include "funcs/inbuilt.h"

#define __PATH_MAX__ 4097
#define HOSTNAME_MAX 255