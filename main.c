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

#define GIF_LGTM_DATA_COLORS {0,0,0,0,0,51,0,0,102,0,0,153,0,0,204,0,0,255,0,43,0,0,43,51,0,43,102,0,43,153,0,43,204,0,43,255,0,85,0,0,85,51,0,85,102,0,85,153,0,85,204,0,85,255,0,128,0,0,128,51,0,128,102,0,128,153,0,128,204,0,128,255,0,170,0,0,170,51,0,170,102,0,170,153,0,170,204,0,170,255,0,213,0,0,213,51,0,213,102,0,213,153,0,213,204,0,213,255,0,255,0,0,255,51,0,255,102,0,255,153,0,255,204,0,255,255,51,0,0,51,0,51,51,0,102,51,0,153,51,0,204,51,0,255,51,43,0,51,43,51,51,43,102,51,43,153,51,43,204,51,43,255,51,85,0,51,85,51,51,85,102,51,85,153,51,85,204,51,85,255,51,128,0,51,128,51,51,128,102,51,128,153,51,128,204,51,128,255,51,170,0,51,170,51,51,170,102,51,170,153,51,170,204,51,170,255,51,213,0,51,213,51,51,213,102,51,213,153,51,213,204,51,213,255,51,255,0,51,255,51,51,255,102,51,255,153,51,255,204,51,255,255,102,0,0,102,0,51,102,0,102,102,0,153,102,0,204,102,0,255,102,43,0,102,43,51,102,43,102,102,43,153,102,43,204,102,43,255,102,85,0,102,85,51,102,85,102,102,85,153,102,85,204,102,85,255,102,128,0,102,128,51,102,128,102,102,128,153,102,128,204,102,128,255,102,170,0,102,170,51,102,170,102,102,170,153,102,170,204,102,170,255,102,213,0,102,213,51,102,213,102,102,213,153,102,213,204,102,213,255,102,255,0,102,255,51,102,255,102,102,255,153,102,255,204,102,255,255,153,0,0,153,0,51,153,0,102,153,0,153,153,0,204,153,0,255,153,43,0,153,43,51,153,43,102,153,43,153,153,43,204,153,43,255,153,85,0,153,85,51,153,85,102,153,85,153,153,85,204,153,85,255,153,128,0,153,128,51,153,128,102,153,128,153,153,128,204,153,128,255,153,170,0,153,170,51,153,170,102,153,170,153,153,170,204,153,170,255,153,213,0,153,213,51,153,213,102,153,213,153,153,213,204,153,213,255,153,255,0,153,255,51,153,255,102,153,255,153,153,255,204,153,255,255,204,0,0,204,0,51,204,0,102,204,0,153,204,0,204,204,0,255,204,43,0,204,43,51,204,43,102,204,43,153,204,43,204,204,43,255,204,85,0,204,85,51,204,85,102,204,85,153,204,85,204,204,85,255,204,128,0,204,128,51,204,128,102,204,128,153,204,128,204,204,128,255,204,170,0,204,170,51,204,170,102,204,170,153,204,170,204,204,170,255,204,213,0,204,213,51,204,213,102,204,213,153,204,213,204,204,213,255,204,255,0,204,255,51,204,255,102,204,255,153,204,255,204,204,255,255,255,0,0,255,0,51,255,0,102,255,0,153,255,0,204,255,0,255,255,43,0,255,43,51,255,43,102,255,43,153,255,43,204,255,43,255,255,85,0,255,85,51,255,85,102,255,85,153,255,85,204,255,85,255,255,128,0,255,128,51,255,128,102,255,128,153,255,128,204,255,128,255,255,170,0,255,170,51,255,170,102,255,170,153,255,170,204,255,170,255,255,213,0,255,213,51,255,213,102,255,213,153,255,213,204,255,213,255,255,255,0,255,255,51,255,255,102,255,255,153,255,255,204,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0}
#define GIF_LGTM_DATA_BLOCKS {255,0,1,8,28,72,176,160,193,131,8,19,42,92,200,176,161,195,135,16,35,74,156,72,177,162,197,139,24,51,106,220,200,177,163,199,143,32,67,138,28,73,178,164,201,147,40,83,170,92,201,178,165,203,151,48,99,202,156,73,179,166,205,155,56,115,234,220,201,179,167,207,159,64,131,226,12,16,227,70,140,24,21,15,136,65,179,69,133,193,3,70,139,30,141,74,117,42,82,162,70,21,98,69,90,51,206,190,175,251,130,69,12,0,39,26,216,175,250,22,13,20,96,246,172,219,183,251,20,121,253,42,246,32,91,176,132,186,158,45,7,81,71,91,184,194,156,10,56,5,183,240,62,125,132,230,30,166,114,208,8,88,196,122,193,42,122,232,216,240,215,121,42,238,90,118,171,79,238,217,93,6,53,31,158,76,83,241,62,90,14,33,108,6,203,78,244,234,195,137,207,234,99,76,16,136,236,188,165,207,234,106,232,90,95,45,24,1,100,44,147,93,200,135,152,227,199,213,156,29,134,28,185,10,211,97,9,186,222,135,123,230,157,179,164,23,86,70,75,91,96,128,80,96,133,29,255,180,112,182,110,193,235,178,187,111,71,91,93,166,105,212,11,191,63,110,47,80,245,62,241,7,31,236,61,8,125,31,59,129,211,117,22,217,87,217,37,148,192,95,255,29,148,3,50,9,145,7,150,121,4,245,7,217,122,236,13,184,207,110,11,217,247,21,124,17,233,7,22,104,6,245,231,223,116,95,209,7,147,105,5,34,180,157,128,18,105,24,157,65,232,113,86,12,92,250,104,97,33,135,9,41,54,219,68,30,126,197,87,136,175,61,150,34,76,49,198,21,31,120,95,229,211,221,86,85,141,87,30,127,143,13,199,153,38,243,89,136,97,66,242,113,55,144,136,70,22,212,227,62,32,22,164,88,62,100,188,197,78,16,103,153,248,18,138,71,130,165,228,150,52,154,232,34,132,112,162,85,5,146,236,173,168,166,75,239,49,100,90,117,34,66,230,229,126,64,114,247,229,127,182,161,53,228,154,216,249,217,168,64,69,86,89,144,131,116,29,20,163,146,1,16,182,24,0,71,72,58,147,105,87,38,180,222,58,3,17,117,148,114,146,25,244,101,152,17,62,198,152,125,117,37,235,90,98,77,69,46,90,208,129,96,97,102,80,167,21,22,52,39,148,73,118,87,155,167,238,157,133,35,66,174,29,11,128,172,130,18,244,229,143,98,186,218,152,144,22,218,90,144,172,135,217,72,144,158,6,81,250,98,180,193,30,132,102,175,159,158,197,206,82,199,161,129,156,186,48,0,168,41,90,181,56,21,128,14,82,18,168,234,103,192,110,106,208,122,123,182,196,37,92,28,82,104,88,179,3,253,10,163,155,194,14,196,108,191,44,253,251,22,36,195,174,86,227,189,96,65,219,170,150,6,49,107,45,75,145,90,118,172,17,127,1,230,84,183,79,22,186,207,155,187,18,27,147,195,110,41,139,0,149,111,13,163,109,126,248,154,188,99,198,105,214,100,170,85,60,75,197,21,66,50,32,23,70,67,62,143,92,208,86,237,30,180,243,13,66,53,237,244,211,80,71,45,245,212,84,87,109,245,213,88,103,173,245,214,92,119,237,245,215,96,135,45,246,216,100,151,109,246,217,104,167,173,54,214,1,1,0}
#define GIF_LGTM_DATA_COLORS_SIZE 768
#define GIF_LGTM_DATA_BLOCKS_SIZE 749
#define GIF_LGTM_DATA_SIZE (768 + 749 + 8 + 11)
#define GIF_LGTM_IMG_MIN_WIDTH 128
#define GIF_LGTM_IMG_MIN_HEIGHT 64
#define GIF_LGTM_IMG_DELAY_TIME 200

