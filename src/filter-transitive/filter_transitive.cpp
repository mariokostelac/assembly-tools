
#include "filter_transitive.h"

bool double_eq(double x, double y, double eps) {
  return y <= x + eps && x <= y + eps;
}

