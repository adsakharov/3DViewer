#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>  // isdigit
#include <stdio.h>
#include <stdlib.h>  // malloc, atof
#include <string.h>  // strtok

typedef struct s_vertex {
  double x;
  double y;
  double z;
  int lineIndex;  // Номер строки с вершиной в файле. Надо для обработки
                  // negative reference numbers
} t_vertex;

typedef struct s_edge {  // Структура с данными по рёбрам
  int vertIndex[2];      // Индексы двух вершин ребра
} t_edge;

typedef struct s_object {
  t_vertex *v;  //Массив структур с данными по вершинам
  t_edge *e;  //Массив структур с данными по рёбрам
  double minMaxX[2];  //Минимальные и максимальные значения вершин по осям
  double minMaxY[2];
  double minMaxZ[2];
  int vAmount;      //Количество вершин
  int eAmount;      //Количество рёбер
  int minusAmount;  //Счётчик, используется для обработки negative reference
                    // numbers
} t_object;

int parser(const char *fname, t_object *object);
void setNulls(t_object *object);

#endif
