#include <fstream>
#include <vector>
#include <string>
#include "overlap2dot.h"
#include "afgreader/src/overlap.h"
#include "afgreader/src/reader.h"
#include "cmdline/cmdline.h"
#include "mhap/parser.h"

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
  cmdline::parser args;
  args.add<string>("format", 'f', "input file format; supported: afg, mhap", false, "afg");
  args.parse_check(argc, argv);

  vector<string> input_streams;

  for (int i = 1; i < args.rest().size(); ++i) {
    input_streams.emplace_back(args.rest()[i]);
  }
  if (input_streams.size() == 0) {
    input_streams.emplace_back("-");
  }

  vector<AMOS::Overlap*> overlaps_afg;
  vector<MHAP::Overlap*> overlaps_mhap;

  string format = args.get<string>("format");

  for (auto stream_name : input_streams) {
    cerr << "Starting reading from " << stream_name << endl;
    int read = 0;
    istream* input;
    if (stream_name == "-") {
      input = &cin;
    } else {
      input = new fstream(stream_name);
    }

    if (format == "afg") {
      read = read_amos_overlaps(*input, overlaps_afg);
    } else if (format == "mhap") {
      read = MHAP::read_overlaps(*input, overlaps_mhap);
    }

    cerr << "Read " << read << " objects from " << stream_name << endl;
  }

  if (format == "afg") {
    cerr <<  "Read " << overlaps_afg.size() << " overlaps." << endl;
    dot_graph(cout, overlaps_afg);
  } else if (format == "mhap") {
    cerr <<  "Read " << overlaps_mhap.size() << " overlaps." << endl;
    dot_graph(cout, overlaps_mhap);
  }
}
