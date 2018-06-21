#include "block_frame.h"

struct gif_block_frame *
add_frame(struct gif_block_frame *prev_framep)
{
  struct gif_block_frame *next_framep;

  next_framep = malloc(sizeof(struct gif_block_frame));
  if (next_framep == NULL) die("[ERROR] could not allocate memory for frame of gif frames");

  next_framep->ctrl = NULL;
  next_framep->img = NULL;
  next_framep->next = NULL;
  prev_framep->next = next_framep;

  return next_framep;
}

void
dealloc_gif_frames(struct gif_block_frame *framep)
{
  struct gif_block_frame *fp;
  struct gif_block_frame *nfp;

  fp = framep->next;
  if (fp == NULL) return;
  while ((nfp = fp->next) != NULL) {
    free(fp->ctrl);
    if (fp->img->local_color_table_flag) free(fp->img->local_color_table);
    free(fp->img);
    free(fp);
    fp = nfp;
  }
}
