#include <fstream>
#include <vector>
#include <string>
#include "overlap2dot.h"
#include "afgreader/src/overlap.h"
#include "afgreader/src/reader.h"

using std::string;
using std::vector;

int read_amos_overlaps(istream& input, vector<AMOS::Overlap*>& overlaps) {
  AMOS::Reader reader(input);

  int stored = 0;
  while (reader.has_next()) {
    if (reader.next_type() == AMOS::OVERLAP) {
      AMOS::Overlap *overlap = new AMOS::Overlap();
      if (!reader.next(overlap)) {
        cerr << "Error while reading overlap" << endl;
        continue;
      }

      overlaps.emplace_back(overlap);
      stored++;
    } else {
      reader.skip_next();
      continue;
    }
  }

  return stored;
}

int main(int argc, char **argv) {
  vector<string> input_streams;

  for (int i = 1; i < argc; ++i) {
    input_streams.emplace_back(argv[i]);
  }
  if (argc < 2) {
    input_streams.emplace_back("-");
  }

  vector<AMOS::Overlap*> overlaps;

  for (auto stream_name : input_streams) {
    cerr << "Starting reading from " << stream_name << endl;
    int read = 0;
    if (stream_name == "-") {
      read = read_amos_overlaps(cin, overlaps);
    } else {
      fstream file(stream_name);
      read = read_amos_overlaps(file, overlaps);
      file.close();
      cerr << "Read " << read << " objects from " << stream_name << endl;
    }
  }

  cerr <<  "Read " << overlaps.size() << " overlaps." << endl;

  dot_graph(cout, overlaps);
}
