#include "s21_cat.h"

int main(int argc, char *argv[]) {
  FILE *file = NULL;
  int n = 1;
  cat_struct flags = {0};
  if (argc > 1 && find_flag(argv, &flags, &n)) {
    while (n < argc) {
      if ((file = fopen(argv[n], "r")) != NULL) {
        zavod(flags, file);
      } else
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[n]);
      n++;
    }
  }
  return 0;
}

int find_flag(char *argv[], cat_struct *flags, int *n) {
  int flag = 1;
  while (argv[*n][0] == '-') {
    for (int i = 1; i < (int)strlen(argv[*n]); i++) {
      if (argv[*n][1] != '-') {
        if (argv[*n][i] == 'n')
          flags->n = 1;
        else if (argv[*n][i] == 'b')
          flags->b = 1;
        else if (argv[*n][i] == 's')
          flags->s = 1;
        else if (argv[*n][i] == 'v')
          flags->v = 1;
        else if (argv[*n][i] == 't')
          flags->t = 1;
        else if (argv[*n][i] == 'e')
          flags->e = 1;
        else if (argv[*n][i] == 'T')
          flags->T = 1;
        else if (argv[*n][i] == 'E')
          flags->E = 1;
        else {
          flag = 0;
          printf(
              "cat: illegal option -- %c\nusage: cat [-belnstuv] [file ...]\n",
              argv[*n][i]);
        }
      } else if (!strcmp(argv[*n], "--number"))
        flags->n = 1;
      else if (!strcmp(argv[*n], "--number-nonblank"))
        flags->b = 1;
      else if (!strcmp(argv[*n], "--squeeze-blank"))
        flags->s = 1;
      else {
        flag = 0;
        printf("cat: unrecognized option '%s'\n", argv[*n]);
      }
    }
    *n += 1;
  }
  return flag;
}

void zavod(cat_struct flags, FILE *file) {
  char c, prev = '\n', preprev = ' ';
  int line_number = 1;
  while ((c = fgetc(file)) != EOF) {
    int v_flag = 0;
    if (flags.s) {
      if ((preprev == '\n' && prev == '\n' && c == '\n')) continue;
    }
    if ((flags.T || flags.t) && c == '\t') {
      v_flag = 1;
      c += 64;
    }
    if (flags.b && prev == '\n') {
      if (c != '\n') {
        printf("%6d\t", line_number);
        line_number++;
      } else if (flags.e)
        printf("      \t");
    }
    if (flags.n && !flags.b && prev == '\n') {
      printf("%6d\t", line_number);
      line_number++;
    }
    if ((flags.e || flags.E) && c == '\n') printf("$");
    if (flags.v || flags.e || flags.t) {
      if (c >= 0 && c <= 31 && c != '\n' && c != '\t') {
        v_flag = 1;
        c += 64;
      } else if (c == 127) {
        v_flag = 1;
        c -= 64;
      }
    }
    if (v_flag) printf("^");
    fputc(c, stdout);
    preprev = prev;
    prev = c;
  }
}