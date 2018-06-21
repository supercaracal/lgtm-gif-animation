#ifndef TOOL_H_
#define TOOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void
die_err(const char *msg);

void
die(const char *fmt, ...);

int
calc_file_size(FILE *fp);

uint32_t
extract_data(const unsigned char *bytes,
             int                 n);

void
print_color_table(FILE         *fp,
                  unsigned int size,
                  unsigned int *table,
                  char         *label);

#endif // TOOL_H_
