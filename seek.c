#include "headers.h"

void set_seek(char* input, char* UserName, char* prevwd)
{
    char tilda[1024];
    sprintf(tilda, "/home/%s", UserName);
    char* home = ".";
    int e = 0;
    char* token = strtok(input, " ");
    int number = 0;
    if(strcmp(token, "seek") == 0)
    {
        token = strtok(NULL, " ");
        if(strcmp(token, "-d") == 0)
        {
            token = strtok(NULL, " ");
            if(strcmp(token, "-f") == 0)
            {
                printf("Invalid Flags!\n");
                number = -1;
            }
            else if(strcmp(token, "-e") == 0)
            {
                e = 1;
                char* name = strtok(NULL, " ");
                if(strcmp(name, "-f") == 0)
                {
                    printf("Invalid Flags!\n");
                    number = -1;
                }
                else
                {
                    char* dir = strtok(NULL, " ");
                    if(dir)
                    {
                        dir[strlen(dir)-1] = '\0';
                        if(strcmp(dir, "~") == 0)
                        {
                            strcpy(dir, tilda);
                        }
                        number = seek(name, dir, 'd', e, dir, prevwd);
                    }
                    else
                    {
                        name[strlen(name)-1] = '\0';
                        number = seek(name, home, 'd', e, home, prevwd);
                    }
                }
            }
            else
            {
                char* dir = strtok(NULL, " ");
                if(dir)
                {
                    dir[strlen(dir)-1] = '\0';
                    if(strcmp(dir, "~") == 0)
                    {
                        strcpy(dir, tilda);
                    }
                    number = seek(token, dir, 'd', e, dir, prevwd);
                }
                else
                {
                    token[strlen(token)-1] = '\0';
                    number = seek(token, home, 'd', e, home, prevwd);
                }
            }
        }
        else if(strcmp(token, "-f") == 0)
        {
            token = strtok(NULL, " ");
            if(strcmp(token, "-d") == 0)
            {
                printf("Invalid Flags!\n");
                number = -1;
            }
            else if(strcmp(token, "-e") == 0)
            {
                e = 1;
                char* name = strtok(NULL, " ");
                if(strcmp(name, "-d") == 0)
                {
                    printf("Invalid Flags!\n");
                    number = -1;
                }
                else
                {
                    char* dir = strtok(NULL, " ");
                    if(dir)
                    {
                        dir[strlen(dir)-1] = '\0';
                        if(strcmp(dir, "~") == 0)
                        {
                            strcpy(dir, tilda);
                        }
                        number = seek(name, dir, 'd', e, dir, prevwd);
                    }
                    else
                    {
                        name[strlen(name)-1] = '\0';
                        number = seek(name, home, 'f', e, home, prevwd);
                    }
                }
            }
            else
            {
                char* dir = strtok(NULL, " ");
                if(dir)
                {
                        dir[strlen(dir)-1] = '\0';
                        if(strcmp(dir, "~") == 0)
                        {
                            strcpy(dir, tilda);
                        }
                        number = seek(token, dir, 'd', e, dir, prevwd);
                    }
                else
                {
                    token[strlen(token)-1] = '\0';
                    number = seek(token, home, 'f', e, home, prevwd);
                }
            }
        }
        else if(strcmp(token, "-e") == 0)
        {
            e = 1;
            token = strtok(NULL, " ");
            if(strcmp(token, "-f") == 0)
            {
                char* name = strtok(NULL, " ");
                if(strcmp(name, "-d") == 0)
                {
                    printf("Invalid Flags!\n");
                    number = -1;
                }
                else
                {
                    char* dir = strtok(NULL, " ");
                    if(dir)
                    {
                        dir[strlen(dir)-1] = '\0';
                        if(strcmp(dir, "~") == 0)
                        {
                            strcpy(dir, tilda);
                        }
                        number = seek(name, dir, 'd', e, dir, prevwd);
                    }
                    else
                    {
                        name[strlen(name)-1] = '\0';
                        number = seek(name, home, 'f', e, home, prevwd);
                    }
                }
            }
            else if(strcmp(token, "-d") == 0)
            {
                char* name = strtok(NULL, " ");
                if(strcmp(name, "-f") == 0)
                {
                    printf("Invalid Flags!\n");
                    number = -1;
                }
                else
                {
                    char* dir = strtok(NULL, " ");
                    if(dir)
                    {
                        dir[strlen(dir)-1] = '\0';
                        if(strcmp(dir, "~") == 0)
                        {
                            strcpy(dir, tilda);
                        }
                        number = seek(name, dir, 'd', e, dir, prevwd);
                    }
                    else
                    {
                        name[strlen(name)-1] = '\0';
                        number = seek(name, home, 'd', e, home, prevwd);
                    }
                }
            }
            else
            {
                char* dir = strtok(NULL, " ");
                if(dir)
                {
                    dir[strlen(dir)-1] = '\0';
                    if(strcmp(dir, "~") == 0)
                    {
                        strcpy(dir, tilda);
                    }
                    number = seek(token, dir, 'x', e, dir, prevwd);
                }
                else
                {
                    token[strlen(token)-1] = '\0';
                    number = seek(token, home, 'x', e, home, prevwd);
                }
            }
        }
        else
        {
            char* name = token;
            char* dir = strtok(NULL, " ");
            if(dir)
            {
                dir[strlen(dir)-1] = '\0';
                if(strcmp(dir, "~") == 0)
                {
                    strcpy(dir, tilda);
                }
                number = seek(name, dir, 'x', e, dir, prevwd);
            }
            else
            {
                name[strlen(name)-1] = '\0';
                number = seek(name, home, 'x', e, home, prevwd);
            }
        }
    }
    else
        exit;
    if(number == 0)
        printf("No match found!\n");
}

