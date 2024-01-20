#include "headers.h"

void pastevents(FILE* fptr)
{
    char line[128];
    while(fgets(line, sizeof(line), fptr) != NULL)
    {
        printf("%s", line);
    }
}

void past_purge(int* flag)
{
    FILE* fptr = fopen("command_store.txt", "w");
    fclose(fptr);
    *flag = 0;
}

void past_execute(int index, FILE* fptr, int* count)
{
    char line[128];
    int val = *count - index;
    while(fgets(line, sizeof(line), fptr) != NULL)
    {
        if(val--)
            continue;
        break;
    }

    if(line[strlen(line)-1] == '\n')
        line[strlen(line)-1] = '\0';
    char* args[10];
    char* token = strtok(line, " ");
    args[0] = token;
    int i=1;
    while(token)
    {
        token = strtok(NULL, " ");
        args[i++] = token;
    }
    char* final_args[i];
    for(int j=0;j<i;j++)
    {
        final_args[j] = args[j];
    }
    
    pid_t sys = fork();
    if(sys == 0)
    {
    int e_check = execvp(final_args[0], final_args);
    if(e_check == -1)
        printf("ERROR : %s is not a valid command\n", args[0]);
    }
    else
    {
        int status;
        waitpid(sys, &status, 0);
    }
}