#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdarg.h>
#include <string.h>

#define GIF_BLOCK_TYPE_IMG 0x2c
#define GIF_BLOCK_TYPE_EXT 0x21

#define GIF_EXT_LABEL_GRAPH_CTRL 0xf9
#define GIF_EXT_LABEL_COMMENT 0xfe
#define GIF_EXT_LABEL_PLAIN_TEXT 0x01
#define GIF_EXT_LABEL_APP 0xff

#define GIF_TRAILER 0x3b
#define GIF_NULL_BYTE 0x00

#define GIF_LGTM_DATA_SIZE 27

struct gif_bytes {
  unsigned char *buf;
  int size;
  unsigned int idx;
  unsigned int image_block_idx_last;
};

struct gif_header {
  char signature[4];
  char version[4];
  unsigned int logical_screen_width;
  unsigned int logical_screen_height;
  unsigned char global_color_table_flag;
  unsigned char color_resolution;
  unsigned char sort_flag;
  unsigned int size_of_global_color_table;
  unsigned char background_color_index;
  unsigned char pixel_aspect_ratio;
  unsigned int *global_color_table;
};

struct gif_block_ext_gp_ctrl {
  unsigned int block_size;
  unsigned char reserved;
  unsigned char disposal_method;
  unsigned char user_input_flag;
  unsigned char transparent_color_flag;
  unsigned int delay_time;
  unsigned char transparent_color_index;
};

struct gif_block_ext_app {
  unsigned char block_size;
  char application_identifier[9];
  char application_authentication_code[4];
  unsigned int application_data[2];
  char read_flag;
};

struct gif_block_image {
  unsigned int image_left_position;
  unsigned int image_top_position;
  unsigned int image_witdh;
  unsigned int image_height;
  unsigned char local_color_table_flag;
  unsigned char interlace_flag;
  unsigned char sort_flag;
  unsigned char reserved;
  unsigned int size_of_local_color_table;
  unsigned int *local_color_table;
  unsigned char lzw_minimum_code_side;
};

struct gif_block_frame {
  struct gif_block_ext_gp_ctrl *ctrl;
  struct gif_block_image *img;
  struct gif_block_frame *next;
};

static void read_gif_data(FILE *fp, struct gif_bytes *bytesp);
static void read_gif_header(struct gif_bytes *bytesp, struct gif_header *hp);
static void read_gif_blocks(struct gif_bytes *bytesp, struct gif_block_frame *framep, struct gif_block_ext_app *appp);

static struct gif_block_frame *read_gif_block_img(struct gif_bytes *bytesp, struct gif_block_frame *framep);
static struct gif_block_frame *read_gif_block_ext(struct gif_bytes *bytesp, struct gif_block_frame *framep, struct gif_block_ext_app *appp);
static struct gif_block_frame *read_gif_block_ext_graph_ctrl(struct gif_bytes *bytesp, struct gif_block_frame *framep);
static void read_gif_block_ext_app(struct gif_bytes *bytesp, struct gif_block_ext_app *appp);
static void read_gif_block_ext_comment(struct gif_bytes *bytesp);
static void read_gif_block_ext_plain_text(struct gif_bytes *bytesp);

static void append_lgtm_bytes(struct gif_bytes *bytesp, struct gif_header *hp);

static void write_gif_data(FILE *fp, const struct gif_bytes *bytesp);
static void write_gif_header(FILE *fp, const struct gif_header *hp);
static void write_gif_ext_app(FILE *fp, const struct gif_block_ext_app *appp);
static void write_gif_blocks(FILE *fp, const struct gif_block_frame *framep);

static struct gif_block_frame *add_frame(struct gif_block_frame *prev_framep);
static void dealloc_gif_header(struct gif_header *hp);
static void dealloc_gif_frames(struct gif_block_frame *frame);

static void die_err(const char *msg);
static void die(const char *fmt, ...);
static long calc_file_size(FILE *fp);
static uint32_t extract_data(const unsigned char *bytes, int n);
static void print_color_table(FILE *fp, unsigned int size, unsigned int *table, char *label);

