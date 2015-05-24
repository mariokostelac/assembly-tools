
#include "afgreader/src/reader.h"

#include <iostream>
#include <vector>

using namespace std;
using AMOS::Overlap;
using AMOS::Reader;

int main(int argc, char **argv) {

  vector<const Overlap*> overlaps;
  int read = 0;
  int skip = 0;

  Reader* reader = new Reader(cin);
  Overlap* curr_overlap;
  while ((curr_overlap = reader->next_overlap()) != nullptr) {
    overlaps.push_back(curr_overlap);
  }

  cerr << "Read " << read << " overlaps\n" << endl;

  for (int i = 0; i < overlaps.size(); ++i) {
    const auto& overlap = overlaps[i];
    auto read1 = overlap->read1;
    auto read2 = overlap->read2;
    if ((overlap->use_start(read1) && overlap->use_end(read1)) ||
        (overlap->use_start(read2) && overlap->use_end(read2))) {
      ++skip;
      cerr << "Skipping overlap (" << read1 << ", " << read2 << ")" << endl;
      continue;
    }

    cout << *overlap;
  }

  cerr << "Filtered " << skip << " overlaps" << endl;
}
