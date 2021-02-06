#ifndef TOOL_H_
#define TOOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdarg.h>

void die_err(const char *);
void die(const char *, ...);
int calc_file_size(FILE *);
uint32_t extract_data(const unsigned char *, int n);
void print_color_table(FILE *, unsigned int size, unsigned int *, char *);

#endif  // TOOL_H_
