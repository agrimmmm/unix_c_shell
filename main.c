#include "headers.h"

char current[2048];

typedef struct children
{
    int no_of_children;
    int pids[128];
    char* names[128];
}children;

children processes;

bool isSubstring(char *input, char *substring)
{
    int inputLength = strlen(input);
    int substringLength = strlen(substring);

    if(inputLength < substringLength)
        return false;

    for (int i = 0; i <= inputLength - substringLength; i++)
    {
        int j;
        for (j = 0; j < substringLength; j++)
        {
            if (input[i + j] != substring[j])
                break;
        }
        if (j == substringLength)
            return true;
    }
    return false;
}

void ls(char* str, char* UserName)
{
    if(str[0] == 'p' && str[1] == 'e' && str[2] == 'e' && str[3] == 'k')
    {
        char Directory[1024];
        getcwd(Directory, sizeof(Directory));
        if(str[4] == '\n' || (str[4] == ' ' && str[5] == '\n'))
        {
            peek(Directory, 0, 0);
            return;
        }
        char* token;
        token = strtok(str, " ");
        int op_a = 0, op_l = 0;
        while(token)
        {
            token = strtok(NULL, " ");
            if(token == NULL)
                return;
            if(strcmp(token, "-a\n") == 0)
                peek(Directory, 1, 0);
            else if(strcmp(token, "-l\n") == 0)
                peek(Directory, 0, 1);
            else if(strcmp(token, "-al\n") == 0 || strcmp(token, "-la\n") == 0)
                peek(Directory, 1, 1);
            else if(strcmp(token, "-a") == 0)
                op_a = 1;
            else if(strcmp(token, "-l") == 0)
                op_l = 1;
            else if(strcmp(token, "-al") == 0 || strcmp(token, "-la") == 0)
            {
                op_a = 1;
                op_l = 1;
            }
            else if(!token)
            {
                peek(Directory, op_a, op_l);
                exit;
            }
            else
            {
                token[strlen(token)-1] = '\0';
                if(strcmp(token, "~") == 0)
                {
                    char home[256];
                    sprintf(home, "/home/%s", UserName);
                    peek(home, op_a, op_l);
                }
                else
                    peek(token, op_a, op_l);
            }
        }
    }
    else    
        exit;
}

void retrieve(char* input)
{
    if(strcmp(input, "proclore\n") == 0 || strcmp(input, "proclore ") == 0)
        proclore(0);
    else
    {
        char* token;
        token = strtok(input, " ");
        if(strcmp(token, "proclore") == 0)
        {
            token = strtok(NULL, " ");
            int pid = atoi(token);
            proclore(pid);
        }
        else
            exit;
    }
}

void hist_fns(char* str, int* flag, int* count)
{
    if(strcmp(str, "pastevents\n") == 0 || strcmp(str, "pastevents ") == 0)
    {
        char* path = (char*)calloc(sizeof(char), 2048);
        sprintf(path, "%s/%s", current, "command_store.txt");
        FILE* fptr = fopen(path, "r");
        pastevents(fptr);
        fclose(fptr);
    }
    else
    {
        char* token;
        token = strtok(str, " ");
        token = strtok(NULL, " ");
        if(token)
        {
            if(strcmp(token, "purge\n") == 0)
                past_purge(flag);
            else if(strcmp(token, "execute") == 0)
            {
                char* path = (char*)calloc(sizeof(char), 2048);
                sprintf(path, "%s/%s", current, "command_store.txt");
                FILE* fptr = fopen(path, "r");
                token = strtok(NULL, " ");
                past_execute(atoi(token), fptr, count);
                fclose(fptr);
            }
        }
    }
}

