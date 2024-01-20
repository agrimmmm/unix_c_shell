#ifndef __PEEK_H
#define __PEEK_H

typedef struct store
{
    char* name;
    char* month;
    int date;
    int hours;
    int min;
    long int size;
    char* perms;
    int links;
    char* user;
    char* grp;
    int color;
}store;

void peek(char* dir, int op_a, int op_l);

#endif