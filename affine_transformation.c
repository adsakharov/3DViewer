#include "affine_transformation.h"

/**
 * Функция увеличения размера модели
 * @param value Значение, на которое увеличивается модель
 * @param object Структура объекта
 */
void scale_increase(double value, t_object *object) {
  for (int i = 0; i < object->vAmount; i++) {
    object->v[i].x *= value;
    object->v[i].y *= value;
    object->v[i].z *= value;
  }
}

/**
 * Функция уменьшения размера модели
 * @param value Значение, на которое уменьшается модель
 * @param object Структура объекта
 */
void scale_decrease(double value, t_object *object) {
  for (int i = 0; i < object->vAmount; i++) {
    object->v[i].x /= value;
    object->v[i].y /= value;
    object->v[i].z /= value;
  }
}

/**
 * Функция перемещения модели по координате Х
 * @param value Значение, на которое передвигается модель
 * @param object Структура объекта
 */
void move_x(double value, t_object *object) {
  for (int i = 0; i < object->vAmount; i++) {
    object->v[i].x += value;
  }
}

/**
 * Функция перемещения модели по координате Y
 * @param value Значение, на которое передвигается модель
 * @param object Структура объекта
 */
void move_y(double value, t_object *object) {
  for (int i = 0; i < object->vAmount; i++) {
    object->v[i].y += value;
  }
}

/**
 * Функция перемещения модели по координате Z
 * @param value Значение, на которое передвигается модель
 * @param object Структура объекта
 */
void move_z(double value, t_object *object) {
  for (int i = 0; i < object->vAmount; i++) {
    object->v[i].z += value;
  }
}

/**
 * Функция поворота модели на заданный угол по оси X
 * @param value Значение, на которое поворачивается модель
 * @param object Структура объекта
 */
void rotate_x(double value, t_object *object) {
  for (int i = 0; i < object->vAmount; i++) {
    double tmp_y = object->v[i].y;
    double tmp_z = object->v[i].z;

    object->v[i].y = cos(value) * tmp_y - sin(value) * tmp_z;
    object->v[i].z = sin(value) * tmp_y + cos(value) * tmp_z;
  }
}

/**
 * Функция поворота модели на заданный угол по оси Y
 * @param value Значение, на которое поворачивается модель
 * @param object Структура объекта
 */
void rotate_y(double value, t_object *object) {
  for (int i = 0; i < object->vAmount; i++) {
    double tmp_x = object->v[i].x;
    double tmp_z = object->v[i].z;

    object->v[i].x = cos(value) * tmp_x + sin(value) * tmp_z;
    object->v[i].z = -sin(value) * tmp_x + cos(value) * tmp_z;
  }
}

/**
 * Функция поворота модели на заданный угол по оси Z
 * @param value Значение, на которое поворачивается модель
 * @param object Структура объекта
 */
void rotate_z(double value, t_object *object) {
  for (int i = 0; i < object->vAmount; i++) {
    double tmp_x = object->v[i].x;
    double tmp_y = object->v[i].y;

    object->v[i].x = cos(value) * tmp_x - sin(value) * tmp_y;
    object->v[i].y = sin(value) * tmp_x + cos(value) * tmp_y;
  }
}
