# MIG_generator
## Формат данных 
Каждая схема описывается 5-ю строками в текстовом файле.
1. Вектор-столбец функции переведенный в десятичную систему.
2. Класс схем, для MIG  это строка "mig"
3. Сложность схемы (количество элементов функции голосования)
4. Номер вершины выхода и флаг наличия отрицания (0 - есть отрицание, 1 - нет отрицания)
5. Последовательное описние вершин с помощью шестерок следкющего типа: первые два числа определяют номер вершины, который должен быть соединен с первым входом ребром, и флаг отрицания данного ребра (0 - есть, 1 - нет) аналогично для двух оставшихся входов.
