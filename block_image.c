#include <stdlib.h>
#include "block_image.h"
#include "tool.h"
#include "block_frame.h"

void concat_img_block(struct gif_bytes *, struct gif_block_image *, int);

struct gif_block_frame *read_gif_block_img(struct gif_bytes *bytesp, struct gif_block_frame *framep) {
  unsigned char bits;
  int i, block_size;

  if (framep->img != NULL) framep = add_frame(framep);
  framep->img = (struct gif_block_image *) malloc(sizeof(struct gif_block_image));
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

  framep->img->data_size = 0;
  framep->img->data_idx = 0;
  framep->img->data = NULL;
  while ((block_size = bytesp->buf[bytesp->idx++]) != 0) {
    concat_img_block(bytesp, framep->img, block_size);
  }

  return framep;
}

void concat_img_block(struct gif_bytes *b, struct gif_block_image *i, int size) {
  void *p;

  i->data_size += size;

  if (i->data == NULL) {
    p = malloc(sizeof(unsigned char) * i->data_size);
  } else {
    p = realloc(i->data, sizeof(unsigned char) * i->data_size);
  }

  if (p == NULL) die("[ERROR] could not allocate memory for lzw data");
  i->data = (unsigned char *) p;

  while (size--) i->data[i->data_idx++] = b->buf[b->idx++];
}
