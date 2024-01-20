#include "headers.h"

void ls(char* str, char* UserName)
{
    if(str[0] == 'p' && str[1] == 'e' && str[2] == 'e' && str[3] == 'k')
    {
        char Directory[1024];
        getcwd(Directory, sizeof(Directory));
        if(str[4] == '\0' || (str[4] == ' ' && str[5] == '\0'))
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
            if(!token)
            {
                peek(Directory, op_a, op_l);
                return;
            }
            else if(strcmp(token, "-a") == 0)
                op_a = 1;
            else if(strcmp(token, "-l") == 0)
                op_l = 1;
            else if(strcmp(token, "-al") == 0 || strcmp(token, "-la") == 0)
            {
                op_a = 1;
                op_l = 1;
            }
            else
            {
                if(strcmp(token, "~") == 0)
                {
                    char home[256];
                    sprintf(home, "/home/%s", UserName);
                    peek(home, op_a, op_l);
                    return;
                }
                else
                {
                    peek(token, op_a, op_l);
                    return;
                }
            }
        }
    }
    else    
        return;
}

int cmpfunc (const void * a, const void * b)
{
    store as = *(store*)a;
    store bs = *(store*)b;
    return strcmp(bs.name, as.name);
}

void peek(char* dir, int op_a, int op_l)
{
    int blocks;
    struct dirent* d;
    DIR *dptr = opendir(dir);

    store list[100];
    int i=0;
    while((d = readdir(dptr)) != NULL)
    {
        int color = 0;
        struct stat buff;
        if(op_a == 1)
        {
            if(op_l == 0)
            {
                char path[1024];
                sprintf(path, "%s/%s", dir, d->d_name);
                stat(path, &buff);

                if(S_ISDIR(buff.st_mode))
                    color = 1;
                else if (buff.st_mode & S_IXUSR)
                    color = 2;

                list[i].color = color;
                list[i++].name = d->d_name;
            }
            else
            {
                char path[1024];
                sprintf(path, "%s/%s", dir, d->d_name);
                stat(path, &buff);

                int uid = buff.st_uid;
                struct passwd* pw;
                pw = getpwuid(uid);

                int gid = buff.st_gid;
                struct group* gp;
                gp = getgrgid(gid);

                char mtime[128];
                time_t t = buff.st_mtime;
                struct tm* lt;
                lt = localtime(&t);

                char* perm = (char*)calloc(sizeof(char),11);
                perm[0] = ((S_ISDIR(buff.st_mode)) ? 'd' : '-');
                perm[1] = ((buff.st_mode & S_IRUSR) ? 'r' : '-');
                perm[2] = ((buff.st_mode & S_IWUSR) ? 'w' : '-');
                perm[3] = ((buff.st_mode & S_IXUSR) ? 'x' : '-');
                perm[4] = ((buff.st_mode & S_IRGRP) ? 'r' : '-');
                perm[5] = ((buff.st_mode & S_IWGRP) ? 'w' : '-');
                perm[6] = ((buff.st_mode & S_IXGRP) ? 'x' : '-');
                perm[7] = ((buff.st_mode & S_IROTH) ? 'r' : '-');
                perm[8] = ((buff.st_mode & S_IWOTH) ? 'w' : '-');
                perm[9] = ((buff.st_mode & S_IXOTH) ? 'x' : '-');
                perm[10] = '\0';
                
                if(S_ISDIR(buff.st_mode))
                    color = 1;
                else if (buff.st_mode & S_IXUSR)
                    color = 2;

                list[i].perms = perm;
                list[i].links = buff.st_nlink;
                list[i].user = pw->pw_name;
                list[i].grp = gp->gr_name;
                list[i].size = buff.st_size;

                if(lt->tm_mon == 0)
                    list[i].month = "JAN";
                else if (lt->tm_mon == 1)
                    list[i].month = "FEB";
                else if (lt->tm_mon == 2)
                    list[i].month = "MAR";
                else if (lt->tm_mon == 3)
                    list[i].month = "APR";
                else if (lt->tm_mon == 4)
                    list[i].month = "MAY";
                else if (lt->tm_mon == 5)
                    list[i].month = "JUN";
                else if (lt->tm_mon == 6)
                    list[i].month = "JUL";
                else if (lt->tm_mon == 7)
                    list[i].month = "AUG";
                else if (lt->tm_mon == 8)
                    list[i].month = "SEP";
                else if (lt->tm_mon == 9)
                    list[i].month = "OCT";
                else if (lt->tm_mon == 10)
                    list[i].month = "NOV";
                else if (lt->tm_mon == 11)
                    list[i].month = "DEC";

                list[i].date = lt->tm_mday;
                list[i].hours = lt->tm_hour;
                list[i].min = lt->tm_min;
                list[i].color = color;
                list[i++].name = d->d_name;
            }
        }
        else if(d->d_name[0] != '.')
        {
            if(op_l == 0)
            {
                char path[1024];
                sprintf(path, "%s/%s", dir, d->d_name);
                stat(path, &buff);

                if(S_ISDIR(buff.st_mode))
                    color = 1;
                else if (buff.st_mode & S_IXUSR)
                    color = 2;

                list[i].color = color;
                list[i++].name = d->d_name;
            }
            else
            {
                char path[1024];
                sprintf(path, "%s/%s", dir, d->d_name);
                stat(path, &buff);

                int uid = buff.st_uid;
                struct passwd* pw;
                pw = getpwuid(uid);

                int gid = buff.st_gid;
                struct group* gp;
                gp = getgrgid(gid);

                char mtime[128];
                time_t t = buff.st_mtime;
                struct tm* lt;
                lt = localtime(&t);

                char* perm = (char*)calloc(sizeof(char),11);
                perm[0] = ((S_ISDIR(buff.st_mode)) ? 'd' : '-');
                perm[1] = ((buff.st_mode & S_IRUSR) ? 'r' : '-');
                perm[2] = ((buff.st_mode & S_IWUSR) ? 'w' : '-');
                perm[3] = ((buff.st_mode & S_IXUSR) ? 'x' : '-');
                perm[4] = ((buff.st_mode & S_IRGRP) ? 'r' : '-');
                perm[5] = ((buff.st_mode & S_IWGRP) ? 'w' : '-');
                perm[6] = ((buff.st_mode & S_IXGRP) ? 'x' : '-');
                perm[7] = ((buff.st_mode & S_IROTH) ? 'r' : '-');
                perm[8] = ((buff.st_mode & S_IWOTH) ? 'w' : '-');
                perm[9] = ((buff.st_mode & S_IXOTH) ? 'x' : '-');
                perm[10] = '\0';

                if(S_ISDIR(buff.st_mode))
                    color = 1;
                else if (buff.st_mode & S_IXUSR)
                    color = 2;

                list[i].perms = perm;
                list[i].links = buff.st_nlink;
                list[i].user = pw->pw_name;
                list[i].grp = gp->gr_name;
                list[i].size = buff.st_size;
                blocks = buff.st_blocks;

                if(lt->tm_mon == 0)
                    list[i].month = "JAN";
                else if (lt->tm_mon == 1)
                    list[i].month = "FEB";
                else if (lt->tm_mon == 2)
                    list[i].month = "MAR";
                else if (lt->tm_mon == 3)
                    list[i].month = "APR";
                else if (lt->tm_mon == 4)
                    list[i].month = "MAY";
                else if (lt->tm_mon == 5)
                    list[i].month = "JUN";
                else if (lt->tm_mon == 6)
                    list[i].month = "JUL";
                else if (lt->tm_mon == 7)
                    list[i].month = "AUG";
                else if (lt->tm_mon == 8)
                    list[i].month = "SEP";
                else if (lt->tm_mon == 9)
                    list[i].month = "OCT";
                else if (lt->tm_mon == 10)
                    list[i].month = "NOV";
                else if (lt->tm_mon == 11)
                    list[i].month = "DEC";

                list[i].date = lt->tm_mday;
                list[i].hours = lt->tm_hour;
                list[i].min = lt->tm_min;
                list[i].color = color;
                list[i++].name = d->d_name;
            }
        }
    }

    qsort(list, i, sizeof(list[0]), cmpfunc);
    if(op_l == 0)
    {
        while(i--)
        {
            if(list[i].color == 0)
                printf("%s  ", list[i].name);
            else if(list[i].color == 1)
                printf("\033[1;34m%s\033[0m\t", list[i].name);
            else
                printf("\033[1;32m%s\033[0m\t", list[i].name);
        }
        printf("\n");
    }
    else
    {
        while(i--)
        {
            printf("%s %d %s %s %ld %s %d %d:%d ", list[i].perms, list[i].links, list[i].user, list[i].grp, list[i].size, list[i].month, list[i].date, list[i].hours, list[i].min);
            if(list[i].color == 0)
                printf("%s\n", list[i].name);
            else if(list[i].color == 1)
                printf("\033[1;34m%s\033[0m\n", list[i].name);
            else
                printf("\033[1;32m%s\033[0m\n", list[i].name);
        }
    }
}