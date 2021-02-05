#ifndef BLOCK_EXT_H_
#define BLOCK_EXT_H_

#include "./constant.h"
#include "./type.h"
#include "./tool.h"
#include "./block_ext_gp_ctrl.h"
#include "./block_ext_app.h"
#include "./block_ext_comment.h"
#include "./block_ext_plain_text.h"

struct gif_block_frame *read_gif_block_ext(struct gif_bytes *bytesp, struct gif_block_frame *framep, struct gif_block_ext_app *appp);

#endif // BLOCK_EXT_H_
