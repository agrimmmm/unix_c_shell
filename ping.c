#include "headers.h"

void ping(char* input)
{
    char* token = strtok(input, " ");
    if(strcmp(token, "ping") == 0)
    {
        token = strtok(NULL, " ");
        if(token == NULL)
        {
            printf("Syntax Error!\n");
            return;
        }
        int pid = atoi(token);
        token = strtok(NULL, " ");
        if(token == NULL)
        {
            printf("Syntax Error!\n");
            return;
        }
        if(token[strlen(token)-1] == '\n')
            token[strlen(token)-1] = '\0';
        int signal = atoi(token);
        signal %= 32;
        if(kill(pid, signal) == -1)
        {
            if(errno == ESRCH)
                printf("No such process found\n");
            else
                perror("kill");
        }
    }
}