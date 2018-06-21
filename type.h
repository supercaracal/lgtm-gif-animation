#ifndef TYPE_H_
#define TYPE_H_

struct gif_bytes {
  unsigned char *buf;
  int           size;
  int           idx;
};

struct gif_header {
  char          signature[4];
  char          version[4];
  unsigned int  logical_screen_width;
  unsigned int  logical_screen_height;
  unsigned char global_color_table_flag;
  unsigned char color_resolution;
  unsigned char sort_flag;
  unsigned int  size_of_global_color_table;
  unsigned char background_color_index;
  unsigned char pixel_aspect_ratio;
  unsigned int  *global_color_table;
};

struct gif_block_ext_app {
  unsigned char block_size;
  char          application_identifier[9];
  char          application_authentication_code[4];
  char          read_flag;
};

struct gif_block_ext_gp_ctrl {
  unsigned int  block_size;
  unsigned char reserved;
  unsigned char disposal_method;
  unsigned char user_input_flag;
  unsigned char transparent_color_flag;
  unsigned int  delay_time;
  unsigned char transparent_color_index;
};

struct gif_block_image {
  unsigned int  image_left_position;
  unsigned int  image_top_position;
  unsigned int  image_witdh;
  unsigned int  image_height;
  unsigned char local_color_table_flag;
  unsigned char interlace_flag;
  unsigned char sort_flag;
  unsigned char reserved;
  unsigned int  size_of_local_color_table;
  unsigned int  *local_color_table;
  unsigned char lzw_minimum_code_side;
};

struct gif_block_frame {
  struct gif_block_ext_gp_ctrl *ctrl;
  struct gif_block_image       *img;
  struct gif_block_frame       *next;
};

#endif // TYPE_H_
