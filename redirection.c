#include "headers.h"

void redirectInput(char* in_file)
{
    int fd = open(in_file, O_RDONLY);
    if(fd == -1)
    {
        printf("Error in opening file!\n");
        return;
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void redirectOutput(char* out_file, int out_flag)
{
    int fd;
    if(out_flag == 1)
        fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if(out_flag == 2)
    {
        fd = open(out_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
    }
    if(fd == -1)
    {
        printf("Error in opening file!\n");
        return;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}