int
main(int argc, char *argv[])
{
  FILE *fp;
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
  bytes.buf = malloc(bytes.size);
  if (bytes.buf == NULL) die("[ERROR] could not allocate memory for buffer of gif bytes");

  first_frame.ctrl = NULL;
  first_frame.img = NULL;
  first_frame.next = NULL;

  read_gif_data(fp, &bytes);
  read_gif_header(&bytes, &h);
  app.read_flag = 0;
  read_gif_blocks(&bytes, &first_frame, &app);

  append_lgtm_bytes(&bytes, &h);

  write_gif_header(stderr, &h);
  write_gif_ext_app(stderr, &app);
  write_gif_blocks(stderr, &first_frame);
  write_gif_data(stdout, &bytes);

  free(bytes.buf);
  dealloc_gif_header(&h);
  dealloc_gif_frames(&first_frame);

  exit(0);
}

static void
read_gif_data(FILE *fp, struct gif_bytes *bytesp)
{
  long n;

  n = fread(bytesp->buf, sizeof(unsigned char), bytesp->size, fp);
  if (n != bytesp->size) die("[ERROR] failed to read from file");
}

static void
read_gif_header(struct gif_bytes *bytesp, struct gif_header *hp)
{
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

  if (!strcmp(hp->signature, "89a")) die("[ERROR] not supported gif version: %s", hp->signature);

  hp->logical_screen_width = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  hp->logical_screen_height = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;

  bits = bytesp->buf[bytesp->idx++];

  hp->global_color_table_flag = ((bits & (1 << 7)) >> 7);
  hp->color_resolution = (bits << 1 >> 5) + 1;
  hp->sort_flag = ((bits & (1 << 3)) >> 3);
  hp->size_of_global_color_table = 1 << ((bits & ((1 << 2) | (1 << 1) | 1)) + 1);

  hp->background_color_index = bytesp->buf[bytesp->idx++];

  hp->pixel_aspect_ratio = bytesp->buf[bytesp->idx++];

  if (hp->global_color_table_flag) {
    hp->global_color_table = malloc(hp->size_of_global_color_table);
    if (hp->global_color_table == NULL) die("[ERROR] could not allocate memory for global color table of gif header");
    for (i = 0; i < hp->size_of_global_color_table; ++i) {
      hp->global_color_table[i] = extract_data(&bytesp->buf[bytesp->idx], 3);
      bytesp->idx += 3;
    }
  }
}

static void
read_gif_blocks(struct gif_bytes *bytesp, struct gif_block_frame *framep, struct gif_block_ext_app *appp)
{
  unsigned char c;

  while (bytesp->idx < bytesp->size) {
    c = bytesp->buf[bytesp->idx++];
    switch (c) {
      case GIF_BLOCK_TYPE_IMG:
        framep = read_gif_block_img(bytesp, framep);
        break;
      case GIF_BLOCK_TYPE_EXT:
        framep = read_gif_block_ext(bytesp, framep, appp);
        break;
      case GIF_NULL_BYTE:
        fprintf(stderr, "[WARN] encountered null byte\n");
        break;
      case GIF_TRAILER:
        fprintf(stderr, "[INFO] reached to trailer\n");
        break;
      default:
        die("[ERROR] unknown block type: %d", c);
        break;
    }
  }
}

struct gif_block_frame *
read_gif_block_img(struct gif_bytes *bytesp, struct gif_block_frame *framep)
{
  unsigned char bits;
  int i;
  unsigned char block_size;

  if (framep->img != NULL) framep = add_frame(framep);
  framep->img = malloc(sizeof(struct gif_block_image));

  framep->img->image_left_position = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  framep->img->image_top_position = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  framep->img->image_witdh = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  framep->img->image_height = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;

  bits = bytesp->buf[bytesp->idx++];
  framep->img->local_color_table_flag = (bits >> 7);
  framep->img->interlace_flag = (bits & (1 << 6) >> 6);
  framep->img->sort_flag = (bits & (1 << 5) >> 5);
  framep->img->reserved = ((bits & (1 << 4)) | (bits & (1 << 3)) >> 3);
  framep->img->size_of_local_color_table = (1 << ((bits << 5 >> 5) + 1));

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

  bytesp->image_block_idx_last = bytesp->idx - 1;

  return framep;
}

