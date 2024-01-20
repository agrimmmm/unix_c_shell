#ifndef __RAW_H
#define __RAW_H

void die(const char *s);

void disableRawMode();

void enableRawMode();

char* TakeInput();

#endif