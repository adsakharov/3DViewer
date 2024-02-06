#ifndef S21_TEST_H_
#define S21_TEST_H_

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "../affine_transformation.h"
#include "../parser.h"

#define RESET "\033[0m"
#define RED "\e[0;91m"
#define GREEN "\033[32m"
#define BLUE "\e[0;94m"

Suite *suite_1();
void fAll(t_object *obj);

#endif  // S21_TEST_H_
