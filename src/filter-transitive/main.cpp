
#include "filter_transitive.h"
#include "afgreader/src/reader.h"
#include "cmdline/cmdline.h"
#include "graph/edges_set.cpp"
#include "mhap/overlap.h"
#include "mhap/parser.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

using std::string;
using Graph::EdgesSet;

int afg_reads(map<uint32_t, AMOS::Read*>* reads, istream& input) {
  AMOS::Reader reader(input);

  int stored = 0;
  while (reader.has_next()) {
    if (reader.next_type() == AMOS::READ) {
      auto read = new AMOS::Read();
      if (!reader.next(read)) {
        cerr << "Error while reading read" << endl;
        continue;
      }

      (*reads)[read->iid] = read;
      stored++;
    } else {
      reader.skip_next();
      continue;
    }
  }

  return stored;
}

int afg_overlaps(vector<AMOS::Overlap*>* overlaps, map<uint32_t, AMOS::Read*>& reads,  istream& input) {
  AMOS::Reader reader(input);

  int stored = 0;
  while (reader.has_next()) {
    if (reader.next_type() == AMOS::OVERLAP) {
      auto overlap = new Overlap();
      if (!reader.next(overlap)) {
        cerr << "Error while reading overlap" << endl;
        continue;
      }

      overlap->a = reads[overlap->a_id];
      overlap->b = reads[overlap->b_id];

      overlaps->emplace_back(overlap);
      stored++;
    } else {
      reader.skip_next();
      continue;
    }
  }

  return stored;
}

int main(int argc, char **argv) {
  ios_base::sync_with_stdio(false);

  cmdline::parser args;
  args.add<string>("rformat", 'r', "reads file format; supported: afg, mhap", false, "afg");
  args.parse_check(argc, argv);

  if (args.rest().size() < 2) {
    cerr << args.usage() << endl;
    exit(2);
  }

  string reads_format = args.get<string>("format");

  // handling reads
  map<uint32_t, AMOS::Read*> reads;

  auto read_stream_name = args.rest()[0];
  cerr << "Starting reading from " << read_stream_name << endl;
  istream* reads_input_stream = (read_stream_name == "-") ? &cin : new fstream(read_stream_name);
  afg_reads(&reads, *reads_input_stream);
  cerr << "Read " << reads.size() << " reads from " << read_stream_name << endl;

  if (reads_format == "afg") {
    vector<AMOS::Overlap*> overlaps;
    auto overlaps_stream_name = args.rest()[1];
    cerr << "Starting reading from " << overlaps_stream_name << endl;
    istream* overlaps_input_stream = (read_stream_name == "-") ? &cin : new fstream(overlaps_stream_name);
    afg_overlaps(&overlaps, reads, *overlaps_input_stream);
    cerr << "Read " << overlaps.size() << " overlaps from " << overlaps_stream_name << endl;

    vector<Overlap*> non_transitive_edges;
    get_non_transitives(&non_transitive_edges, overlaps);

    cerr << "Filtered " << (overlaps.size() - non_transitive_edges.size()) << " overlaps" << endl;

    for (auto& o : non_transitive_edges) {
      cout << *o;
    }
  } else if (reads_format == "mhap") {
    vector<MHAP::Overlap*> overlaps;
    auto overlaps_stream_name = args.rest()[1];
    cerr << "Starting reading from " << overlaps_stream_name << endl;
    istream* overlaps_input_stream = (read_stream_name == "-") ? &cin : new fstream(overlaps_stream_name);
    MHAP::read_overlaps(*overlaps_input_stream, &overlaps);
    cerr << "Read " << overlaps.size() << " overlaps from " << overlaps_stream_name << endl;

    vector<MHAP::Overlap*> non_transitive_edges;
    get_non_transitives(&non_transitive_edges, overlaps);

    cerr << "Filtered " << (overlaps.size() - non_transitive_edges.size()) << " overlaps" << endl;

    for (auto& o : non_transitive_edges) {
      cout << *o;
    }
  } else {
    assert(false);
  }
}
