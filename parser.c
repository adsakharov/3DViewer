#include "parser.h"

// Проверка, что первый символ строки '-' или число
static int check_number(const char *word) {
  int ret = 0;
  if (!word)
    ret = 0;
  else if (isdigit(word[0]))
    ret = 1;
  else if (word[0] == '-')
    ret = -1;
  return ret;
}

// Устанавливает стартовые значения для структуры t_object.
void setNulls(t_object *object) {
  object->vAmount = 0;
  object->eAmount = 0;
  object->minusAmount = 0;
  return;
}

// Возвращает 'N', если строка завершается '\n' или файл прочитан до конца,
// и 'X', если нет.
static char searchN(const char *line, FILE *file) {
  size_t i = 0;
  char flag = 'X';
  while (line[i] != '\n' && line[i] != '\0') i++;
  if (line[i] == '\n')
    flag = 'N';
  else if (feof(file))
    flag = 'N';
  return flag;
}

// Возвращает изменения в структуре t_object.
// Считает количество рёбер в одной строке файла.
// Считает количество ссылок на вершины в формате negative reference.
// Строка на входе должна быть не менее 2 символов длиной при *tooLong == 'N',
// иначе функция может не работать или работать неправильно.
// Переменная tooLong принимает значения: по умолчанию 'N' - строка
// заканчивается '\n' или достигнут конец файла. 'X' - строка длинная, на конце
// нет '\n'. 'Y' - строка содержит данные о поверхностях, слишком длинная для
// анализа за 1 раз, последний символ анализа space ,'T' - строка содержит
// данные о поверхностях, слишком длинная для анализа за 1 раз, последний символ
// анализа не space.
static void countEdges(const char *line, t_object *object, char *tooLong,
                       FILE *file) {
  int i = 0;
  int flag = 0;
  if (*tooLong == 'T')
    while (line[i] != ' ' || line[i] != '\0') i++;
  else if (*tooLong == 'N')
    i = 2;
  *tooLong = searchN(line, file);
  while (line[i] != '\0' && flag == 0) {
    while (line[i] == ' ') i++;
    if (isdigit(line[i])) {
      object->eAmount++;
    } else if (line[i] == '-') {
      object->minusAmount++;
      object->eAmount++;
    } else {
      flag = 1;
    }
    if (flag == 0)
      while (line[i] != ' ' && line[i] != '\0') i++;
  }
  if (flag == 0 && i > 0) {
    if (line[i] == '\0' && line[i - 1] != '\n') {
      if (line[i - 1] == ' ')
        *tooLong = 'Y';
      else
        *tooLong = 'T';
    }
  }
  return;
}

// Первое прочтение файла, подсчёт количества вершин и рёбер.
// В переменную isTooLong записывается информация о наличии в файле
// строк длинее 1000 символов. Сделана обработка подсчёта количества рёбер для
// длинных строк, начинающихся с "f".
static void first_reading(t_object *object, FILE *file, char *isTooLong) {
  char tooLong = 'N';
  char line[1000];
  while (!feof(file)) {
    fgets(line, 1000, file);
    if (tooLong == 'N') {
      if (strncmp(line, "f ", 2) == 0) {
        countEdges(line, object, &tooLong, file);
      } else {
        tooLong = searchN(line, file);
        if (strncmp(line, "v ", 2) == 0) object->vAmount++;
      }
    } else if (tooLong == 'T' || tooLong == 'Y') {
      *isTooLong = 'Y';
      countEdges(line, object, &tooLong, file);
    } else {
      *isTooLong = 'Y';
    }
  }
  bzero(line, 1000);
  return;
}

// Сравниваются значения текущей вершины по осям X, Y, Z
// с минимальными и максимальными значениями по всему объекту,
// после чего данные minMax обновляются при необходимости.
static void compareMinMax(t_object *object, int index) {
  if (index == 0) {
    object->minMaxX[0] = object->v[0].x;
    object->minMaxX[1] = object->v[0].x;
    object->minMaxY[0] = object->v[0].y;
    object->minMaxY[1] = object->v[0].y;
    object->minMaxZ[0] = object->v[0].z;
    object->minMaxZ[1] = object->v[0].z;
  } else {
    if (object->v[index].x < object->minMaxX[0])
      object->minMaxX[0] = object->v[index].x;
    else if (object->v[index].x > object->minMaxX[1])
      object->minMaxX[1] = object->v[index].x;
    if (object->v[index].y < object->minMaxY[0])
      object->minMaxY[0] = object->v[index].y;
    else if (object->v[index].y > object->minMaxY[1])
      object->minMaxY[1] = object->v[index].y;
    if (object->v[index].z < object->minMaxZ[0])
      object->minMaxZ[0] = object->v[index].z;
    else if (object->v[index].z > object->minMaxZ[1])
      object->minMaxZ[1] = object->v[index].z;
  }
}

