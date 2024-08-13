#include "cat.h"

void parse_options(int argc, char *argv[], options *opts) {
  struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                  {"number", 0, 0, 'n'},
                                  {"squeeze-blank", 0, 0, 's'},
                                  {NULL, 0, NULL, 0}};
  int opt;
  // opt = -1 (параметров нет)
  while ((opt = getopt_long(argc, argv, "+benstvET", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        opts->b = 1;
        break;
      case 'e':
        opts->e = 1;
        opts->v = 1;
        break;
      case 'n':
        opts->n = 1;
        break;
      case 's':
        opts->s = 1;
        break;
      case 't':
        opts->t = 1;
        opts->v = 1;
        break;
      case 'v':
        opts->v = 1;
        break;
      case 'E':
        opts->e = 1;
        break;
      case 'T':
        opts->t = 1;
        break;
      case '?':
      default:
        fprintf(stderr, "cat [OPTION] [FILE]\n");
        exit(EXIT_FAILURE);
    }
  }
}

void handle_flag_e(char *buffer) {
  char *pointer = buffer;
  while (*pointer != '\0') {
    if (*pointer == '\n') {
      if ((pointer - buffer) + 1 >= BUFFER_SIZE) {
        fprintf(stderr, "Error: buffer size\n");
        break;
      }
      // куда / откуда / сколько
      memmove(pointer + 1, pointer, strlen(pointer) + 1);
      *pointer = '$';
      pointer++;
    }
    pointer++;
  }
}

void handle_flag_t(char *buffer) {
  char *pointer = buffer;
  while (*pointer != '\0') {
    if (*pointer == '\t') {
      if ((pointer - buffer) + 2 >= BUFFER_SIZE) {
        fprintf(stderr, "error (-t buffer)\n");
        break;
      }
      memmove(pointer + 2, pointer + 1, strlen(pointer + 1) + 1);
      *pointer++ = '^';
      *pointer++ = 'I';
      continue;
    }
    pointer++;
  }
}

void handle_flag_v(char *buffer) {
  char *ptr = buffer;
  while (*ptr != '\0') {
    unsigned char byte = (unsigned char)(*ptr);
    if (byte >= 128 && byte < 160) {
      printf("M-^%c", byte - 64);
    } else if (byte >= 160) {
      printf("M-%c", byte - 128);
    } else {
      printf("%c", *ptr);
    }
    ptr++;
  }
}

void handle_file(options *opts, FILE *file, int *line_number,
                 int *previous_empty) {
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (opts->s) {
      if (buffer[0] == '\n' && (*previous_empty) == 1) continue;
      *previous_empty = (buffer[0] == '\n');
    }
    if (opts->b && buffer[0] != '\n') {
      printf("%6d\t", (*line_number)++);
    } else if (opts->n && (!opts->b || buffer[0] != '\n')) {
      printf("%6d\t", (*line_number)++);
    }
    if (opts->e) {
      handle_flag_e(buffer);
    }
    if (opts->t) {
      handle_flag_t(buffer);
    }
    if (opts->v) {
      handle_flag_v(buffer);
      continue;
    }
    printf("%s", buffer);
  }
}
