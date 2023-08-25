UNAME := $(shell uname)

CC=gcc
CFLAGS= -Wall -Wextra -Wextra -std=c11 -pedantic

SOURCES=*.c
OBJECTS=*.o


all: clean decimal.a test

decimal.a:
	$(CC) $(CFLAGS) -c $(SOURCES)
	ar rc decimal.a $(OBJECTS)
	ranlib decimal.a
	rm -f *.o
	
ifeq ($(UNAME), Linux)
test: decimal.a
	$(CC) $(CFLAGS) -c tests/decimal_test.c
	ar rc decimal_test.a decimal_test.o
	ranlib decimal_test.a
	$(CC) $(CFLAGS) --coverage -o c_tests decimal.a decimal_test.a $(SOURCES) -lcheck -lm -lsubunit -lrt -lpthread
	./c_tests
else
test: decimal.a
	$(CC) $(CFLAGS) -c tests/decimal_test.c
	ar rc decimal_test.a decimal_test.o
	ranlib decimal_test.a
	$(CC) $(CFLAGS) --coverage -o c_tests decimal.a decimal_test.a $(SOURCES) -lcheck -lm -pthread
	./c_tests
endif

gcov_report:
	gcovr -r . --html --html-details -o ./html/coverage_report.html
	./c_tests
	rm -rf *.gcda *.gcno

cppcheck: 
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h ./tests/decimal_test.c
linter:
	python3 ../materials/linters/cpplint.py --extensions=c *.c *.h
check_all: cppcheck linter

valgrind: decimal.a
	touch valgrind.txt
	gcc -std=c11 ./tests/decimal_test.c decimal.a -o check_unit_tests `pkg-config --cflags --libs check`
	valgrind --log-file="valgrind.txt" --trace-children=yes --track-fds=yes --leak-check=full --show-leak-kinds=all --track-origins=yes ./c_tests

push: 
	git add .
	git commit -m "insertion"
	git push origin $(whoami)

clean:	
	rm -rf decimal.a decimal_test.a *.gcda *.gcno *.css test *.dSYM *.html *.o ./html/*.html ./html/*.css ./c_tests valgrind.txt ./check_unit_tests