// Данные о координатах вершины вносятся из строки в структуру.
// В случае, если в данных не число, функция досрочно завершается,
// возвращая число сохранённых в структуру координат.
static int vTake(char *line, t_object *object, int index, int lineIndex) {
  int ret = 3;
  object->v[index].lineIndex = lineIndex;
  char *vString;
  vString = strtok(line, " ");
  vString = strtok(NULL, " ");
  if (check_number(vString) == 0) {
    ret = 0;
  } else {
    object->v[index].x = atof(vString);
    vString = strtok(NULL, " ");
    if (check_number(vString) == 0) {
      ret = 1;
    } else {
      object->v[index].y = atof(vString);
      vString = strtok(NULL, " ");
      if (check_number(vString) == 0) {
        ret = 2;
      } else {
        object->v[index].z = atof(vString);
      }
    }
  }
  compareMinMax(object, index);
  return ret;
}

// Возвращает индекс вершины в обычном формате (порядковый номер),
// имея на входе формат Negative reference number:
// v 0.000000 2.000000 2.000000
// v 0.000000 0.000000 2.000000
// v 2.000000 0.000000 2.000000
// v 2.000000 2.000000 2.000000
// f -4 -3 -2 -1
//
// v 2.000000 2.000000 0.000000
// v 2.000000 0.000000 0.000000
// v 0.000000 0.000000 0.000000
// v 0.000000 2.000000 0.000000
// f -4 -3 -2 -1
// Делает проверку, что строки с вершинами идут строго перед строкой face
// с форматом Negative reference number. Если условие не соблюдается,
// возвращает -1.
// Возвращает -1, если подано не отрицательное значение number, в т.ч. 0.
static int process_0_minus(int number, t_object *object, int latestIndexV,
                           int currentLineIndex) {
  int ret = -1;
  if (number < 0) {
    int vLineIndex = currentLineIndex + number;
    int indexVReference = latestIndexV + number + 1;
    if (object->v[indexVReference - 1].lineIndex == vLineIndex)
      ret = indexVReference;
  }
  return ret;
}

// Запись информации о поверхностях. Индексы вершин вносятся в массив рёбер.
// Функция предназначена для 2-ого и 3-его индекса в строке - осуществляется
// проверка, что в функции как минимум 3 вершины. Если индекс равен 0, то
// случай считается ошибочным и возвращается -1.
// Реализована обработка negative reference number. Если запись отрицательных
// индексов реализована неправильно, функция также возвращает -1.
// Если формат записи в строке верный, функция возвращает 0.
static int eTake_ensure3Vertices(char *eString, t_object *object, int *iE,
                                 int latestIndexV, int lineIndex) {
  int ret = 0;
  object->e[*iE].vertIndex[1] = atoi(eString);
  if (object->e[*iE].vertIndex[1] <= 0)
    object->e[*iE].vertIndex[1] = process_0_minus(
        object->e[*iE].vertIndex[1], object, latestIndexV, lineIndex);
  if (object->e[*iE].vertIndex[1] <= 0) {
    ret = -1;
  } else {
    object->e[*iE].vertIndex[1]--;
    *iE = *iE + 1;
    object->e[*iE].vertIndex[0] = object->e[*iE - 1].vertIndex[1];
  }
  return ret;
}

