#ifndef AFFINE_TRANSFORMATION_H
#define AFFINE_TRANSFORMATION_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"

void scale_increase(double value, t_object *object);
void scale_decrease(double value, t_object *object);
void move_x(double value, t_object *object);
void move_y(double value, t_object *object);
void move_z(double value, t_object *object);
void rotate_x(double value, t_object *object);
void rotate_y(double value, t_object *object);
void rotate_z(double value, t_object *object);

#endif  // AFFINE_TRANSFORMATION_H