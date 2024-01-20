#include "headers.h"

void proclore(int pid)
{
    if(pid == 0)
        pid = getpid();
    int pgid = getpgid(pid);

    char stat_proc[128];
    char status_proc[128];
    char exe_proc[128];
    sprintf(stat_proc, "/proc/%d/stat", pid);
    sprintf(status_proc, "/proc/%d/status", pid);
    sprintf(exe_proc, "/proc/%d/exe", pid);

    FILE* stat_open = fopen(stat_proc, "r");
    char* read_stat[25];
    char line[1024];
    char* token;
    token = strtok(fgets(line, sizeof(line), stat_open), " ");
    read_stat[0] = token;
    token = strtok(NULL, " ");
    int i=1;
    while(i<24)
    {
        read_stat[i++] = token;
        token = strtok(NULL, " ");
    }

    char* status = read_stat[2];
    char* memory = read_stat[22];
    char* executable = (char*)calloc(sizeof(char), 256);

    if(read_stat[4] == read_stat[7])
    {
        status[1] = '+';
        status[2] = 0;
    }
    else
        status[1] = 0;
    fclose(stat_open);
    
    char buff[1024] = "";
    readlink(exe_proc,buff,1024);
    char* home = "/home/agrim";
    if(strlen(home) > strlen(buff))
        strcpy(executable, buff);
    else
    {
        if(strncmp(buff, home, strlen(home)) == 0)
            strcat(executable, "~");
        strcat(executable, &buff[strlen(home)]);
    }

    printf("pid : %d\nProcess Status : %s\nProcess Group : %d\nVirtual Memory : %s\nExecutable Path : %s\n", pid, status, pgid, memory, executable);
}