struct gif_bytes {
  unsigned char *buf;
  int size;
  int idx;
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
static int calc_file_size(FILE *fp);
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
  int n;

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
  if (hp->logical_screen_width < GIF_LGTM_IMG_MIN_WIDTH) die("[ERROR] not supported size of width (>= %d)", GIF_LGTM_IMG_MIN_WIDTH);

  hp->logical_screen_height = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  if (hp->logical_screen_height < GIF_LGTM_IMG_MIN_HEIGHT) die("[ERROR] not supported size of height (>= %d)", GIF_LGTM_IMG_MIN_HEIGHT);

  bits = bytesp->buf[bytesp->idx++];

  hp->global_color_table_flag = (bits & (1 << 7)) >> 7;
  hp->color_resolution = ((bits & ((1 << 6) | (1 << 5) | (1 << 4))) >> 4) + 1;
  hp->sort_flag = (bits & (1 << 3)) >> 3;
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
  int trailer_exists;

  trailer_exists = 0;
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
        break;
      case GIF_TRAILER:
        trailer_exists = 1;
        break;
      default:
        die("[ERROR] unknown block type: %u", c);
        break;
    }
  }

  if (!trailer_exists) {
    die("[FATAL] trailer not found");
  }
}

struct gif_block_frame *
read_gif_block_img(struct gif_bytes *bytesp, struct gif_block_frame *framep)
{
  unsigned char bits;
  int i;
  int block_size;

  if (framep->img != NULL) framep = add_frame(framep);
  framep->img = malloc(sizeof(struct gif_block_image));
  if (framep->img == NULL) die("[ERROR] could not allocate memory for gif image block");

  framep->img->image_left_position = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  framep->img->image_top_position = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  framep->img->image_witdh = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;
  framep->img->image_height = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;

  bits = bytesp->buf[bytesp->idx++];
  framep->img->local_color_table_flag = bits >> 7;
  framep->img->interlace_flag = (bits & (1 << 6)) >> 6;
  framep->img->sort_flag = (bits & (1 << 5)) >> 5;
  framep->img->reserved = ((bits & (1 << 4)) | (bits & (1 << 3))) >> 3;
  framep->img->size_of_local_color_table = 2 << (bits & ((1 << 2) | (1 << 1) | 1));

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
  if (framep->ctrl == NULL) die("[ERROR] could not allocate memory for gif graphic control extension");

  framep->ctrl->block_size = bytesp->buf[bytesp->idx++];

  bits = bytesp->buf[bytesp->idx++];
  framep->ctrl->reserved = bits >> 5;
  framep->ctrl->disposal_method = (bits & ((1 << 4) | (1 << 3) | (1 << 2))) >> 2;
  framep->ctrl->user_input_flag = (bits & (1 << 1)) >> 1;
  framep->ctrl->transparent_color_flag = bits & 1;

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
  unsigned int total_size;
  unsigned char *p;
  unsigned int i;
  unsigned char lgtm_colors[GIF_LGTM_DATA_COLORS_SIZE] = GIF_LGTM_DATA_COLORS;
  unsigned char lgtm_blocks[GIF_LGTM_DATA_BLOCKS_SIZE] = GIF_LGTM_DATA_BLOCKS;
  unsigned int left;
  unsigned int top;

  total_size = bytesp->size + GIF_LGTM_DATA_SIZE;
  p = (unsigned char *) realloc(bytesp->buf, total_size);
  if (p == NULL) {
    free(bytesp->buf);
    die("[ERROR] could not reallocate memory for lgtm bytes");
  }
  bytesp->buf = p;
  bytesp->size = total_size;
  if (bytesp->buf[bytesp->idx - 1] != GIF_TRAILER) {
    die("[FATAL] something wrong with gif data or this program");
  }
  --bytesp->idx;

  bytesp->buf[bytesp->idx++] = GIF_BLOCK_TYPE_EXT;
  bytesp->buf[bytesp->idx++] = GIF_EXT_LABEL_GRAPH_CTRL;
  bytesp->buf[bytesp->idx++] = 4;                                           // Block Size
  bytesp->buf[bytesp->idx++] = (1 << 3) | 1;                                // Reserved | Disposal Method | User Input Flag | Transparent Color Flag
  bytesp->buf[bytesp->idx++] = GIF_LGTM_IMG_DELAY_TIME & 0x000000ff;      // Delay Time
  bytesp->buf[bytesp->idx++] = (GIF_LGTM_IMG_DELAY_TIME & 0x0000ff00) >> 8; // Delay Time
  bytesp->buf[bytesp->idx++] = 252;                                         // Transparent Color Index
  bytesp->buf[bytesp->idx++] = 0;                                           // Block Terminator

  left = (hp->logical_screen_width / 2) - (GIF_LGTM_IMG_MIN_WIDTH / 2);
  top = (hp->logical_screen_height / 2) - (GIF_LGTM_IMG_MIN_HEIGHT / 2);

  bytesp->buf[bytesp->idx++] = GIF_BLOCK_TYPE_IMG;
  bytesp->buf[bytesp->idx++] = left & 0x000000ff;                           // Image Left Position
  bytesp->buf[bytesp->idx++] = (left & 0x0000ff00) >> 8;                    // Image Left Position
  bytesp->buf[bytesp->idx++] = top & 0x000000ff;                            // Image Top Position
  bytesp->buf[bytesp->idx++] = (top & 0x0000ff00) >> 8;                     // Image Top Position
  bytesp->buf[bytesp->idx++] = (GIF_LGTM_IMG_MIN_WIDTH & 0x000000ff);       // Image Width
  bytesp->buf[bytesp->idx++] = (GIF_LGTM_IMG_MIN_WIDTH & 0x0000ff00) >> 8;  // Image Width
  bytesp->buf[bytesp->idx++] = (GIF_LGTM_IMG_MIN_HEIGHT & 0x000000ff);      // Image Height
  bytesp->buf[bytesp->idx++] = (GIF_LGTM_IMG_MIN_HEIGHT & 0x0000ff00) >> 8; // Image Height
  bytesp->buf[bytesp->idx++] = (1 << 7) | (1 << 2) | (1 << 1) | 1;          // Local Color Table Flag | Interlace Flag | Sort Flag | Reserved | Size of Local Color Table
  for (i = 0; i < GIF_LGTM_DATA_COLORS_SIZE; ++i) {
    bytesp->buf[bytesp->idx++] = lgtm_colors[i];                            // Local Color Table
  }
  bytesp->buf[bytesp->idx++] = 8;                                           // LZW Minimum Code Size
  for (i = 0; i < GIF_LGTM_DATA_BLOCKS_SIZE; ++i) {
    bytesp->buf[bytesp->idx++] = lgtm_blocks[i];                            // Block Data with Terminator
  }

  bytesp->buf[bytesp->idx++] = GIF_TRAILER;
}

