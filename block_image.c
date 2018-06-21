#include "block_image.h"

struct gif_block_frame *
read_gif_block_img(struct gif_bytes       *bytesp,
                   struct gif_block_frame *framep)
{
  unsigned char bits;
  int           i;
  int           block_size;

  if (framep->img != NULL) framep = add_frame(framep);
  framep->img = malloc(sizeof(struct gif_block_image));
  if (framep->img == NULL) die("[ERROR] could not allocate memory for gif image block");

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
    framep->img->local_color_table = malloc(framep->img->size_of_local_color_table);
    if (framep->img->local_color_table == NULL) die("[ERROR] could not allocate memory for local color table of gif image block");
    for (i = 0; i < framep->img->size_of_local_color_table; ++i) {
      framep->img->local_color_table[i] = extract_data(&bytesp->buf[bytesp->idx], 3);
      bytesp->idx += 3;
    }
  }

  framep->img->lzw_minimum_code_side = bytesp->buf[bytesp->idx++];

  while ((block_size = bytesp->buf[bytesp->idx++]) != 0) {
    bytesp->idx += block_size;
  }

  return framep;
}
