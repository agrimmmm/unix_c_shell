#include "headers.h"

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

char* TakeInput() 
{
    char *inp = calloc(sizeof(char), 100);
    char c;
    while (1)
    {
        setbuf(stdout, NULL);
        enableRawMode();
        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) 
        {
            if (iscntrl(c))
            {
                if (c == 10) 
                    break;
                else if (c == 27)
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2)
                    { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                }
                else if (c == 4)
                {
                    printf("B-Bye :)\n");
                    exit(0);
                }
                else if (c == 127)
                { // backspace
                    if (pt > 0)
                    {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 9)
                { // TAB character
                    inp[pt++] = c;
                    for (int i = 0; i < 8; i++)
                    { // TABS should be 8 spaces
                        printf(" ");
                    }
                }
                else
                {
                    printf("%d\n", c);
                }
            }
            else
            {
            inp[pt++] = c; 
            printf("%c", c);
            }
        }
        printf("\n");
        disableRawMode();

        return inp;
    }
    return 0;
}