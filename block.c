#include "block.h"

void
read_gif_blocks(struct gif_bytes         *bytesp,
                struct gif_block_frame   *framep,
                struct gif_block_ext_app *appp)
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

void
write_gif_blocks(FILE                         *fp,
                 const struct gif_block_frame *framep)
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
    fprintf(fp, "  LZW Minimum Code Size: %u\n", framep->img->lzw_minimum_code_size);

    if (framep->img->local_color_table_flag) {
      print_color_table(fp,
                        framep->img->size_of_local_color_table,
                        framep->img->local_color_table,
                        "Local Color Table");
    }
  }
}
