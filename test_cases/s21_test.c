#include "s21_test.h"

void tests_func_s21(Suite *s);

int main() {
  tests_func_s21(suite_1());
  return 0;
}

void tests_func_s21(Suite *s) {
  SRunner *runner = srunner_create(s);
  srunner_set_fork_status(runner, CK_NOFORK);
  srunner_run_all(runner, CK_NORMAL);
  srunner_free(runner);
}

void fAll(t_object *obj) {
  free(obj->e);
  free(obj->v);
}