#include <stdlib.h>
#include "block_frame.h"
#include "tool.h"

GIFBlockFrame *add_frame(GIFBlockFrame *prev_framep) {
  GIFBlockFrame *next_framep;

  next_framep = (GIFBlockFrame *) malloc(sizeof(GIFBlockFrame));
  if (next_framep == NULL) {
    die("[ERROR] could not allocate memory for frame of gif frames");
  }

  next_framep->ctrl = NULL;
  next_framep->img = NULL;
  next_framep->next = NULL;
  prev_framep->next = next_framep;

  return next_framep;
}

void free_gif_frames(GIFBlockFrame *framep) {
  GIFBlockFrame *fp, *nfp;

  fp = framep->next;
  if (fp == NULL) return;
  while ((nfp = fp->next) != NULL) {
    free(fp->ctrl);
    if (fp->img->local_color_table_flag) free(fp->img->local_color_table);
    free(fp->img->lzw_data.buf);
    free(fp->img->raw_data.buf);
    free(fp->img);
    free(fp);
    fp = nfp;
  }
}
