
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

int main(int argc, char **argv) {
  int read = 0;
  int skip = 0;

  Reader* reader = new Reader(cin);

  while (reader->has_next()) {
    Overlap overlap;
    if (reader->next_type() != AMOS::OVERLAP) {
      reader->skip_next();
      continue;
    }

    if (!reader->next(&overlap)) {
      cerr << "Error while reading overlap" << endl;
      continue;
    }

    if (is_contained(overlap)) {
      ++skip;
      cerr << "Skipping overlap (" << overlap.read1 << ", " << overlap.read2 << ")" << endl;
      continue;
    }

    cout << overlap;
    read++;
  }

  cerr << "Read " << read << " overlaps" << endl;
  cerr << "Filtered " << skip << " overlaps" << endl;
}
