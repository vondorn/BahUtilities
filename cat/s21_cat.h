#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct cat_struct {
  int n;
  int b;
  int s;
  int v;
  int t;
  int e;
  int T;
  int E;
} cat_struct;

int find_flag(char *argv[], cat_struct *flags, int *n);
void zavod(cat_struct flags, FILE *file);