void process(char* input, char* UserName, char* prevwd, int* flag, int* count)
{
    char* cd = "warp";
    char* list = "peek";
    char* match = "seek";
    char* printhis = "pastevents";
    char* info = "proclore";

    if(isSubstring(input, "&"))
    {
        int val = background(input);
        if(val != 0)
        {
            char *temp = (char*)calloc(sizeof(char), 128);
            strcpy(temp, input);
            processes.pids[processes.no_of_children] = val;
            char* tok = strtok(temp, " ");
            processes.names[processes.no_of_children++] = tok;
        }
    }
    else if(strncmp(input, cd, strlen(cd)) == 0)
        warp(input, UserName, prevwd);
    else if(strncmp(input, list, strlen(list)) == 0)
        ls(input, UserName);
    else if(strncmp(input, match, strlen(match)) == 0)
        set_seek(input, UserName);
    else if(strncmp(input, printhis, strlen(printhis)) == 0)
        hist_fns(input, flag, count);
    else if(strncmp(input, info, strlen(info)) == 0)
        retrieve(input);
    else
    {
        if(input[strlen(input)-1] == '\n')
            input[strlen(input)-1] = '\0';
        char* args[10];
        char* token = strtok(input, " ");
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
        int e_check = execvp(args[0], final_args);
        if(e_check == -1)
            printf("ERROR : %s is not a valid command\n", args[0]);
        }
        else
        {
            int status;
            waitpid(sys, &status, 0);
        }
    }
}


void and_delimiter(char* input, char* UserName, char* prevwd, int* flag, int* count)
{
    char* token = strtok(input, "&");
    char* all_commands[128];
    int i=0;
    while(token)
    {
        all_commands[i++] = token;
        token = strtok(NULL, "&");
    }
    for(int j=0;j<i;j++)
    {
        int len = strlen(all_commands[j]);
        char* command = (char*)calloc(sizeof(char), len+3);
        strcpy(command, all_commands[j]);
        if(j != i-1)
            command[strlen(command)] = '&';
        process(command, UserName, prevwd, flag, count);
    }
}

void semi_delimiter(char* input, char* UserName, char* prevwd, int* flag, int* count)
{
    char* token = strtok(input, ";");
    char* all_commands[128];
    int i=0;
    while(token)
    {
        all_commands[i++] = token;
        token = strtok(NULL, ";");
    }
    
    for(int j=0;j<i;j++)
    {
        if(isSubstring(all_commands[j], "&"))
            and_delimiter(all_commands[j], UserName, prevwd, flag, count);
        else
            process(all_commands[j], UserName, prevwd, flag, count);
    }
}

int main()
{
    getcwd(current, sizeof(current));
    processes.no_of_children = 0;
    for(int i=0;i<128;i++)
    {
        processes.names[i] = (char*)calloc(sizeof(char), 128);
    }
    char* UserName = getlogin();
    // Keep accepting commands
    char* prevwd = (char*)calloc(sizeof(char), 4096);
    char* prevComm = (char*)calloc(sizeof(char), 4096);
    int flag = 0, count = 0;

    FILE* fptr = fopen("command_store.txt", "r");
    char c = fgetc(fptr);
    while(c != EOF)
    {
        if(c == '\n')
            count++;
        c = fgetc(fptr);
    }
    fclose(fptr);

    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(UserName);
        char input[4096];
        fgets(input, 4096, stdin);
        for(int k=0; k<processes.no_of_children; k++)
        {
            int status;
            int check = waitpid(processes.pids[k], &status, WNOHANG);
            if(check > 0)
            {
                if(WIFEXITED(status))
                    printf("%s exited normally (%d)\n", processes.names[k], processes.pids[k]);
                else
                    printf("%s exited abnormally (%d)\n", processes.names[k], processes.pids[k]);
            }
        }

        if(isSubstring(input, "pastevents") == false)
        {
            if(input != " " && strcmp(input, prevComm) != 0)
            {
                FILE* fptr = fopen("command_store.txt", "a");
                fprintf(fptr, "%s", input);
                if(count < 16)
                    count++;
                strcpy(prevComm, input);
                if (count == 16)
                    flag = 1;
                fclose(fptr);
        
                if(flag == 1)
                {
                    FILE* org = fopen("command_store.txt", "r");
                    FILE* copy = fopen("temp.txt", "w");
                    char garbage = fgetc(org);
                    while(garbage != '\n')
                    {
                        garbage = fgetc(org);
                    }
                    char line[128];
                    while(fgets(line, sizeof(line), org) != NULL)
                    {
                        fprintf(copy, "%s", line);
                    }
                    fclose(org);
                    fclose(copy);
                    rmdir("command_store.txt");
                    rename("temp.txt", "command_store.txt");
                }
            }
        }

        if(isSubstring(input, ";"))
            semi_delimiter(input, UserName, prevwd, &flag, &count);
        else if(isSubstring(input, "&"))
            and_delimiter(input, UserName, prevwd, &flag, &count);
        else
            process(input, UserName, prevwd, &flag, &count);
    }
}