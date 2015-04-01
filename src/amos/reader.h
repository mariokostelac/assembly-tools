
#ifndef _AMOS_READER_H
#define _AMOS_READER_H

#include "read.h"
#include "overlap.h"

#include <cstdio>
#include <vector>

namespace AMOS {
  int get_reads(std::vector<const Read*>& container, FILE *fd);
  int get_overlaps(std::vector<const Overlap*>& container, FILE *fd);
}

#endif