// Вносит данные в object о рёбрах в формате индексов двух вершин.
// Параллельно изменяет переменную iE - индекс ребра.
// Возвращает -1, если данные в строке не соответствуют ожидаемому формату.
// При нормальной работе возвращает 0;
// Обрабатывает negative reference number.
// Функция предназначена для строки начинающейся с 'f ', считанной целиком до
// конца. Функция не проверяет указанные условия, ожидая, что они уже проверены.
// Функция может неправильно обработать данные, если на исходной строке слишком
// много вершин и исходные данные обрезаны в случайном месте.
static int eTake(char *line, t_object *object, int *iE, int latestIndexV,
                 int lineIndex) {
  int ret = 0;
  char *eString;
  int firstV;
  eString = strtok(line, " ");
  eString = strtok(NULL, " ");
  firstV = atoi(eString);
  if (firstV <= 0) {
    firstV = process_0_minus(firstV, object, latestIndexV, lineIndex);
    if (firstV <= 0) ret = (-1);
  }
  object->e[*iE].vertIndex[0] = firstV - 1;
  int i = 0;
  while (i < 2 && ret == 0) {
    eString = strtok(NULL, " ");
    if (eString == NULL)
      ret = -1;  // меньше трёх вершин в поверхности
    else
      ret = eTake_ensure3Vertices(eString, object, iE, latestIndexV, lineIndex);
    i++;
  }
  if (ret == 0) {
    eString = strtok(NULL, " ");
    while (eString != NULL && ret == 0) {
      object->e[*iE].vertIndex[1] = atoi(eString);
      if (object->e[*iE].vertIndex[1] == 0) {
        ret = 1;  // конец числовых переменных, начало текста в строке
      } else if (object->e[*iE].vertIndex[1] < 0) {
        object->e[*iE].vertIndex[1] = process_0_minus(
            object->e[*iE].vertIndex[1], object, latestIndexV, lineIndex);
        if (object->e[*iE].vertIndex[1] < 0)
          ret = -1;  // неправильная запись negative reference number
      }
      if (ret >= 0) {
        object->e[*iE].vertIndex[1]--;
        *iE = *iE + 1;
        object->e[*iE].vertIndex[0] = object->e[*iE - 1].vertIndex[1];
        eString = strtok(NULL, " ");
      }
    }
  }
  if (ret >= 0) {  // последнее ребро на строке, ставится индекс
    object->e[*iE].vertIndex[1] = firstV - 1;  // первой вершины поверхности
    *iE = *iE + 1;
  }
  return ret;
}

// Считывает данные о вершинах и рёбрах из файла, сохраняя их в object.
// Возвращает 0, если данные в файле имеют ожидаемый формат.
// Возвращает -1, если формат данных в файле не соответствует ожидаемому.
// Функция должна работать без ошибок при условии,
// что строки в файле короче 1000 символов.
static int dataTakeBasic(FILE *file, t_object *object) {
  int ret = 0;
  int lineIndex = -1;
  int indexV = 0;
  int iE = 0;
  char line[1000];
  while (!feof(file) && ret == 0) {
    fgets(line, 1000, file);
    lineIndex++;
    if (strncmp(line, "v ", 2) == 0) {
      if (vTake(line, object, indexV, lineIndex) != 3) {
        bzero(line, 1000);
        ret = -1;
      }
      indexV++;
    } else if (strncmp(line, "f ", 2) == 0) {
      if (eTake(line, object, &iE, indexV, lineIndex) < 0) {
        bzero(line, 1000);
        ret = -1;
      };
    }
  }
  return ret;
}

// Открывает файл, считает количество вершин и рёбер,
// маллочит для структуры object память для записи информации о рёбрах и
// вершинах, записывает в структуру object информацию о рёбрах, вершинах,
// минимальных максимальных значениях координат.
// Функция проверяет наличие длинных строк в файле (более 999 символов)
// и делает обработку подсчёта числа вершин и рёбер для данного случая.
// Функция не обрабатывает чтение данных о вершинах и рёбрах для длинных строк,
// поставлена заглушка - случай считается ошибочным. Потенциально можно
// доработать.
int parser(const char *fname, t_object *object) {
  int ret = 0;
  char isTooLong = 'N';
  FILE *file = fopen(fname, "r");
  if (!file) {
    ret = -1;
  } else {
    setNulls(object);
    first_reading(object, file, &isTooLong);
    if (object->minusAmount > 0 && object->minusAmount != object->eAmount) {
      ret = -1;  // Запись типа f 5 -4 2. Считаю такую запись ошибкой.
    }
  }
  if (ret == 0) {
    object->v = (t_vertex *)malloc(sizeof(t_vertex) * object->vAmount);
    if (!(object->v) && ret == 0) {
      free(object->v);
      ret = -1;
    } else {
      object->e = (t_edge *)malloc(sizeof(t_edge) * object->eAmount);
      if (!(object->e) && ret == 0) {
        free(object->v);
        free(object->e);
        ret = -1;
      }
    }
  }
  if (ret == 0) {
    rewind(file);  // Возврат к началу файла
    if (isTooLong == 'N') {
      if (dataTakeBasic(file, object) < 0) {
        free(object->v);
        free(object->e);
        ret = -1;
      }
    } else {
      free(object->v);
      free(object->e);
      ret = -1;
    }
  }
  fclose(file);
  return ret;
}
