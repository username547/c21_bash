#include "grep.h"

int main(int argc, char *argv[]) {
  options opts = {0};

  if (argc < 3) {
    fprintf(stderr, "grep [OPTION] [TEMPLATE] [FILE]...\n");
    return 1;
  }

  parse_command_line(argc, argv, &opts);
  char *pattern = argv[optind++];

  int multifile = (argc - optind > 1) ? 1 : 0;

  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL) {
      if (!opts.s) {
        fprintf(stderr, "Error: cannot open file %s\n", argv[i]);
      }
      continue;
    }
    process_file(&opts, file, argv[i], pattern, multifile);
    fclose(file);
  }

  return 0;
}

// multifile
// grep -e hello 1.txt 2.txt -> argc = 5 && optind = 3 | argc - optind = 2
// grep hello 1.txt 2.txt -> argc = 4 && optind = 2 | argc - optind = 2

// single file
// grep -e hello 1.txt -> argc = 4 && optind = 3 | argc - optind = 1
// grep hello 1.txt -> argc = 3 && optind = 2 | argc - optind = 1
