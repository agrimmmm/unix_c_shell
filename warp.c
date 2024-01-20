#include "headers.h"

char* warp(char* str, char* UserName, char* prevwd)
{
    if(strcmp(str, "warp\n") == 0)
    {
        chdir("/home");
        printf("/home\n");
        exit;
    }

    if(strcmp(str, "warp \n") == 0)
    {
        chdir("/home");
        exit;
    }

    char home[256];
    sprintf(home, "/home/%s", UserName);

    char* token;
    token = strtok(str, " ");

    if(strcmp(token, "warp") == 0)
    {
        token = strtok(NULL, " ");
        while(token)
        {
            int len = strlen(token);
            if(token[len-1] == '\n')
                token[len-1] = '\0';

            char currwd[4096];
            getcwd(currwd, sizeof(currwd));
            if(*token == '~')
                chdir(home);
            else if(*token == '-')
            {   if(strcmp(prevwd, "") == 0)
                    printf("OLDPWD not set\n");
                else
                    chdir(prevwd);
            }
            else
                chdir(token);
            strcpy(prevwd, currwd);
            
            token = strtok(NULL, " ");
        }
        char pwd[1024];
        getcwd(pwd, sizeof(pwd));
        printf("%s\n", pwd);
    }
    return prevwd;
}