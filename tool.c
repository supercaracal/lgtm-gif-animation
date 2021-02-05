#include "tool.h"

#define ANSI_ESC 0x1b
#define ANSI_ESC_SEQ_BG_FMT "%c[48;2;%d;%d;%dm"
#define ANSI_ESC_SEQ_FG_FMT "%c[38;2;%d;%d;%dm"
#define ANSI_ESC_SEQ_RESET_FMT "%c[0m"
#define RGB_HEX_FMT "#%02X%02X%02X"

static void print_color(FILE *fp, int r, int g, int b);

void die_err(const char *msg) {
  perror(msg);
  exit(1);
}

void die(const char *fmt, ...) {
  va_list list;

  va_start(list, fmt);
  vfprintf(stderr, fmt, list);
  fprintf(stderr, "\n");
  va_end(list);

  exit(1);
}

int calc_file_size(FILE *fp) {
  int size;

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  return size;
}

uint32_t extract_data(const unsigned char *bytes, int n) {
  uint32_t ret;
  int i;

  for (i = 0, ret = 0; i < n; ++i) {
    ret |= (bytes[i] << (8 * i));
  }

  return ret;
}

void print_color_table(FILE *fp, unsigned int size, unsigned int *table, char *label) {
  uint32_t color;
  unsigned int r;
  unsigned int g;
  unsigned int b;
  unsigned int i;

  fprintf(fp, "  %s:\n", label);
  for (i = 0; i < size; ++i) {
    color = table[i];
    r = (color & 0x00ff0000) >> 16;
    g = (color & 0x0000ff00) >> 8;
    b = (color & 0x000000ff);
    print_color(fp, r, g, b);
  }
  fprintf(fp, "\n");
}

static void print_color(FILE *fp, int r, int g, int b) {
  int fg = ((r + g + b) / 3) < 81 ? 255 : 0;

  fprintf(fp,
          ANSI_ESC_SEQ_BG_FMT ANSI_ESC_SEQ_FG_FMT RGB_HEX_FMT ANSI_ESC_SEQ_RESET_FMT,
          ANSI_ESC, r, g, b,
          ANSI_ESC, fg, fg, fg, r, g, b,
          ANSI_ESC);
}