static void
write_gif_header(FILE *fp, const struct gif_header *hp)
{
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
    print_color_table(fp, hp->size_of_global_color_table, hp->global_color_table, "Global Color Table");
  }
}

static void
write_gif_ext_app(FILE *fp, const struct gif_block_ext_app *appp)
{
  if (!appp->read_flag) return;
  fprintf(fp, "Application Extension:\n");
  fprintf(fp, "  Block Size: %u\n", appp->block_size);
  fprintf(fp, "  Application Identifier: %s\n", appp->application_identifier);
  fprintf(fp, "  Application Authentication Code: %s\n", appp->application_authentication_code);
  fprintf(fp, "  Application Data1: %u\n", appp->application_data[0]);
  fprintf(fp, "  Application Data2: %u\n", appp->application_data[1]);
}

static void
write_gif_blocks(FILE *fp, const struct gif_block_frame *framep)
{
  int i;

  for (i = 1; framep != NULL; framep = framep->next, ++i) {
    fprintf(fp, "Blocks[%d]:\n", i);
    fprintf(fp, "  Block Size: %u\n", framep->ctrl->block_size);
    fprintf(fp, "  Reserved: %u\n", framep->ctrl->reserved);
    fprintf(fp, "  Disposal Method: %u\n", framep->ctrl->disposal_method);
    fprintf(fp, "  User Input Flag: %u\n", framep->ctrl->user_input_flag);
    fprintf(fp, "  Transparent Color Flag: %u\n", framep->ctrl->transparent_color_flag);
    fprintf(fp, "  Delay Time: %umsec\n", framep->ctrl->delay_time * 10);
    fprintf(fp, "  Transparent Color Index: %u\n", framep->ctrl->transparent_color_index);
    fprintf(fp, "  Image Left Position: %u\n", framep->img->image_left_position);
    fprintf(fp, "  Image Top Position: %u\n", framep->img->image_top_position);
    fprintf(fp, "  Image Width: %u\n", framep->img->image_witdh);
    fprintf(fp, "  Image Height: %u\n", framep->img->image_height);
    fprintf(fp, "  Local Color Table Flag: %u\n", framep->img->local_color_table_flag);
    fprintf(fp, "  Interlace Flag: %u\n", framep->img->interlace_flag);
    fprintf(fp, "  Sort Flag: %u\n", framep->img->sort_flag);
    fprintf(fp, "  Reserved: %u\n", framep->img->reserved);
    fprintf(fp, "  Size of Local Color Table: %u\n", framep->img->size_of_local_color_table);
    fprintf(fp, "  LZW Minimum Code Side: %u\n", framep->img->lzw_minimum_code_side);

    if (framep->img->local_color_table_flag) {
      print_color_table(fp, framep->img->size_of_local_color_table, framep->img->local_color_table, "Local Color Table");
    }
  }
}

static void
write_gif_data(FILE *fp, const struct gif_bytes *bytesp)
{
  int n;
  n = fwrite(bytesp->buf, sizeof(unsigned char), bytesp->size, fp);
  if (n != bytesp->size) die("[ERROR] failed to write of gif bytes");
}

static struct gif_block_frame *
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

static int
calc_file_size(FILE *fp)
{
  int size;

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

  for (i = 0, ret = 0; i < n; ++i) {
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
    fprintf(fp, "(%u,%u,%u)", r, g, b);
  }
  fprintf(fp, "\n");
}
