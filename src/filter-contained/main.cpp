
#include "filter_contained.h"
#include "afgreader/src/reader.h"
#include "cmdline/cmdline.h"
#include "mhap/overlap.h"
#include "mhap/parser.h"
#include <iostream>
#include <vector>

using std::cerr;
using std::cin;
using std::vector;

int read_overlaps(istream& input, vector<AMOS::Overlap*>* overlaps) {
  AMOS::Reader reader(input);

  int stored = 0;
  while (reader.has_next()) {
    if (reader.next_type() == AMOS::OVERLAP) {
      AMOS::Overlap *overlap = new AMOS::Overlap();
      if (!reader.next(overlap)) {
        cerr << "Error while reading overlap" << endl;
        continue;
      }

      overlaps->emplace_back(overlap);
      stored++;
    } else {
      reader.skip_next();
      continue;
    }
  }

  return stored;
}

template <typename T>
int read_and_filter(vector<T>* overlaps, vector<T>* filtered) {
  int read = read_overlaps(cin, overlaps);

  cerr << "Read " << read << " overlaps" << endl;

  get_non_contained(filtered, *overlaps);

  return read;
}

template <typename T>
void output(ostream& out, ostream& err, vector<T>& overlaps, vector<T>& filtered) {
  for (auto o : filtered) {
    cout << *o;
  }

  err << "Written " << filtered.size() << " overlaps. Ratio: " << filtered.size() / (1.0 * overlaps.size()) << endl;
}

int main(int argc, char **argv) {
  ios_base::sync_with_stdio(false);

  cmdline::parser args;
  args.add<string>("format", 'f', "input file format; supported: afg, mhap", false, "afg");
  args.parse_check(argc, argv);

  string format = args.get<string>("format");

  if (format == "afg") {
    vector<AMOS::Overlap*> overlaps, filtered;
    read_and_filter(&overlaps, &filtered);
    output(cout, cerr, overlaps, filtered);
  } else if (format == "mhap") {
    vector<MHAP::Overlap*> overlaps, filtered;
    read_and_filter(&overlaps, &filtered);
    output(cout, cerr, overlaps, filtered);
  }
}
