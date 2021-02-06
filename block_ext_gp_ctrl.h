#ifndef BLOCK_EXT_GP_CTRL_H_
#define BLOCK_EXT_GP_CTRL_H_

#include "./type.h"
#include "./tool.h"
#include "./block_frame.h"

struct gif_block_frame *read_gif_block_ext_graph_ctrl(struct gif_bytes *, struct gif_block_frame *);

#endif  // BLOCK_EXT_GP_CTRL_H_
