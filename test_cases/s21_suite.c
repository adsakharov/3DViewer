#include "s21_test.h"

START_TEST(test1) {
  t_object obj;
  setNulls(&obj);
  char str[100] = "\0";
  int err = parser(str, &obj);
  ck_assert_int_eq(err, -1);
}
END_TEST

START_TEST(test2) {
  t_object obj;
  setNulls(&obj);
  char str[100] = "./test_cases/2.obj\0";
  int err = parser(str, &obj);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(obj.vAmount, 8);
  ck_assert_int_eq(obj.eAmount, 8);
  fAll(&obj);
}
END_TEST

START_TEST(test3) {
  t_object obj;
  setNulls(&obj);
  char str[100] = "./test_cases/cube.obj\0";
  int err = parser(str, &obj);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(obj.vAmount, 8);
  ck_assert_int_eq(obj.eAmount, 36);
  fAll(&obj);
}
END_TEST

START_TEST(test4) {
  t_object obj;
  setNulls(&obj);
  char str[100] = "./test_cases/icosphere.obj\0";
  int err = parser(str, &obj);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(obj.vAmount, 42);
  ck_assert_int_eq(obj.eAmount, 243);
  fAll(&obj);
}
END_TEST

START_TEST(test5) {
  t_object obj;
  setNulls(&obj);
  char str[100] = "./test_cases/cubes.txt\0";
  int err = parser(str, &obj);
  ck_assert_int_eq(err, -1);
}
END_TEST

START_TEST(test6) {
  t_object obj;
  setNulls(&obj);
  char str[100] = "./test_cases/err.obj\0";
  int err = parser(str, &obj);
  ck_assert_int_eq(err, -1);
}
END_TEST

START_TEST(test7) {
  t_object obj;
  setNulls(&obj);
  char str[100] = "./test_cases/err2.obj\0";
  int err = parser(str, &obj);
  ck_assert_int_eq(err, -1);
}
END_TEST

START_TEST(test8) {
  t_object obj;
  setNulls(&obj);
  char str[100] = "./test_cases/err3.obj\0";
  int err = parser(str, &obj);
  ck_assert_int_eq(err, -1);
}
END_TEST

START_TEST(test9) {
  t_object obj, obj2;
  setNulls(&obj);
  setNulls(&obj2);
  char str[100] = "./test_cases/cube.obj\0";
  int err = parser(str, &obj);
  int err2 = parser(str, &obj2);

  move_x(1.0, &obj2);
  move_y(1.0, &obj2);
  move_z(1.0, &obj2);

  move_x(-1.0, &obj2);
  move_y(-1.0, &obj2);
  move_z(-1.0, &obj2);

  for (int i = 0; i < obj.vAmount; i++) {
    ck_assert_double_eq(obj.v[i].x, obj2.v[i].x);
    ck_assert_double_eq(obj.v[i].y, obj2.v[i].y);
    ck_assert_double_eq(obj.v[i].z, obj2.v[i].z);
  }

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(err2, 0);
  fAll(&obj);
  fAll(&obj2);
}
END_TEST

START_TEST(test10) {
  t_object obj, obj2;
  setNulls(&obj);
  setNulls(&obj2);
  char str[100] = "./test_cases/cube.obj\0";
  int err = parser(str, &obj);
  int err2 = parser(str, &obj2);

  scale_increase(2, &obj);
  scale_decrease(2, &obj);

  for (int i = 0; i < obj.vAmount; i++) {
    ck_assert_double_eq(obj.v[i].x, obj2.v[i].x);
    ck_assert_double_eq(obj.v[i].y, obj2.v[i].y);
    ck_assert_double_eq(obj.v[i].z, obj2.v[i].z);
  }

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(err2, 0);
  fAll(&obj);
  fAll(&obj2);
}
END_TEST

START_TEST(test11) {
  t_object obj, obj2;
  setNulls(&obj);
  setNulls(&obj2);
  char str[100] = "./test_cases/cube.obj\0";
  int err = parser(str, &obj);
  int err2 = parser(str, &obj2);

  double degree = 50.0;

  rotate_x(degree, &obj2);
  rotate_x(-degree, &obj2);

  rotate_y(degree, &obj2);
  rotate_y(-degree, &obj2);

  rotate_z(degree, &obj2);
  rotate_z(-degree, &obj2);

  for (int i = 0; i < obj.vAmount; i++) {
    ck_assert_double_eq(obj.v[i].x, obj2.v[i].x);
    ck_assert_double_eq(obj.v[i].y, obj2.v[i].y);
    ck_assert_double_eq(obj.v[i].z, obj2.v[i].z);
  }

  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(err2, 0);
  fAll(&obj);
  fAll(&obj2);
}
END_TEST

START_TEST(test12) {
  t_object obj;
  setNulls(&obj);
  char str[100] = "./test_cases/1.obj\0";
  int err = parser(str, &obj);
  ck_assert_int_eq(err, -1);
}
END_TEST

Suite *suite_1(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("1");
  tc = tcase_create("1");
  tcase_add_test(tc, test1);
  tcase_add_test(tc, test2);
  tcase_add_test(tc, test3);
  tcase_add_test(tc, test4);
  tcase_add_test(tc, test5);
  tcase_add_test(tc, test6);
  tcase_add_test(tc, test7);
  tcase_add_test(tc, test8);
  tcase_add_test(tc, test9);
  tcase_add_test(tc, test10);
  tcase_add_test(tc, test11);
  tcase_add_test(tc, test12);

  suite_add_tcase(s, tc);
  return s;
}