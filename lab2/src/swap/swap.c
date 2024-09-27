#include "swap.h"

void Swap(char *left, char *right)
{
    // Временная переменная для хранения значения
    char temp = *left;  // Сохраняем значение по адресу left
    *left = *right;     // Копируем значение по адресу right в left
    *right = temp;      // Копируем сохраненное значение в right
}