struct gif_block_frame *
read_gif_block_ext(struct gif_bytes *bytesp, struct gif_block_frame *framep, struct gif_block_ext_app *appp)
{
    switch (bytesp->buf[bytesp->idx++]) {
      case GIF_EXT_LABEL_GRAPH_CTRL:
        framep = read_gif_block_ext_graph_ctrl(bytesp, framep);
        break;
      case GIF_EXT_LABEL_APP:
        read_gif_block_ext_app(bytesp, appp);
        break;
      case GIF_EXT_LABEL_COMMENT:
        read_gif_block_ext_comment(bytesp);
        break;
      case GIF_EXT_LABEL_PLAIN_TEXT:
        read_gif_block_ext_plain_text(bytesp);
        break;
      default:
        die("[ERROR] unknown extension block label");
        break;
    }

    return framep;
}

struct gif_block_frame *
read_gif_block_ext_graph_ctrl(struct gif_bytes *bytesp, struct gif_block_frame *framep)
{
  unsigned char bits;

  if (framep->ctrl != NULL) framep = add_frame(framep);
  framep->ctrl = malloc(sizeof(struct gif_block_ext_gp_ctrl));

  framep->ctrl->block_size = bytesp->buf[bytesp->idx++];

  bits = bytesp->buf[bytesp->idx++];
  framep->ctrl->reserved = (bits >> 5);
  framep->ctrl->disposal_method = (bits & ((1 << 4) | (1 << 3) | (1 << 2)) >> 2);
  framep->ctrl->user_input_flag = ((bits & (1 << 1)) >> 1);
  framep->ctrl->transparent_color_flag = (bits & 1);

  framep->ctrl->delay_time = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;

  framep->ctrl->transparent_color_index = bytesp->buf[bytesp->idx++];

  if (bytesp->buf[bytesp->idx] != '\0') die("[ERROR] failed to read from gif graphic control extension block data");

  return framep;
}

static void
read_gif_block_ext_app(struct gif_bytes *bytesp, struct gif_block_ext_app *appp)
{
  int i;
  unsigned char block_size;

  appp->block_size = bytesp->buf[bytesp->idx++];

  for (i = 0; i < 8; ++i) {
    appp->application_identifier[i] = bytesp->buf[bytesp->idx++];
  }
  appp->application_identifier[8] = '\0';

  for (i = 0; i < 3; ++i) {
    appp->application_authentication_code[i] = bytesp->buf[bytesp->idx++];
  }
  appp->application_authentication_code[3] = '\0';

  block_size = bytesp->buf[bytesp->idx++];
  if (block_size != 3) die("[ERROR] not supported block size of application extension");

  appp->application_data[0] = bytesp->buf[bytesp->idx++];
  appp->application_data[1] = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;

  if (bytesp->buf[bytesp->idx] != '\0') die("[ERROR] failed to read from gif application extension block data");

  appp->read_flag = 1;
}

static void
read_gif_block_ext_comment(struct gif_bytes *bytesp)
{
  die("[ERROR] comment extension block is not implemented yet");
}

static void
read_gif_block_ext_plain_text(struct gif_bytes *bytesp)
{
  die("[ERROR] plain text extension block is not implemented yet");
}

