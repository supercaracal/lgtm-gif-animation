#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./type.h"
#include "./tool.h"
#include "./header.h"
#include "./block.h"
#include "./lgtm.h"
#include "./block_ext_app.h"

int main(int argc, char **argv) {
  FILE *fp;
  int n;
  struct gif_bytes bytes;
  struct gif_header h;
  struct gif_block_frame first_frame;
  struct gif_block_ext_app app;

  if (argc == 1) {
    fp = stdin;
  } else {
    fp = fopen(argv[1], "r");
    if (fp == NULL) die_err(argv[1]);
  }

  bytes.idx = 0;
  bytes.size = calc_file_size(fp);
  bytes.buf = (unsigned char *) malloc(bytes.size);
  if (bytes.buf == NULL) die("[ERROR] could not allocate memory for buffer of gif bytes");
  n = fread(bytes.buf, sizeof(unsigned char), bytes.size, fp);
  if (n != bytes.size) die("[ERROR] failed to read from file");
  n = fclose(fp);
  if (n == EOF) die_err("[ERROR] failed to close file");
  fp = NULL;

  h.global_color_table = NULL;
  first_frame.ctrl = NULL;
  first_frame.img = NULL;
  first_frame.next = NULL;
  app.read_flag = 0;
  read_gif_header(&bytes, &h);
  read_gif_blocks(&bytes, &first_frame, &app);
  append_lgtm_bytes(&bytes, &h);

#ifdef DEBUG
  write_gif_header(stderr, &h);
  write_gif_ext_app(stderr, &app);
  write_gif_blocks(stderr, &first_frame);
#endif
  n = fwrite(bytes.buf, sizeof(unsigned char), bytes.size, stdout);
  if (n != bytes.size) die("[ERROR] failed to write of gif bytes");

  free(bytes.buf);
  dealloc_gif_header(&h);
  dealloc_gif_frames(&first_frame);

  exit(0);
}
