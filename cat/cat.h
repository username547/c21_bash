#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10240

typedef struct struct_options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} options;

void parse_options(int argc, char *argv[], options *opts);
void handle_flag_e(char *buffer);
void handle_flag_t(char *buffer);
void handle_flag_v(char *buffer);
void handle_file(options *opts, FILE *file, int *line_number,
                 int *previous_empty);

#endif