static void
append_lgtm_bytes(struct gif_bytes *bytesp, struct gif_header *hp)
{
  unsigned char *after_data_buf;
  unsigned int after_data_size;
  unsigned int total_size;
  unsigned char lgtm_buf[GIF_LGTM_DATA_SIZE];
  unsigned char *p;
  unsigned int i;
  unsigned int j;
  unsigned int idx;

  after_data_size = bytesp->size - bytesp->image_block_idx_last;
  after_data_buf = (unsigned char *) malloc(after_data_size);
  if (after_data_buf == NULL) die("[ERROR] could not allocate memory for lgtm bytes");
  memcpy(after_data_buf, &bytesp->buf[bytesp->image_block_idx_last + 1], after_data_size);

  total_size = bytesp->size + GIF_LGTM_DATA_SIZE;
  p = (unsigned char *) realloc(bytesp->buf, total_size);
  if (p == NULL) {
    free(bytesp->buf);
    die("[ERROR] could not reallocate memory for lgtm bytes");
  }
  bytesp->buf = p;
  bytesp->size = total_size;

  i = 0;
  lgtm_buf[i++] = GIF_BLOCK_TYPE_EXT;
  lgtm_buf[i++] = GIF_EXT_LABEL_GRAPH_CTRL;
  lgtm_buf[i++] = 4;                       // Block Size
  lgtm_buf[i++] = 1 << 3;                  // Reserved | Disposal Method | User Input Flag | Transparent Color Flag
  lgtm_buf[i++] = (255 & 0x000000ff);      // Delay Time
  lgtm_buf[i++] = (255 & 0x0000ff00) >> 8; // Delay Time
  lgtm_buf[i++] = 0;                       // Transparent Color Index
  lgtm_buf[i++] = 0;                       // Block Terminator

  lgtm_buf[i++] = GIF_BLOCK_TYPE_IMG;
  lgtm_buf[i++] = 0; // Image Left Position
  lgtm_buf[i++] = 0; // Image Left Position
  lgtm_buf[i++] = 0; // Image Top Position
  lgtm_buf[i++] = 0; // Image Top Position
  lgtm_buf[i++] = (hp->logical_screen_width & 0x000000ff);       // Image Width
  lgtm_buf[i++] = (hp->logical_screen_width & 0x0000ff00) >> 8;  // Image Width
  lgtm_buf[i++] = (hp->logical_screen_height & 0x000000ff);      // Image Height
  lgtm_buf[i++] = (hp->logical_screen_height & 0x0000ff00) >> 8; // Image Height
  lgtm_buf[i++] = (1 << 7); // Local Color Table Flag | Interlace Flag | Sort Flag | Reserved | Size of Local Color Table
  lgtm_buf[i++] = 250; // Local Color Table[0]
  lgtm_buf[i++] = 250; // Local Color Table[0]
  lgtm_buf[i++] = 250; // Local Color Table[0]
  lgtm_buf[i++] = 10; // Local Color Table[1]
  lgtm_buf[i++] = 10; // Local Color Table[1]
  lgtm_buf[i++] = 10; // Local Color Table[1]
  lgtm_buf[i++] = 8; // LZW Minimum Code Size
  lgtm_buf[i++] = 0; // Block Size
  lgtm_buf[i++] = 0; // Block Terminator

  idx = bytesp->image_block_idx_last + 1;
  for (j = 0; j < GIF_LGTM_DATA_SIZE; ++j) {
    bytesp->buf[idx++] = lgtm_buf[j];
  }
  for (i = 0; i < after_data_size; ++i) {
    bytesp->buf[idx++] = after_data_buf[i];
  }
  free(after_data_buf);
}

static void
write_gif_header(FILE *fp, const struct gif_header *hp)
{
  fprintf(fp, "Headers\n");
  fprintf(fp, "  Signature: %s\n", hp->signature);
  fprintf(fp, "  Version: %s\n", hp->version);
  fprintf(fp, "  Logical Screen Width: %u\n", hp->logical_screen_width);
  fprintf(fp, "  Logical Screen Height: %u\n", hp->logical_screen_height);
  fprintf(fp, "  Global Color Table Flag: %d\n", hp->global_color_table_flag);
  fprintf(fp, "  Color Resolution: %d\n", hp->color_resolution);
  fprintf(fp, "  Sort Flag: %d\n", hp->sort_flag);
  fprintf(fp, "  Size of Global Color Table: %d\n", hp->size_of_global_color_table);
  fprintf(fp, "  Background Color Index: %d\n", hp->background_color_index);
  fprintf(fp, "  Pixel Aspect Ratio: %d\n", hp->pixel_aspect_ratio);

  if (hp->global_color_table_flag) {
    print_color_table(fp, hp->size_of_global_color_table, hp->global_color_table, "Global Color Table");
  }
}

static void
write_gif_ext_app(FILE *fp, const struct gif_block_ext_app *appp)
{
  if (!appp->read_flag) return;
  fprintf(fp, "Application Extension:\n");
  fprintf(fp, "  Block Size: %d\n", appp->block_size);
  fprintf(fp, "  Application Identifier: %s\n", appp->application_identifier);
  fprintf(fp, "  Application Authentication Code: %s\n", appp->application_authentication_code);
  fprintf(fp, "  Application Data1: %d\n", appp->application_data[0]);
  fprintf(fp, "  Application Data2: %d\n", appp->application_data[1]);
}

