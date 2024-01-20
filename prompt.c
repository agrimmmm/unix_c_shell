#include "headers.h"

void prompt(char* UserName)
{
    // Do not hardcode the prompt
    char SystemName[256];
    gethostname(SystemName, sizeof(SystemName));
    char Directory[256];
    getcwd(Directory, sizeof(Directory));

    char check[124];
    sprintf(check, "/home/%s", UserName);

    if(strcmp(Directory, check) != 0)
        printf("\033[1;36m<%s@%s\033[0m:\033[1;31m%s>\033[0m ", UserName, SystemName, Directory);
    else
        printf("\033[1;36m<%s@%s\033[0m:\033[1;31m~>\033[0m ", UserName, SystemName);
}