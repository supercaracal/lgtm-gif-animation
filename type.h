#ifndef TYPE_H_
#define TYPE_H_

typedef struct {
  unsigned char *buf;
  int size;
  int idx;
} BinData;

typedef struct {
  char signature[4];
  char version[4];
  unsigned int logical_screen_width;
  unsigned int logical_screen_height;
  unsigned char global_color_table_flag;
  unsigned char color_resolution;
  unsigned char sort_flag;
  int size_of_global_color_table;
  unsigned char background_color_index;
  unsigned char pixel_aspect_ratio;
  unsigned int *global_color_table;
} GIFHeader;

typedef struct {
  unsigned char block_size;
  char application_identifier[9];
  char application_authentication_code[4];
  char read_flag;
} GIFBlockExtApp;

typedef struct {
  unsigned char block_size;
  unsigned char reserved;
  unsigned char disposal_method;
  unsigned char user_input_flag;
  unsigned char transparent_color_flag;
  unsigned int delay_time;
  unsigned char transparent_color_index;
} GIFBlockExtGpCtrl;

typedef struct {
  unsigned int image_left_position;
  unsigned int image_top_position;
  unsigned int image_witdh;
  unsigned int image_height;
  unsigned char local_color_table_flag;
  unsigned char interlace_flag;
  unsigned char sort_flag;
  unsigned char reserved;
  int size_of_local_color_table;
  unsigned int *local_color_table;
  unsigned char lzw_minimum_code_size;
  BinData lzw_data;
  BinData raw_data;
} GIFBlockImage;

typedef struct gif_block_frame {
  GIFBlockExtGpCtrl *ctrl;
  GIFBlockImage *img;
  struct gif_block_frame *next;
} GIFBlockFrame;

#endif  // TYPE_H_
