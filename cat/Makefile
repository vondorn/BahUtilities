CC = gcc -Wall -Werror -Wextra

all: s21_cat

s21_cat:
	$(CC) cat.c -o s21_cat

test:
	sh test_func_cat.sh

clean:
	rm -rf s21_cat

style:
	clang-format -n -style=google *.c *.h

format:
	clang-format -i -style=google *.c *.h

rebuild: clean s21_cat