
#include "filter_contained.h"
#include "afgreader/src/reader.h"
#include <vector>

using std::vector;
using AMOS::Reader;
using AMOS::Overlap;

int main(int argc, char **argv) {
  int read = 0;

  Reader* reader = new Reader(cin);
  vector<Overlap*> overlaps;

  while (reader->has_next()) {
    Overlap* overlap = new Overlap();
    if (reader->next_type() != AMOS::OVERLAP) {
      reader->skip_next();
      continue;
    }

    if (!reader->next(overlap)) {
      cerr << "Error while reading overlap" << endl;
      continue;
    }

    overlaps.emplace_back(overlap);
    read++;
  }

  cerr << "Read " << read << " overlaps" << endl;

  vector<Overlap*> filtered;
  get_non_contained(&filtered, overlaps);

  for (auto o : filtered) {
    cout << o;
  }

  cerr << "Written " << filtered.size() << " overlaps. Ratio: " << filtered.size() / (1.0 * overlaps.size()) << endl;
}
