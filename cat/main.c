#include "cat.h"

int main(int argc, char *argv[]) {
  options opts = {0};
  int line_number = 1;
  int previous_empty = 0;

  if (argc < 2) {
    fprintf(stderr, "Error: incorrect program usage\n");
    return 1;
  }

  parse_options(argc, argv, &opts);

  // optind - индекс первого необработанного аргумента
  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL) {
      fprintf(stderr, "Error: cannot open file %s", argv[i]);
      continue;
    }
    handle_file(&opts, file, &line_number, &previous_empty);

    fclose(file);
  }
  return 0;
}
