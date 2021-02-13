#include <stdlib.h>
#include "block_image.h"
#include "tool.h"
#include "block_frame.h"

void extract_img_block(BinData *, BinData *, int);

GIFBlockFrame *read_gif_block_img(BinData *bytesp, GIFBlockFrame *framep) {
  unsigned char bits;
  int i, block_size;

  if (framep->img != NULL) framep = add_frame(framep);
  framep->img = (GIFBlockImage *) malloc(sizeof(GIFBlockImage));
  if (framep->img == NULL) {
    die("[ERROR] could not allocate memory for gif image block");
  }

  framep->img->image_left_position = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  framep->img->image_top_position = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  framep->img->image_witdh = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  framep->img->image_height = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;

  bits = bytesp->buf[bytesp->idx++];
  framep->img->local_color_table_flag = bits >> 7;
  framep->img->interlace_flag = (bits & (1 << 6)) >> 6;
  framep->img->sort_flag = (bits & (1 << 5)) >> 5;
  framep->img->reserved = ((bits & (1 << 4)) | (bits & (1 << 3))) >> 3;
  framep->img->size_of_local_color_table = 2 << (bits & ((1 << 2) | (1 << 1) | 1));

  if (framep->img->local_color_table_flag) {
    framep->img->local_color_table = (unsigned int *) malloc(sizeof(unsigned int) * framep->img->size_of_local_color_table);
    if (framep->img->local_color_table == NULL) {
      die("[ERROR] could not allocate memory for local color table of gif image block");
    }
    for (i = 0; i < framep->img->size_of_local_color_table; ++i) {
      framep->img->local_color_table[i] = extract_data(&bytesp->buf[bytesp->idx], 3);
      bytesp->idx += 3;
    }
  } else {
    framep->img->local_color_table = NULL;
  }

  framep->img->lzw_minimum_code_size = bytesp->buf[bytesp->idx++];

  framep->img->lzw_data.size = 0;
  framep->img->lzw_data.idx = 0;
  framep->img->lzw_data.buf = NULL;
  while ((block_size = bytesp->buf[bytesp->idx++]) != 0) {
    extract_img_block(bytesp, &framep->img->lzw_data, block_size);
  }

  return framep;
}

void extract_img_block(BinData *b, BinData *d, int size) {
  void *p;

  d->size += size;

  if (d->buf == NULL) {
    p = malloc(sizeof(unsigned char) * d->size);
  } else {
    p = realloc(d->buf, sizeof(unsigned char) * d->size);
  }

  if (p == NULL) die("[ERROR] could not allocate memory for lzw data");
  d->buf = (unsigned char *) p;

  while (size--) d->buf[d->idx++] = b->buf[b->idx++];
}
