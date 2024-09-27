#!/bin/bash

# Проверим, переданы ли аргументы
if [ $# -eq 0 ]; then
  echo "No arguments provided."
  exit 1
fi

# Количество аргументов
count=$#

# Сумма всех аргументов
sum=0

# Подсчет суммы
for arg in "$@"; do
  sum=$((sum + arg))
done

# Вычисление среднего арифметического
average=$(echo "scale=2; $sum / $count" | bc)

# Вывод результатов
echo "Number of arguments: $count"
echo "Sum: $sum"
echo "Average: $average"
