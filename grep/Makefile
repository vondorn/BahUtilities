CC = gcc -Wall -Werror -Wextra

all: clean s21_grep

s21_grep:
	$(CC) s21_grep.c -o s21_grep

test:
	sh test_func_grep.sh

style:
	clang-format -n -style=google *.c *.h

format:
	clang-format -i -style=google *.c *.h

clean:
	rm -rf s21_grep

rebuild: clean s21_grep