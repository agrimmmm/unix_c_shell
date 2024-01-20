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
        printf("<%s@%s:%s> ", UserName, SystemName, Directory);
    else
        printf("<%s@%s:~> ", UserName, SystemName);
}