#ifndef DATA_H_
#define DATA_H_

#include <stdio.h>
#include "./type.h"
#include "./tool.h"

void read_gif_data(FILE *fp, struct gif_bytes *bytesp);
void write_gif_data(FILE *fp, const struct gif_bytes *bytesp);

#endif // DATA_H_
