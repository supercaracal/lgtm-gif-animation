#include "block_ext_app.h"

void read_gif_block_ext_app(struct gif_bytes *bytesp, struct gif_block_ext_app *appp) {
  int i;
  int block_size;

  appp->block_size = bytesp->buf[bytesp->idx++];

  for (i = 0; i < 8; ++i) {
    appp->application_identifier[i] = bytesp->buf[bytesp->idx++];
  }
  appp->application_identifier[8] = '\0';

  for (i = 0; i < 3; ++i) {
    appp->application_authentication_code[i] = bytesp->buf[bytesp->idx++];
  }
  appp->application_authentication_code[3] = '\0';

  while ((block_size = bytesp->buf[bytesp->idx++]) != 0) {
    bytesp->idx += block_size;
  }

  if (bytesp->buf[bytesp->idx - 1] != '\0') {
    die("[ERROR] failed to read from gif application extension block data");
  }

  appp->read_flag = 1;
}

void write_gif_ext_app(FILE *fp, const struct gif_block_ext_app *appp) {
  if (!appp->read_flag) return;
  fprintf(fp, "Application Extension:\n");
  fprintf(fp, "  Block Size: %u\n", appp->block_size);
  fprintf(fp, "  Application Identifier: %s\n", appp->application_identifier);
  fprintf(fp, "  Application Authentication Code: %s\n", appp->application_authentication_code);
}
