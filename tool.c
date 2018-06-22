#include "tool.h"

#define ANSI_ESC 0x1b
#define ANSI_ESC_SEQ_BG_FMT "%c[48;5;%dm %c[0m"
#define ANSI_COLOR_DEGREE(rgb) ((rgb * 10 - 1) / 425)
#define ANSI_COLOR_NUMBER(r, g, b) (16 + 36 * r + 6 * g + b)

static int
convert_rgb_into_ansi_color(int r, int g, int b);

void
die_err(const char *msg)
{
  perror(msg);
  exit(1);
}

void
die(const char *fmt, ...)
{
  va_list list;

  va_start(list, fmt);
  vfprintf(stderr, fmt, list);
  fprintf(stderr, "\n");
  va_end(list);

  exit(1);
}

int
calc_file_size(FILE *fp)
{
  int size;

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  return size;
}

uint32_t
extract_data(const unsigned char *bytes,
             int                 n)
{
  uint32_t ret;
  int i;

  for (i = 0, ret = 0; i < n; ++i) {
    ret |= (bytes[i] << (8 * i));
  }

  return ret;
}

void
print_color_table(FILE         *fp,
                  unsigned int size,
                  unsigned int *table,
                  char         *label)
{
  uint32_t     color;
  unsigned int r;
  unsigned int g;
  unsigned int b;
  unsigned int i;

  fprintf(fp, "  %s: ", label);
  for (i = 0; i < size; ++i) {
    // if (i > 0) fprintf(fp, ",");
    color = table[i];
    r = (color & 0x00ff0000) >> 16;
    g = (color & 0x0000ff00) >> 8;
    b = (color & 0x000000ff);
    // fprintf(fp, "#%02X%02X%02X", r, g, b);
    fprintf(fp,
            ANSI_ESC_SEQ_BG_FMT,
            ANSI_ESC,
            convert_rgb_into_ansi_color(r, g, b),
            ANSI_ESC);
  }
  fprintf(fp, "\n");
}

static int
convert_rgb_into_ansi_color(int r, int g, int b)
{
  return ANSI_COLOR_NUMBER(ANSI_COLOR_DEGREE(r),
                           ANSI_COLOR_DEGREE(g),
                           ANSI_COLOR_DEGREE(b));
}
