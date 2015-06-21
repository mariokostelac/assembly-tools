#include <fstream>
#include <vector>
#include <string>
#include "overlap2dot.h"
#include "cmdline/cmdline.h"
#include "mhap/parser.h"

using std::cerr;
using std::cin;
using std::cout;
using std::fstream;
using std::string;
using std::vector;
using std::endl;

int main(int argc, char **argv) {
  cmdline::parser args;
  args.add<string>("format", 'f', "input file format; supported: afg, mhap", false, "afg");
  args.parse_check(argc, argv);

  vector<string> input_streams;

  for (int i = 0; i < args.rest().size(); ++i) {
    input_streams.emplace_back(args.rest()[i]);
  }
  if (input_streams.size() == 0) {
    input_streams.emplace_back("-");
  }

  vector<Overlap*> overlaps;

  string format = args.get<string>("format");

  for (auto stream_name : input_streams) {
    cerr << "Starting reading from " << stream_name << endl;

    if (format == "afg") {
      readAfgOverlaps(overlaps, stream_name.c_str());
    } else if (format == "mhap") {
      fstream file(stream_name);
      MHAP::read_overlaps(file, &overlaps);
      file.close();
    }
  }

  cerr <<  "Read " << overlaps.size() << " overlaps." << endl;
  dot_graph(cout, overlaps);
}
