#include "revert_string.h"
#include <string.h>

void RevertString(char *str)
{
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        // Меняем местами символы на позициях i и (length - i - 1)
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}
