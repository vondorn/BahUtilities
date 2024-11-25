#include "s21_grep.h"

int main(int argc, char *argv[]) {
  FILE *file = NULL;
  grep flags = {0};
  if (parser(argv, &flags, argc)) {
    for (int file_n = 0; file_n < flags.count_files; file_n++) {
      if ((file = fopen(flags.files[file_n], "r")) != NULL) {
        todo_grep(flags, file, flags.files[file_n]);
        fclose(file);
      } else if (!flags.s)
        fprintf(stderr, "grep: %s: No such file or directory\n",
                flags.files[file_n]);
    }
  }
}

int parser(char *argv[], grep *flags, int argc) {
  int check = 1, index = 1;
  flags->count_files = 0;
  flags->n_strings = 0;
  int ef = ef_check(argv, argc);
  if (argc > 2) {
    while (index < argc) {
      if (argv[index][0] == '-') {
        check = find_flag(argv[index], flags, argv[index + 1]);
        if (argv[index][strlen(argv[index]) - 1] == 'e' ||
            argv[index][strlen(argv[index]) - 1] == 'f')
          index++;
      } else if (!flags->n_strings && !ef) {
        strcpy(flags->strings[0], argv[index]);
        flags->n_strings = 1;
      } else {
        strcpy(flags->files[flags->count_files], argv[index]);
        flags->count_files++;
      }
      index++;
    }
  } else
    check = 0;
  if (flags->v) flags->o = 0;
  return check;
}

int ef_check(char *argv[], int argc) {
  int flag = 0;
  for (int index = 1; index < argc; index++) {
    if (argv[index][0] == '-' &&
        (strstr(argv[index], "e") || strstr(argv[index], "f")))
      flag = 1;
  }
  return flag;
}

int find_flag(char *string, grep *flags, char *string_next) {
  int check = 1;
  for (int j = 1; j < (int)strlen(string); j++) {
    if (string[j] == 'e') {
      flags->e = 1;
      if (j == (int)strlen(string) - 1)
        strcpy(flags->strings[flags->n_strings], string_next);
      else
        strcpy(flags->strings[flags->n_strings], string + j + 1);
      j = (int)strlen(string);
      flags->n_strings++;
    } else if (string[j] == 'i')
      flags->i = 1;
    else if (string[j] == 'v')
      flags->v = 1;
    else if (string[j] == 'c')
      flags->c = 1;
    else if (string[j] == 'l')
      flags->l = 1;
    else if (string[j] == 'n')
      flags->n = 1;
    else if (string[j] == 'h')
      flags->h = 1;
    else if (string[j] == 's')
      flags->s = 1;
    else if (string[j] == 'o')
      flags->o = 1;
    else if (string[j] == 'f') {
      flags->e = 1;
      if (j == (int)strlen(string) - 1)
        from_file(flags, string_next);
      else
        from_file(flags, string + j + 1);
      j = (int)strlen(string);
    } else {
      check = 0;
      fprintf(stderr, "grep: invalid option -- %c\n", string[1]);
    }
  }
  return check;
}

void todo_grep(grep flags, FILE *file, char *path) {
  char str_file[4096] = "";
  int count_lines = 0, line_number = 1, check = 0;
  while (fgets(str_file, 4096, file) != NULL) {
    check = 0;
    for (int i = 0; i < flags.n_strings; i++) {
      char *strq;
      if (flags.o) {
        while ((strq = regex_check(flags, str_file, flags.strings[i])) !=
               NULL) {
          check = 1;
          char qwerty[4096] = "";
          memcpy(qwerty, strq, (int)strlen(flags.strings[i]));
          memcpy(str_file, strq + strlen(flags.strings[i]), (int)strlen(strq));
          if (flags.e && flags.v) check--;
          if (check) {
            if (!flags.c && !flags.l)
              print_grep(flags, path, qwerty, line_number);
          }
        }
      } else if (regex_check(flags, str_file, flags.strings[i])) {
        check = 1;
      }
    }
    if (flags.e && flags.v) check--;
    if (check) {
      if (!flags.c && !flags.l && !flags.o)
        print_grep(flags, path, str_file, line_number);
      else
        count_lines++;
    }
    line_number++;
  }
  fseek(file, -1, SEEK_END);
  char c;
  if (flags.c || flags.l)
    print_cl(flags, path, count_lines);
  else if (check && (c = fgetc(file)) != '\n' && !flags.o)
    printf("\n");
}

void print_grep(grep flags, char *path, char *str_file, int line_number) {
  if (flags.n) {
    if (flags.count_files > 1 && !flags.h)
      printf("%s:%d:%s", path, line_number, str_file);
    else
      printf("%d:%s", line_number, str_file);
  } else {
    if (flags.count_files > 1 && !flags.h)
      printf("%s:%s", path, str_file);
    else
      printf("%s", str_file);
  }
  if (flags.o) printf("\n");
}

void print_cl(grep flags, char *path, int count_lines) {
  if (flags.c && flags.l && count_lines > 0) count_lines = 1;
  if (flags.c) {
    if (flags.count_files > 1 && !flags.h)
      printf("%s:%d\n", path, count_lines);
    else
      printf("%d\n", count_lines);
  }
  if (flags.l) {
    if (count_lines > 0) printf("%s\n", path);
  }
}

char *regex_check(grep flags, char *str_file, char *str) {
  char *ptr = NULL;
  regex_t regex;
  regmatch_t match;
  int reti;
  if (flags.i)
    reti = regcomp(&regex, str, REG_ICASE);
  else
    reti = regcomp(&regex, str, REG_EXTENDED);
  if (reti == 0) reti = regexec(&regex, str_file, 1, &match, 0);
  if (reti == 0) ptr = str_file + match.rm_so;
  if (flags.v && !flags.e) {
    if (ptr == NULL)
      ptr = str_file;
    else
      ptr = NULL;
  }
  if (!strcmp(str, ".")) {
    if (strcmp(str_file, "\n"))
      ptr = str_file;
    else
      ptr = NULL;
  }
  if (flags.print_all_lines) ptr = str_file;
  regfree(&regex);
  return ptr;
}

void from_file(grep *flags, char *path) {
  char str_file[4096] = "";
  FILE *file = NULL;
  if ((file = fopen(path, "r"))) {
    while (fgets(str_file, 4096, file) != NULL && strcmp(str_file, "\0")) {
      if (str_file[(int)strlen(str_file) - 1] == '\n' &&
          strcmp(str_file, "\n")) {
        str_file[(int)strlen(str_file) - 1] = '\0';
      }
      if (!strcmp(str_file, "\n")) flags->print_all_lines = 1;
      strcpy(flags->strings[flags->n_strings], str_file);
      flags->n_strings++;
    }
    fclose(file);
  } else
    fprintf(stderr, "grep: %s: No such file or directory\n", path);
}