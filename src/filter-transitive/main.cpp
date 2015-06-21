
#include "cmdline/cmdline.h"
#include "graph/edges_set.cpp"
#include "mhap/overlap.h"
#include "mhap/parser.h"
#include "ra/include/ra/ra.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

using Graph::EdgesSet;
using std::cerr;
using std::endl;
using std::fstream;
using std::string;
using std::vector;

int main(int argc, char **argv) {
  cmdline::parser args;
  args.add<string>("overlaps", 'x', "overlaps file", true);
  args.add<string>("overlaps_format", 'f', "overlaps file format; supported: afg, mhap", false, "afg");
  args.add<int>("threads", 't', "threads_num", false, 4);
  args.parse_check(argc, argv);

  auto overlaps_format = args.get<string>("overlaps_format");
  auto overlaps_stream_name = args.get<string>("overlaps");
  auto threads_num = args.get<int>("threads");

  vector<Overlap*> overlaps, filtered;

  if (overlaps_format == "afg") {
    vector<Read*> reads;
    readAfgOverlaps(overlaps, overlaps_stream_name.c_str());
    cerr << "Read " << overlaps.size() << " overlaps from " << overlaps_stream_name << endl;
  } else if (overlaps_format == "mhap") {
    istream* overlaps_input_stream = new fstream(overlaps_stream_name);
    MHAP::read_overlaps(*overlaps_input_stream, &overlaps);
    cerr << "Read " << overlaps.size() << " overlaps from " << overlaps_stream_name << endl;
  }

  filterTransitiveOverlaps(filtered, overlaps, threads_num, true);

  for (auto o: overlaps) {
    std::cout << *o;
    delete o;
  }
}
