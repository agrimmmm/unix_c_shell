#include "headers.h"

void activities()
{
    struct dirent* d;
    DIR* dptr = opendir("/proc");
    char path[1024];
    pid_t shell = getpid();
    while((d = readdir(dptr)) != NULL)
    {
        if(atoi(d->d_name) > 0)
        {
            int pid = atoi(d->d_name);
            sprintf(path, "/proc/%d/stat", pid);
            FILE* stat = fopen(path, "r");
            if(stat != NULL)
            {
                char* read_stat[25];
                char line[1024];
                char* token;
                token = strtok(fgets(line, sizeof(line), stat), " ");
                read_stat[0] = token;
                token = strtok(NULL, " ");
                int i=1;
                while(i<24)
                {
                    read_stat[i++] = token;
                    token = strtok(NULL, " ");
                }

                char* cmnd = read_stat[1];
                cmnd[0] = ' ';
                cmnd[strlen(cmnd)-1] = ' ';
                char* state = read_stat[2];
                int ppid = atoi(read_stat[3]);
                
                fclose(stat);
                char full_state[128];
                if(state[0] == 'R')
                    strcpy(full_state, "Running");
                else if(state[0] == 'S')
                    strcpy(full_state, "Sleeping");
                else if(state[0] == 'D')
                    strcpy(full_state, "Uninterruptible Sleep");
                else if(state[0] == 'Z')
                    strcpy(full_state, "Zombie");
                else if(state[0] == 'T' || state[0] == 't')
                    strcpy(full_state, "Stopped");
                else if(state[0] == 'X' || state[0] == 'x')
                    strcpy(full_state, "Dead");
                else if(state[0] == 'W')
                    strcpy(full_state, "Waking");
                else if(state[0] == 'K')
                    strcpy(full_state, "Wakekill");
                else if(state[0] == 'I')
                    strcpy(full_state, "Idle");

                if(ppid == shell)
                    printf("%d :%s- %s\n", pid, cmnd, full_state);
            }
        }
    }
    closedir(dptr);
}