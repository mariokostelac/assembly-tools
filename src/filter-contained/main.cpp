
#include "filter_contained.h"
#include "afgreader/src/reader.h"

using AMOS::Reader;
using AMOS::Overlap;

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
      cerr << "Skipping overlap (" << overlap.a_id << ", " << overlap.b_id << ")" << endl;
      continue;
    }

    cout << overlap;
    read++;
  }

  cerr << "Read " << read << " overlaps" << endl;
  cerr << "Filtered " << skip << " overlaps" << endl;
}
