#include "headers.h"

int stop = 0;
int del_time;

void alarmHandle(int signo)
{
    if(!stop)
    {
        printf("%d\n", getpid());
        alarm(del_time);
    }
}

void neonate(char* input)
{
    stop = 0;
    char* token = strtok(input, " ");
    token = strtok(NULL, " ");
    if(token == NULL || strcmp(token, "-n") != 0)
    {
        printf("Syntax Error!\n");
        return;
    }
    token = strtok(NULL, " ");
    if(token == NULL)
    {
        printf("Syntax Error!\n");
        return;
    }
    del_time = atoi(token);
    if(del_time < 1)
    {
        printf("Time should be positive!\n");
        return;
    }
    enableRawMode();
    signal(SIGALRM, alarmHandle);
    printf("%d\n", getpid());
    alarm(del_time);

    while(!stop)
    {
        char c;
        if(read(STDIN_FILENO, &c, 1) == 1 && c == 'x')
            stop = 1;
    }
    disableRawMode();
}