#include "headers.h"

char current[2048];
pid_t fg_pid = 0;
int d_flag = 0;

typedef struct children
{
    int no_of_children;
    int pids[128];
    char* names[128];
}children;

children processes;

void hist_fns(char* str, int* flag, int* count)
{
    if(strcmp(str, "pastevents") == 0 || strcmp(str, "pastevents ") == 0)
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
            if(strcmp(token, "purge") == 0)
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
    removeExtraSpaces(input);

    char* cd = "warp";
    char* list = "peek";
    char* match = "seek";
    char* printhis = "pastevents";
    char* info = "proclore";
    char* allinfo = "activities";
    char* signals = "ping";
    char* inf_proc = "neonate";
    char* manpages = "iMan";
    char* f = "fg";
    char* b = "bg";

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
        set_seek(input, UserName, prevwd);
    else if(strncmp(input, printhis, strlen(printhis)) == 0)
        hist_fns(input, flag, count);
    else if(strncmp(input, info, strlen(info)) == 0)
        retrieve(input);
    else if(strncmp(input, allinfo, strlen(allinfo)) == 0)
        activities();
    else if(strncmp(input, signals, strlen(signals)) == 0)
        ping(input);
    else if(strncmp(input, inf_proc, strlen(inf_proc)) == 0)
        neonate(input);
    else if(strncmp(input, f, strlen(f)) == 0)
    {
        char* token = strtok(input, " ");
        token = strtok(NULL, " ");
        if(token == NULL)
            printf("ERROR: Enter man-page to be found\n");
        else
            fg(atoi(token));
    }
    else if(strncmp(input, b, strlen(b)) == 0)
    {
        char* token = strtok(input, " ");
        token = strtok(NULL, " ");
        if(token == NULL)
            printf("ERROR: Enter man-page to be found\n");
        else
            bg(atoi(token));
    }
    else if(strncmp(input, manpages, strlen(manpages)) == 0)
    {
        char* token = strtok(input, " ");
        token = strtok(NULL, " ");
        if(token == NULL)
            printf("ERROR: Enter man-page to be found\n");
        else
            iman(token);
    }
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
            {
                printf("ERROR : %s is not a valid command\n", args[0]);
                exit(1);
            }
        }
        else
        {
            fg_pid = sys;
            int status;
            waitpid(sys, &status, 0);
        }
    }
}

void and_delimiter(char* input, char* UserName, char* prevwd, int* flag, int* count)
{
    removeExtraSpaces(input);

    int flg = 0;
    if(input[strlen(input)-1] == '&')
        flg = 1;
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
        if(j != i-1 || flg == 1)
            command[strlen(command)] = '&';
        process(command, UserName, prevwd, flag, count);
    }
}

