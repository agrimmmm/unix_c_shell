#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

void pastevents(FILE* fptr);
void past_purge(int* flag);
void past_execute(int index, FILE* fptr, int* count);

#endif