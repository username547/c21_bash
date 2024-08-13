#include "grep.h"

void parse_command_line(int argc, char *argv[], options *opts) {
  int opt;
  while ((opt = getopt(argc, argv, "+eivclnhso")) != -1) {
    switch (opt) {
      case 'e':
        opts->e = 1;
        break;
      case 'i':
        opts->i = 1;
        break;
      case 'v':
        opts->v = 1;
        break;
      case 'c':
        opts->c = 1;
        break;
      case 'l':
        opts->l = 1;
        break;
      case 'n':
        opts->n = 1;
        break;
      case 'h':
        opts->h = 1;
        break;
      case 's':
        opts->s = 1;
        break;
      case 'o':
        opts->o = 1;
        break;
      default:
        fprintf(stderr, "grep [OPTION] [TEMPLATE] [FILE]...\n");
        exit(EXIT_FAILURE);
    }
  }
  if (!(opts->e || opts->i || opts->v || opts->c || opts->l || opts->n ||
        opts->h || opts->s || opts->o)) {
    opts->e = 1;
  }
}

int compilation(regex_t *regex, char *pattern, options opts) {
  int result;
  // REG_ICASE - игнорирование регистра символов
  // REG_EXTENDED - расширенные возможности регулярных выражений (+ - ? !)
  if (opts.i)
    result = regcomp(regex, pattern, (REG_ICASE | REG_EXTENDED));
  else
    result = regcomp(regex, pattern, REG_EXTENDED);
  return result;
}

void process_file(options *opts, FILE *file, char *filename, char *pattern,
                  int multifile) {
  char buffer[BUFFER_SIZE];
  char buffer_copy[BUFFER_SIZE];
  int line_number = 1, matching_lines = 0, compilation_result, rc, offset = 0,
      check_matches;
  regex_t regex;
  regmatch_t regex_matches[MAX_MATCHES];

  compilation_result = compilation(&regex, pattern, *opts);
  if (compilation_result != 0) {
    printf("Error: regex compilation");
    regfree(&regex);
    return;
  }

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    // Проверяем, завершена ли строка символом новой строки
    int len = strlen(buffer);
    if (len == 0 || buffer[len - 1] != '\n') {
      // Если строка не завершена символом новой строки, добавляем его в конец
      buffer[len] = '\n';
      buffer[len + 1] = '\0';
    }

    strcpy(buffer_copy, buffer);
    // поиск совпадений регулярного выражения (одно совпадение и доп. флаги)
    rc = regexec(&regex, buffer, 1, regex_matches, 0);
    check_matches = (rc == 0 && !opts->v) || (rc != 0 && opts->v);

    if ((opts->e || opts->i || opts->c || opts->l || opts->n || opts->h ||
         opts->s || opts->o || opts->v) &&
        check_matches) {
      if (opts->c) {
        matching_lines++;
      }
      if (opts->l) {
        fprintf(stdout, "%s\n", filename);
        break;
      }
      if ((opts->i || opts->v || opts->e || (opts->h && !opts->o) || opts->s) &&
          opts->n) {
        multifile && (!opts->h)
            ? fprintf(stdout, "%s:%d:%s", filename, line_number, buffer_copy)
            : fprintf(stdout, "%d:%s", line_number, buffer_copy);
      } else if ((opts->i || opts->v || opts->e || (opts->h && !opts->o) ||
                  opts->s) &&
                 (!opts->c) && (!opts->o)) {
        multifile && (!opts->h)
            ? fprintf(stdout, "%s:%s", filename, buffer_copy)
            : fprintf(stdout, "%s", buffer_copy);
      } else if (opts->n && !opts->c && !opts->o) {
        multifile
            ? fprintf(stdout, "%s:%d:%s", filename, line_number, buffer_copy)
            : fprintf(stdout, "%d:%s", line_number, buffer_copy);
      }
      // rm_so - начальная позиция совпадения
      // rm_eo - конечная позиция совпадения
      if (opts->o && !opts->c) {
        while (regexec(&regex, buffer + offset, MAX_MATCHES, regex_matches,
                       0) == 0) {
          for (int i = 0; i < MAX_MATCHES && regex_matches[i].rm_so != -1;
               i++) {
            if (multifile && !opts->h) {
              fprintf(stdout, "%s:", filename);
              if (opts->n) {
                fprintf(stdout, "%d:", line_number);
              }
            } else if (!multifile && opts->n) {
              fprintf(stdout, "%d:", line_number);
            }
            // buffer / size / count / stream
            fwrite(buffer + offset + regex_matches[i].rm_so, 1,
                   regex_matches[i].rm_eo - regex_matches[i].rm_so, stdout);
            fputc('\n', stdout);
          }
          // увеличение смещения
          offset += regex_matches[0].rm_eo;
        }
      }
    }
    line_number++;
  }

  regfree(&regex);

  if (opts->c && !opts->l) {
    multifile && (!opts->h)
        ? fprintf(stdout, "%s:%d\n", filename, matching_lines)
        : fprintf(stdout, "%d\n", matching_lines);
  }
}
