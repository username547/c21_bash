#ifndef GREP_H
#define GREP_H

#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10240
#define MAX_MATCHES 10240

typedef struct struct_options {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
} options;

void parse_command_line(int argc, char *argv[], options *opts);
void process_file(options *opts, FILE *file, char *filename, char *pattern,
                  int multifile);

#endif
