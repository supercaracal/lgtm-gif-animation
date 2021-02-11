#include "header.h"

void read_gif_header(struct gif_bytes *bytesp, struct gif_header *hp) {
  int i;
  unsigned char bits;

  for (i = 0; i < 3; ++i) {
    hp->signature[i] = bytesp->buf[bytesp->idx++];
  }
  hp->signature[3] = '\0';

  if (strcmp(hp->signature, "GIF")) die("[ERROR] not supported file");

  for (i = 0; i < 3; ++i) {
    hp->version[i] = bytesp->buf[bytesp->idx++];
  }
  hp->version[3] = '\0';

  if (!strcmp(hp->signature, "89a")) {
    die("[ERROR] not supported gif version: %s", hp->signature);
  }

  hp->logical_screen_width = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  if (hp->logical_screen_width < GIF_LGTM_IMG_WIDTH) {
    die("[ERROR] not supported size of width (>= %d)", GIF_LGTM_IMG_WIDTH);
  }

  hp->logical_screen_height = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  if (hp->logical_screen_height < GIF_LGTM_IMG_HEIGHT) {
    die("[ERROR] not supported size of height (>= %d)", GIF_LGTM_IMG_HEIGHT);
  }

  bits = bytesp->buf[bytesp->idx++];

  hp->global_color_table_flag = (bits & (1 << 7)) >> 7;
  hp->color_resolution = ((bits & ((1 << 6) | (1 << 5) | (1 << 4))) >> 4) + 1;
  hp->sort_flag = (bits & (1 << 3)) >> 3;
  hp->size_of_global_color_table = 1 << ((bits & ((1 << 2) | (1 << 1) | 1)) + 1);

  hp->background_color_index = bytesp->buf[bytesp->idx++];

  hp->pixel_aspect_ratio = bytesp->buf[bytesp->idx++];

  if (hp->global_color_table_flag) {
    hp->global_color_table = (unsigned int *) malloc(hp->size_of_global_color_table);
    if (hp->global_color_table == NULL) {
      die("[ERROR] could not allocate memory for global color table of gif header");
    }
    for (i = 0; i < hp->size_of_global_color_table; ++i) {
      hp->global_color_table[i] = extract_data(&bytesp->buf[bytesp->idx], 3);
      bytesp->idx += 3;
    }
  }
}

void write_gif_header(FILE *fp, const struct gif_header *hp) {
  fprintf(fp, "Headers\n");
  fprintf(fp, "  Signature: %s\n", hp->signature);
  fprintf(fp, "  Version: %s\n", hp->version);
  fprintf(fp, "  Logical Screen Width: %u\n", hp->logical_screen_width);
  fprintf(fp, "  Logical Screen Height: %u\n", hp->logical_screen_height);
  fprintf(fp, "  Global Color Table Flag: %u\n", hp->global_color_table_flag);
  fprintf(fp, "  Color Resolution: %u\n", hp->color_resolution);
  fprintf(fp, "  Sort Flag: %u\n", hp->sort_flag);
  fprintf(fp, "  Size of Global Color Table: %u\n", hp->size_of_global_color_table);
  fprintf(fp, "  Background Color Index: %u\n", hp->background_color_index);
  fprintf(fp, "  Pixel Aspect Ratio: %u\n", hp->pixel_aspect_ratio);

  if (hp->global_color_table_flag) {
    print_color_table(fp,
                      hp->size_of_global_color_table,
                      hp->global_color_table,
                      "Global Color Table");
  }
}

void dealloc_gif_header(struct gif_header *hp) {
  if (hp->global_color_table_flag) free(hp->global_color_table);
}