void semi_delimiter(char* input, char* UserName, char* prevwd, int* flag, int* count)
{
    removeExtraSpaces(input);
    
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

void ctrl_c(int sin)
{
    if(fg_pid != 0)
    {
        kill(fg_pid, SIGINT);
        printf("\n\033[1;32mSTOPPED!!\033[0m\n");
    }
}

void ctrl_z(int sin)
{
    if(fg_pid != 0)
    {
        kill(fg_pid, SIGTSTP);
        printf("\n\033[1;32mPushed to Background\033[0m\n");
    }
}

void ctrl_d()
{
    kill(0, SIGTERM);
    exit(0);
}

int main()
{
    fg_pid = 0;
    getcwd(current, sizeof(current));
    processes.no_of_children = 0;
    for(int i=0;i<128;i++)
    {
        processes.names[i] = (char*)calloc(sizeof(char), 128);
    }
    char* UserName = getlogin();
    char* prevwd = (char*)calloc(sizeof(char), 4096);
    char* prevComm = (char*)calloc(sizeof(char), 4096);
    int flag = 0, count = 0;

    char* dirforstore = (char*)calloc(sizeof(char), 2048);
    sprintf(dirforstore, "%s/command_store.txt", current);
    FILE* fptr = fopen(dirforstore, "r");
    char c = fgetc(fptr);
    while(c != EOF)
    {
        if(c == '\n')
            count++;
        c = fgetc(fptr);
    }
    fclose(fptr);

    struct sigaction act;
    act.sa_handler = &ctrl_c;
    sigfillset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    sigaction(SIGINT, &act, NULL);

    struct sigaction azt;
    azt.sa_handler = &ctrl_z;
    sigfillset(&azt.sa_mask);
    sigaction(SIGTSTP, &azt, NULL);

    while (1)
    {
        if(d_flag == 1)
            exit(0);
        prompt(UserName);
        char* input = (char*)calloc(sizeof(char), 4096);
        strcpy(input, TakeInput());
        // printf("%s\n", input);
        if(input == NULL)
            ctrl_d();
        if(d_flag == 1)
            exit(0);
        char tokenizer[4096];
        strcpy(tokenizer, input);
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
                FILE* fptr = fopen(dirforstore, "a");
                fprintf(fptr, "%s\n", input);
                if(count < 16)
                    count++;
                strcpy(prevComm, input);
                if (count == 16)
                    flag = 1;
                fclose(fptr);
                if(flag == 1)
                {
                    char* tempdir = (char*)calloc(sizeof(char), 2048);
                    sprintf(tempdir, "%s/temp.txt", current);
                    FILE* org = fopen(dirforstore, "r");
                    FILE* copy = fopen(tempdir, "w");
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
                    rmdir(dirforstore);
                    rename(tempdir, dirforstore);
                }
            }
        }

        int in_flag = 0, out_flag = 0;
        int in, out, readinp;
        char* strings[8];
        int cnt = 0;
        char* token = strtok(input, "|");
        while(token)
        {
            strings[cnt] = token;
            cnt++;
            token = strtok(NULL, "|");
        }

        if(cnt > 1)
        {
            in = dup(STDIN_FILENO);
            out = dup(STDOUT_FILENO);
            readinp = dup(STDIN_FILENO);

            int fild[2];
            for(int i=0; i<cnt; i++)
            {
                pipe(fild);
                if(i != 0)
                    dup2(readinp, STDIN_FILENO);
                if(i == cnt-1)
                    dup2(out, STDOUT_FILENO);
                else
                {
                    dup2(fild[1], STDOUT_FILENO);
                    close(fild[1]);
                }
                if(isSubstring(strings[i], "<"))
                {
                    char* newinp;
                    char* subtok = strtok(strings[i], "<");
                    newinp = subtok;
                    subtok = strtok(NULL, "<");
                    if(!isSubstring(subtok, ">>") && !isSubstring(subtok, ">"))
                    {
                        if(subtok == NULL)
                        {
                            printf("Syntax Error!\n");
                            continue;
                        }
                        in_flag = 1;
                        removeExtraSpaces(subtok);
                        redirectInput(subtok);
                    }
                    else if(isSubstring(subtok, ">>"))
                    {
                        char* ansubtok = strtok(subtok, ">>");
                        if(ansubtok == NULL)
                        {
                            printf("Syntax Error!\n");
                            continue;
                        }
                        in_flag = 1;
                        removeExtraSpaces(ansubtok);
                        redirectInput(ansubtok);
                        ansubtok = strtok(NULL, ">>");
                        if(ansubtok == NULL)
                        {
                            printf("Syntax Error!\n");
                            continue;
                        }
                        out_flag = 2;
                        removeExtraSpaces(ansubtok);
                        redirectOutput(ansubtok, out_flag);
                    }
                    else if(isSubstring(subtok, ">"))
                    {
                        char* ansubtok = strtok(subtok, ">");
                        if(ansubtok == NULL)
                        {
                            printf("Syntax Error!\n");
                            continue;
                        }
                        in_flag = 1;
                        removeExtraSpaces(ansubtok);
                        redirectInput(ansubtok);
                        ansubtok = strtok(NULL, ">");
                        if(ansubtok == NULL)
                        {
                            printf("Syntax Error!\n");
                            continue;
                        }
                        out_flag = 1;
                        removeExtraSpaces(ansubtok);
                        redirectOutput(ansubtok, out_flag);
                    }
                    strcpy(strings[i], newinp);
                }
                else
                {
                    if(isSubstring(strings[i], ">>"))
                    {
                        char* newinp;
                        char* ansubtok = strtok(strings[i], ">>");
                        newinp = ansubtok;
                        ansubtok = strtok(NULL, ">>");
                        if(ansubtok == NULL)
                        {
                            printf("Syntax Error!\n");
                            continue;
                        }
                        out_flag = 2;
                        removeExtraSpaces(ansubtok);
                        redirectOutput(ansubtok, out_flag);
                        strcpy(strings[i], newinp);
                    }
                    else if(isSubstring(strings[i], ">"))
                    {
                        char* newinp;
                        char* ansubtok = strtok(strings[i], ">");
                        newinp = ansubtok;
                        ansubtok = strtok(NULL, ">");
                        if(ansubtok == NULL)
                        {
                            printf("Syntax Error!\n");
                            continue;
                        }
                        out_flag = 1;
                        removeExtraSpaces(ansubtok);
                        redirectOutput(ansubtok, out_flag);
                        strcpy(strings[i], newinp);
                    }
                }

                if(isSubstring(strings[i], ";"))
                    semi_delimiter(strings[i], UserName, prevwd, &flag, &count);
                else if(isSubstring(strings[i], "&"))
                    and_delimiter(strings[i], UserName, prevwd, &flag, &count);
                else
                    process(strings[i], UserName, prevwd, &flag, &count);
                    
                if(in_flag == 1)    
                    dup2(in, STDIN_FILENO);
                if(out_flag > 0)
                    dup2(out, STDOUT_FILENO);

                readinp = fild[0];
            }
            dup2(in, STDIN_FILENO);
            close(in);
            close(out);
            close(readinp);
        }
        else
        {
            in = dup(STDIN_FILENO);
            out = dup(STDOUT_FILENO);
            if(isSubstring(input, "<"))
            {
                char* newinp;
                char* subtok = strtok(tokenizer, "<");
                newinp = subtok;
                subtok = strtok(NULL, "<");
                if(!isSubstring(subtok, ">>") && !isSubstring(subtok, ">"))
                {
                    if(subtok == NULL)
                    {
                        printf("Syntax Error!\n");
                        continue;
                    }
                    in_flag = 1;
                    removeExtraSpaces(subtok);
                    redirectInput(subtok);
                }
                else if(isSubstring(subtok, ">>"))
                {
                    char* ansubtok = strtok(subtok, ">>");
                    if(ansubtok == NULL)
                    {
                        printf("Syntax Error!\n");
                        continue;
                    }
                    in_flag = 1;
                    removeExtraSpaces(ansubtok);
                    redirectInput(ansubtok);
                    ansubtok = strtok(NULL, ">>");
                    if(ansubtok == NULL)
                    {
                        printf("Syntax Error!\n");
                        continue;
                    }
                    out_flag = 2;
                    removeExtraSpaces(ansubtok);
                    redirectOutput(ansubtok, out_flag);
                }
                else if(isSubstring(subtok, ">"))
                {
                    char* ansubtok = strtok(subtok, ">");
                    if(ansubtok == NULL)
                    {
                        printf("Syntax Error!\n");
                        continue;
                    }
                    in_flag = 1;
                    removeExtraSpaces(ansubtok);
                    redirectInput(ansubtok);
                    ansubtok = strtok(NULL, ">");
                    if(ansubtok == NULL)
                    {
                        printf("Syntax Error!\n");
                        continue;
                    }
                    out_flag = 1;
                    removeExtraSpaces(ansubtok);
                    redirectOutput(ansubtok, out_flag);
                }
                memset(input, '\0', sizeof(input));
                strcpy(input, newinp);
            }
            else
            {
                if(isSubstring(input, ">>"))
                {
                    char* newinp;
                    char* ansubtok = strtok(tokenizer, ">>");
                    newinp = ansubtok;
                    ansubtok = strtok(NULL, ">>");
                    if(ansubtok == NULL)
                    {
                        printf("Syntax Error!\n");
                        continue;
                    }
                    out_flag = 2;
                    removeExtraSpaces(ansubtok);
                    redirectOutput(ansubtok, out_flag);
                    memset(input, '\0', sizeof(input));
                    strcpy(input, newinp);
                }
                else if(isSubstring(input, ">"))
                {
                    char* newinp;
                    char* ansubtok = strtok(tokenizer, ">");
                    newinp = ansubtok;
                    ansubtok = strtok(NULL, ">");
                    if(ansubtok == NULL)
                    {
                        printf("Syntax Error!\n");
                        continue;
                    }
                    out_flag = 1;
                    removeExtraSpaces(ansubtok);
                    redirectOutput(ansubtok, out_flag);
                    memset(input, '\0', sizeof(input));
                    strcpy(input, newinp);
                }
            }

            if(isSubstring(input, ";"))
                semi_delimiter(input, UserName, prevwd, &flag, &count);
            else if(isSubstring(input, "&"))
                and_delimiter(input, UserName, prevwd, &flag, &count);
            else
                process(input, UserName, prevwd, &flag, &count);
            if(in_flag == 1)    
                dup2(in, STDIN_FILENO);
            if(out_flag > 0)
                dup2(out, STDOUT_FILENO);
        }
    }
}