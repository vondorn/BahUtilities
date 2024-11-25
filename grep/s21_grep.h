#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grep {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int n_strings;
  char strings[50][50];
  int count_files;
  char files[50][50];
  int print_all_lines;
} grep;

int parser(char *argv[], grep *flags, int argc);
int ef_check(char *argv[], int argc);
int find_flag(char *string, grep *flags, char *string_next);
void todo_grep(grep flags, FILE *file, char *path);
char *regex_check(grep flags, char *str_file, char *str);
void print_grep(grep flags, char *path, char *str_file, int line_number);
void print_cl(grep flags, char *path, int count_lines);
void from_file(grep *flags, char *path);