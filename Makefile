CC = gcc
DELETE = rm -rf

SEARCH = winopengl.h
REPLASE = ..\/..\/3D-viewer-v1_0\/winopengl.h

VALGRIND_OUTPUT = valgrind.txt

BUILD_FLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -fanalyzer
VALGRIND_FLAGS = -s --log-file="$(VALGRIND_OUTPUT)" --track-origins=yes --trace-children=yes --leak-check=full --leak-resolution=med
LIBS_FLAGS = -lcheck

SRC_C_FILES = parser.c affine_transformation.c
TEST_C_FILES = test_cases/s21_test.c test_cases/s21_suite.c

TEST_TARGET  = s21_test

REPORT_DIR = report

INCLUDES = . 

BUILD_DIR = ../build/

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS_FLAGS += -lsubunit -lm
endif

all: clean test gcov_report

test: format $(SRC_C_FILES) viewer.a
	$(CC) $(BUILD_FLAGS) $(addprefix -I , $(INCLUDES))	 $(SRC_C_FILES) $(TEST_C_FILES) -o $(TEST_TARGET) $(LIBS_FLAGS)
	./$(TEST_TARGET)

viewer.a:
	$(CC) -c $(SRC_C_FILES)
	ar rcs viewer.a *.o
	ranlib $@
	# cp viewer.a gui/3D-viewer-v1_0
	# cp *.o gui/3D-viewer-v1_0

gcov_report: format
	$(CC) --coverage $(BUILD_FLAGS) $(addprefix -I , $(INCLUDES)) $(SRC_C_FILES) $(TEST_C_FILES) -o $(TEST_TARGET) $(LIBS_FLAGS)
	./$(TEST_TARGET)
	lcov --no-external  -t "$(TEST_TARGET)" -o $(TEST_TARGET).info -c -d .
	genhtml -o report $(TEST_TARGET).info
	rm *.gcda *.gcno *.info
	open report/index.html

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .

uninstall:
	$(DELETE) build

install: uninstall viewer.a
	mkdir build
	cd gui && cmake 3D-viewer-v1_0
	#cp -R gui/3D-viewer-v1_0_autogen/include/ui_mainwindow.h gui/3D-viewer-v1_0
	cd gui && make
	cp -R gui/3D-viewer-v1_0.app build

clean:
	$(DELETE) $(TEST_TARGET)
	$(DELETE) $(REPORT_DIR)
	$(DELETE) *.info
	$(DELETE) *.o
	$(DELETE) *.gc*
	$(DELETE) *.a
	$(DELETE) viewer.tar.gz
	$(DELETE) viewer.app
	$(DELETE) $(VALGRIND_OUTPUT)
	cd gui/3D-viewer-v1_0 && $(DELETE) viewer.a parser.o && cd ../..
	cd gui && $(DELETE) 3D-viewer-v1_0_autogen CMake* cmake* 3D-viewer-v1_0.app Makefile && cd ..

valgrind: format test
	valgrind $(VALGRIND_FLAGS) ./$(TEST_TARGET)
	grep --color=always -n "ERROR SUMMARY" $(VALGRIND_OUTPUT) | grep -v "0 errors from 0 contexts" || true
	grep --color=always -n "definitely lost:" $(VALGRIND_OUTPUT) | grep -v "0 bytes in 0 blocks" || true

leak:
	leaks -atExit -- ./$(TEST_TARGET)

dvi:
	open info.html

dist: install
	tar -czf 3D-viewer-v1_0.tar.gz gui/3D-viewer-v1_0.app
	cp 3D-viewer-v1_0.tar.gz build

clang:
	cp ../materials/linters/.clang-format .
	clang-format -style=Google -n *.h *.c test_cases/*.c test_cases/*.h gui/*.h
	rm -f .clang-format

format:
	cp ../materials/linters/.clang-format .
	clang-format -style=Google *.c -i
	clang-format -style=Google *.h -i
	clang-format -style=Google test_cases/*.c -i
	clang-format -style=Google test_cases/*.h -i
	clang-format -style=Google gui/3D-viewer-v1_0/*.h gui/3D-viewer-v1_0/*.cpp -i
	rm -rf .clang-format
