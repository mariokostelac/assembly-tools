
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

using std::cerr;
using std::endl;
using std::string;
using Graph::EdgesSet;


int main(int argc, char **argv) {
  cmdline::parser args;
  args.add<string>("oformat", 'o', "overlaps file format; supported: afg, mhap", false, "afg");
  args.parse_check(argc, argv);

  string overlaps_format = args.get<string>("oformat");

  if (overlaps_format == "afg") {
    if (args.rest().size() < 2) {
      cerr << args.usage() << endl;
      exit(2);
    }

    const auto read_stream_name = args.rest()[0];
    const auto overlaps_stream_name = args.rest()[1];

    vector<Read*> reads;
    vector<Overlap*> overlaps;

    cerr << "Starting reading from " << read_stream_name << endl;
    readAfgReads(reads, read_stream_name.c_str());
    cerr << "Read " << reads.size() << " reads from " << read_stream_name << endl;

    cerr << "Starting reading from " << overlaps_stream_name << endl;
    readAfgOverlaps(overlaps, overlaps_stream_name.c_str());
    cerr << "Read " << overlaps.size() << " overlaps from " << overlaps_stream_name << endl;

    //cerr << "Filtered " << (overlaps.size() - non_transitive_edges.size()) << " overlaps" << endl;

    //for (auto& o : non_transitive_edges) {
      //cout << *o;
    //}
  } else if (overlaps_format == "mhap") {
    //if (args.rest().size() < 1) {
      //cerr << args.usage() << endl;
      //exit(2);
    //}

    //vector<MHAP::Overlap*> overlaps;
    //auto overlaps_stream_name = args.rest()[0];
    //cerr << "Starting reading from " << overlaps_stream_name << endl;
    //istream* overlaps_input_stream = (overlaps_stream_name == "-") ? &cin : new fstream(overlaps_stream_name);
    //MHAP::read_overlaps(*overlaps_input_stream, &overlaps);
    //cerr << "Read " << overlaps.size() << " overlaps from " << overlaps_stream_name << endl;

    //vector<MHAP::Overlap*> non_transitive_edges;
    //get_non_transitives(&non_transitive_edges, overlaps);

    //cerr << "Filtered " << (overlaps.size() - non_transitive_edges.size()) << " overlaps" << endl;

    //for (auto& o : non_transitive_edges) {
      //cout << *o;
    //}
  } else {
    assert(false);
  }
}
