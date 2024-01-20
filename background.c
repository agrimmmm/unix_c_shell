#include "headers.h"

pid_t background(char* input)
{
    char command[1024];
    int i=0;
    while(input[i] != '&')
    {
        command[i] = input[i];
        i++;
    }
    command[i] = '\0';
    while(command[strlen(command)]-1 == ' ')
    {
        command[strlen(command)-1] = '\0';
    }

    pid_t pid = fork();
    char* args[10];
    if(pid == -1)
        perror("ERROR ");
    else if(pid == 0)
    {
        char* token = strtok(command, " ");
        int i=1;
        args[0] = token;
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

        int e_check = execvp(args[0], final_args);
        if(e_check == -1)
        {
            perror("ERROR "); 
            printf("\n");
        }
    }
    else
    {
        printf("%d\n", pid);
        return pid;
    }
}