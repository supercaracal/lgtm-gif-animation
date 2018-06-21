#include "data.h"

void
read_gif_data(FILE             *fp,
              struct gif_bytes *bytesp)
{
  int n;

  n = fread(bytesp->buf, sizeof(unsigned char), bytesp->size, fp);
  if (n != bytesp->size) die("[ERROR] failed to read from file");
}

void
write_gif_data(FILE                   *fp,
               const struct gif_bytes *bytesp)
{
  int n;
  n = fwrite(bytesp->buf, sizeof(unsigned char), bytesp->size, fp);
  if (n != bytesp->size) die("[ERROR] failed to write of gif bytes");
}
