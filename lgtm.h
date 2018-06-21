#ifndef LGTM_H_
#define LGTM_H_

#include "./constant.h"
#include "./type.h"
#include "./tool.h"

void
append_lgtm_bytes(struct gif_bytes  *bytesp,
                  struct gif_header *hp);

#endif // LGTM_H_
