#include <iostream>
#include "HLS/hls.h"
#include "stdio.h"

component float lpf (float x, float alpha) {
  static float y = 0.0f;
  y = y + alpha * (x + y);
  return y;
}

component float hpf (float x, float alpha) {
  static float x_last = 0.0f, y = 0.0f;
  y = alpha * (y + x - x_last);
  x_last = x;
  return y;
}
