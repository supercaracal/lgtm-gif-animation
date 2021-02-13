#include "block_ext.h"
#include "constant.h"
#include "tool.h"
#include "block_ext_gp_ctrl.h"
#include "block_ext_app.h"
#include "block_ext_comment.h"
#include "block_ext_plain_text.h"

GIFBlockFrame *read_gif_block_ext(BinData *bytesp, GIFBlockFrame *framep, GIFBlockExtApp *appp) {
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
