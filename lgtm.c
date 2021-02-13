#include <stdlib.h>
#include "lgtm.h"
#include "constant.h"
#include "tool.h"

void append_lgtm_bytes(BinData *bytesp, GIFHeader *hp) {
  unsigned int total_size, i, left, top;
  void *p;
  unsigned char lgtm_colors[GIF_LGTM_DATA_COLORS_SIZE] = GIF_LGTM_DATA_COLORS;
  unsigned char lgtm_blocks[GIF_LGTM_DATA_BLOCKS_SIZE] = GIF_LGTM_DATA_BLOCKS;

  total_size = bytesp->size + GIF_LGTM_DATA_SIZE;
  p = realloc(bytesp->buf, sizeof(unsigned char) * total_size);
  if (p == NULL) {
    free(bytesp->buf);
    die("[ERROR] could not reallocate memory for lgtm bytes");
  }
  bytesp->buf = (unsigned char *) p;
  bytesp->size = total_size;
  if (bytesp->buf[bytesp->idx - 1] != GIF_TRAILER) {
    die("[FATAL] something wrong with gif data or this program");
  }
  bytesp->idx--;

  bytesp->buf[bytesp->idx++] = GIF_BLOCK_TYPE_EXT;
  bytesp->buf[bytesp->idx++] = GIF_EXT_LABEL_GRAPH_CTRL;
  bytesp->buf[bytesp->idx++] = 4;                                                     // Block Size
  bytesp->buf[bytesp->idx++] = (1 << 3) | 1;                                          // Reserved | Disposal Method | User Input Flag | Transparent Color Flag
  bytesp->buf[bytesp->idx++] = (GIF_LGTM_IMG_DISPLAY_SEC * 100) & 0x000000ff;         // Delay Time
  bytesp->buf[bytesp->idx++] = ((GIF_LGTM_IMG_DISPLAY_SEC * 100) & 0x0000ff00) >> 8;  // Delay Time
  bytesp->buf[bytesp->idx++] = 252;                                                   // Transparent Color Index
  bytesp->buf[bytesp->idx++] = 0;                                                     // Block Terminator

  left = (hp->logical_screen_width / 2) - (GIF_LGTM_IMG_WIDTH / 2);
  top = (hp->logical_screen_height / 2) - (GIF_LGTM_IMG_HEIGHT / 2);

  bytesp->buf[bytesp->idx++] = GIF_BLOCK_TYPE_IMG;
  bytesp->buf[bytesp->idx++] = left & 0x000000ff;                        // Image Left Position
  bytesp->buf[bytesp->idx++] = (left & 0x0000ff00) >> 8;                 // Image Left Position
  bytesp->buf[bytesp->idx++] = top & 0x000000ff;                         // Image Top Position
  bytesp->buf[bytesp->idx++] = (top & 0x0000ff00) >> 8;                  // Image Top Position
  bytesp->buf[bytesp->idx++] = GIF_LGTM_IMG_WIDTH & 0x000000ff;          // Image Width
  bytesp->buf[bytesp->idx++] = (GIF_LGTM_IMG_WIDTH & 0x0000ff00) >> 8;   // Image Width
  bytesp->buf[bytesp->idx++] = GIF_LGTM_IMG_HEIGHT & 0x000000ff;         // Image Height
  bytesp->buf[bytesp->idx++] = (GIF_LGTM_IMG_HEIGHT & 0x0000ff00) >> 8;  // Image Height
  bytesp->buf[bytesp->idx++] = (1 << 7) | (1 << 2) | (1 << 1) | 1;       // Local Color Table Flag | Interlace Flag | Sort Flag | Reserved | Size of Local Color Table
  for (i = 0; i < GIF_LGTM_DATA_COLORS_SIZE; ++i) {
    bytesp->buf[bytesp->idx++] = lgtm_colors[i];                         // Local Color Table
  }
  bytesp->buf[bytesp->idx++] = 8;                                        // LZW Minimum Code Size
  for (i = 0; i < GIF_LGTM_DATA_BLOCKS_SIZE; ++i) {
    bytesp->buf[bytesp->idx++] = lgtm_blocks[i];                         // Block Data with Terminator
  }

  bytesp->buf[bytesp->idx++] = GIF_TRAILER;
}
