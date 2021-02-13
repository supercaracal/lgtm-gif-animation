#include <stdlib.h>
#include "block_ext_gp_ctrl.h"
#include "tool.h"
#include "block_frame.h"

GIFBlockFrame *read_gif_block_ext_graph_ctrl(BinData *bytesp, GIFBlockFrame *framep) {
  unsigned char bits;

  if (framep->ctrl != NULL) framep = add_frame(framep);
  framep->ctrl = (GIFBlockExtGpCtrl *) malloc(sizeof(GIFBlockExtGpCtrl));
  if (framep->ctrl == NULL) {
    die("[ERROR] could not allocate memory for gif graphic control extension");
  }

  framep->ctrl->block_size = bytesp->buf[bytesp->idx++];

  bits = bytesp->buf[bytesp->idx++];
  framep->ctrl->reserved = bits >> 5;
  framep->ctrl->disposal_method = (bits & ((1 << 4) | (1 << 3) | (1 << 2))) >> 2;
  framep->ctrl->user_input_flag = (bits & (1 << 1)) >> 1;
  framep->ctrl->transparent_color_flag = bits & 1;

  framep->ctrl->delay_time = extract_data(&bytesp->buf[bytesp->idx], 2);
  bytesp->idx += 2;

  framep->ctrl->transparent_color_index = bytesp->buf[bytesp->idx++];

  if (bytesp->buf[bytesp->idx] != '\0') {
    die("[ERROR] failed to read from gif graphic control extension block data");
  }

  return framep;
}