int seek(char* word, char* dir, char df, int e, char* current, char* prevwd)
{
    char e_name[1024];
    char e_type;
    char* UserName = getlogin();
    struct dirent* d;
    DIR *dptr = opendir(dir);

    int i=0;
    struct stat buff;
    char path[1024];
    while((d = readdir(dptr)) != NULL)
    {
        if(d->d_name[0] == '.')
            continue;

        sprintf(path, "%s/%s", dir, d->d_name);
        stat(path, &buff);

        if(S_ISDIR(buff.st_mode))
            i += seek(word, path, df, e, current, prevwd);

        int len = strlen(word);
        int check = 0;
        for(int j=0; j<len; j++)
        {
            if(word[j] != d->d_name[j])
            {
                check = 1;
                break;
            }
        }
        if(check == 1)
            continue;

        char* relative = (char*)calloc(sizeof(char), 1024);
        relative[0] = '.';
        int x=1;
        for(int k=strlen(current); k<strlen(path); k++)
        {
            relative[x++] = path[k];
        }
        relative[x] = '\0';

        if(S_ISDIR(buff.st_mode) && df != 'f')
        {
            i++;
            printf("\033[1;34m%s\033[0m\n", relative);
            strcpy(e_name, path);
            e_type = 'd';
        }
        else if(df != 'd')
        {
            if(S_ISDIR(buff.st_mode))
                continue;
            i++;
            printf("\033[1;32m%s\033[0m\n", relative);
            strcpy(e_name, path);
            e_type = 'f';
        }
    }
    if(i == 1 && e == 1)
    {
        if(e_type == 'd')
        {
            // char path[4096];
            // sprintf(path, "warp %s", e_name);
            // printf("%s\n", path);
            //warp(path, UserName, dir);
            char buff[4096];
            getcwd(buff, sizeof(buff));
            strcpy(prevwd, buff);
            chdir(e_name);
        }
        else if(e_type == 'f')
        {
            if(!(buff.st_mode & S_IRUSR))
                printf("Missing permissions for task\n");
            else
            {
                FILE* fptr = fopen(e_name, "r");
                char* line = (char*)calloc(sizeof(char), 1024);
                while(fgets(line, sizeof(line), fptr) != NULL)
                {
                    printf("%s", line);
                }
                printf("\n");
                fclose(fptr);
            }
        }
    }

    return i;
}