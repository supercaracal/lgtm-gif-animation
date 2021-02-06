#ifndef DATA_H_
#define DATA_H_

#include <stdio.h>
#include "./type.h"
#include "./tool.h"

void read_gif_data(FILE *, struct gif_bytes *);
void write_gif_data(FILE *, const struct gif_bytes *);

#endif  // DATA_H_
