# MIG_generator
## Формат данных 
Каждая схема описывается 5-ю строками в текстовом файле.
1. Вектор-столбец функции переведенный в десятичную систему.
2. Класс схем, для MIG  это строка "mig" или "MIG"
3. Сложность схемы (количество элементов функции голосования)
4. Номер вершины выхода и флаг наличия отрицания (0 - нет отрицания, 1 - есть отрицание)
5. Последовательное описние вершин с помощью шестерок следующего типа: первые два числа определяют номер вершины, выход которой должен быть соединен с первым(самым левым) входом новой вершины, и флаг отрицания данного ребра (0 - нет, 1 - есть) аналогично для двух оставшихся входов (среднего и правого).
Изначально для каждой схемы определены 6 вершин, 0-ая вершина - константа 0, 1-5-ые вершины - входы схемы (x1~x5).
