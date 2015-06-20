
#include "cmdline/cmdline.h"
#include "mhap/overlap.h"
#include "mhap/parser.h"
#include "ra/include/ra/ra.hpp"
#include <fstream>
#include <iostream>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;
using std::vector;

int main(int argc, char **argv) {

  cmdline::parser args;
  args.add<string>("oformat", 'f', "overlaps file format; supported: afg, mhap", false, "afg");
  args.parse_check(argc, argv);

  string format = args.get<string>("format");

  if (args.rest().size() < 2) {
    // TODO
    assert(false);
  }

  vector<Overlap*> overlaps, filtered;
  vector<Read*> reads;

  const auto reads_filename = args.rest()[0];
  const auto overlaps_filename = args.rest()[1];

  readAfgReads(reads, reads_filename.c_str());

  if (format == "afg") {
    readAfgOverlaps(overlaps, overlaps_filename.c_str());
  } else if (format == "mhap") {
    fstream overlaps_file(overlaps_filename);
    MHAP::read_overlaps(overlaps_file, &overlaps);
    overlaps_file.close();
  }

  filterContainedOverlaps(filtered, overlaps, reads, true);

  for (const auto o : filtered) {
    cout << *o;
  }

  cerr << "Written " << filtered.size() << " overlaps. Ratio: " << filtered.size() / (1.0 * overlaps.size()) << endl;
}
