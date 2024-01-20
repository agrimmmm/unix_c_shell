#include "headers.h"

void fg(int pid)
{
    if(kill(pid, SIGCONT) == -1)
    {
        printf("No such process found\n");
        return;
    }
    int status;
    waitpid(pid, &status, 0);
}

void bg(int pid)
{
    if(kill(pid, SIGCONT) == -1)
    {
        printf("No such process found\n");
        return;
    }
}