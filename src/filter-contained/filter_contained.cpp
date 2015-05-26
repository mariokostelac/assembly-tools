
#include "afgreader/src/reader.h"

#include <iostream>

using namespace std;
using AMOS::Overlap;
using AMOS::Reader;

bool is_contained(const Overlap& o) {
  auto read1 = o.a_id;
  auto read2 = o.b_id;
  return (o.use_prefix(read1) && o.use_suffix(read1)) ||
    (o.use_prefix(read2) && o.use_suffix(read2));
}

