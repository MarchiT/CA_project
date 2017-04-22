#ifndef  COLORS_H_
#define COLORS_H_

#include <stdint.h>

enum color : uint32_t {
  OFF = ((uint32_t)0 << 16) | ((uint32_t)0 <<  8) | (uint32_t)0,
  WHITE = ((uint32_t)255 << 16) | ((uint32_t)255 <<  8) | (uint32_t)255,

  RED = ((uint32_t)255 << 16) | ((uint32_t)0 <<  8) | (uint32_t)0,
  BLUE = ((uint32_t)0 << 16) | ((uint32_t)255 <<  8) | (uint32_t)0,
  GREEN = ((uint32_t)0 << 16) | ((uint32_t)0 <<  8) | (uint32_t)255,

  MAGENTA = ((uint32_t)255 << 16) | ((uint32_t)255 <<  8) | (uint32_t)0,
  YELLOW = ((uint32_t)255 << 16) | ((uint32_t)0 <<  8) | (uint32_t)255,

  PINK = ((uint32_t)255 << 16) | ((uint32_t)200 <<  8) | (uint32_t)0,

  PLAYER = ((uint32_t)0 << 16) | ((uint32_t)155 <<  8) | (uint32_t)150
};


#endif //COLORS_H_
