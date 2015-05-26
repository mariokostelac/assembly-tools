
#include "afgreader/src/reader.h"

#include <iostream>

using namespace std;
using AMOS::Overlap;
using AMOS::Reader;

bool is_contained(const Overlap& o) {
  auto read1 = o.read1;
  auto read2 = o.read2;
  return (o.use_start(read1) && o.use_end(read1)) ||
    (o.use_start(read2) && o.use_end(read2));
}