static void
write_gif_blocks(FILE *fp, const struct gif_block_frame *framep)
{
  int i;

  for (i = 1; framep != NULL; framep = framep->next, ++i) {
    fprintf(fp, "Blocks[%d]:\n", i);
    fprintf(fp, "  Block Size: %d\n", framep->ctrl->block_size);
    fprintf(fp, "  Reserved: %d\n", framep->ctrl->reserved);
    fprintf(fp, "  Disposal Method: %d\n", framep->ctrl->disposal_method);
    fprintf(fp, "  User Input Flag: %d\n", framep->ctrl->user_input_flag);
    fprintf(fp, "  Transparent Color Flag: %d\n", framep->ctrl->transparent_color_flag);
    fprintf(fp, "  Delay Time: %d\n", framep->ctrl->delay_time);
    fprintf(fp, "  Transparent Color Index: %d\n", framep->ctrl->transparent_color_index);
    fprintf(fp, "  Image Left Position: %d\n", framep->img->image_left_position);
    fprintf(fp, "  Image Top Position: %d\n", framep->img->image_top_position);
    fprintf(fp, "  Image Width: %d\n", framep->img->image_witdh);
    fprintf(fp, "  Image Height: %d\n", framep->img->image_height);
    fprintf(fp, "  Local Color Table Flag: %d\n", framep->img->local_color_table_flag);
    fprintf(fp, "  Interlace Flag: %d\n", framep->img->interlace_flag);
    fprintf(fp, "  Sort Flag: %d\n", framep->img->sort_flag);
    fprintf(fp, "  Reserved: %d\n", framep->img->reserved);
    fprintf(fp, "  Size of Local Color Table: %d\n", framep->img->size_of_local_color_table);
    fprintf(fp, "  LZW Minimum Code Side: %d\n", framep->img->lzw_minimum_code_side);

    if (framep->img->local_color_table_flag) {
      print_color_table(fp, framep->img->size_of_local_color_table, framep->img->local_color_table, "Local Color Table");
    }
  }
}

static void
write_gif_data(FILE *fp, const struct gif_bytes *bytesp)
{
  long n;
  n = fwrite(bytesp->buf, sizeof(unsigned char), bytesp->size, fp);
  if (n != bytesp->size) die("[ERROR] failed to write of gif bytes");
}

static struct gif_block_frame *
add_frame(struct gif_block_frame *prev_framep)
{
  struct gif_block_frame *next_framep;

  next_framep = malloc(sizeof(struct gif_block_frame));
  if (next_framep == NULL) die("[ERROR] could not allocate memory for frame of gif block");

  next_framep->ctrl = NULL;
  next_framep->img = NULL;
  next_framep->next = NULL;
  prev_framep->next = next_framep;

  return next_framep;
}

static void
dealloc_gif_header(struct gif_header *hp)
{
  if (hp->global_color_table_flag) free(hp->global_color_table);
}

static void
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

static void
die_err(const char *msg)
{
  perror(msg);
  exit(1);
}

static void
die(const char *fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  vfprintf(stderr, fmt, list);
  fprintf(stderr, "\n");
  va_end(list);
  exit(1);
}

static long
calc_file_size(FILE *fp)
{
  long size;

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);
  return size;
}

static uint32_t
extract_data(const unsigned char *bytes, int n)
{
  uint32_t ret;
  int i;

  ret = 0;

  for (i = 0; i < n; ++i) {
    ret |= (bytes[i] << (8 * i));
  }

  return ret;
}

static void
print_color_table(FILE *fp, unsigned int size, unsigned int *table, char *label)
{
  uint32_t color;
  unsigned int r;
  unsigned int g;
  unsigned int b;
  unsigned int i;

  fprintf(fp, "  %s: ", label);
  for (i = 0; i < size; ++i) {
    if (i > 0) fprintf(fp, ",");
    color = table[i];
    r = (color & 0x00ff0000) >> 16;
    g = (color & 0x0000ff00) >> 8;
    b = (color & 0x000000ff);
    fprintf(fp, "(%d,%d,%d)", r, g, b);
  }
  fprintf(fp, "\n");
}
