#include "headers.h"

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

void removeExtraSpaces(char* str)
{
    while(str[strlen(str)-1] == ' ')
    {
        str[strlen(str)-1] = '\0';
    }

    int i, j = 0;
    int len = strlen(str);
    for (i = 0; i < len; i++)
    {
        if (str[i] != ' ')
            break;
    }

    while (i < len)
    {
        str[j] = str[i];
        i++;
        j++;
    }
    str[j] = '\0';
}