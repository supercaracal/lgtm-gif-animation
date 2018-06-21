#include "block_ext.h"

struct gif_block_frame *
read_gif_block_ext(struct gif_bytes         *bytesp,
                   struct gif_block_frame   *framep,
                   struct gif_block_ext_app *appp)
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
