
#include "filter_transitive.h"
#include "afgreader/src/reader.h"
#include "graph/edges_set.cpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

using std::shared_ptr;
using std::string;
using AMOS::Overlap;
using AMOS::Read;
using AMOS::Reader;
using Graph::EdgesSet;

void fill_reads(vector<shared_ptr<Overlap>>& overlaps, map<uint32_t, shared_ptr<Read>>& reads) {
  for (auto o : overlaps) {
    o->a = reads[o->a_id];
    o->b = reads[o->b_id];
  }
}

int read_from_stream(istream& input, map<uint32_t, shared_ptr<Read>>& reads, vector<shared_ptr<Overlap>>& edges) {
  Reader reader(input);

  int stored = 0;
  while (reader.has_next()) {
    if (reader.next_type() == AMOS::OVERLAP) {
      Overlap* overlap = new Overlap();
      if (!reader.next(overlap)) {
        cerr << "Error while reading overlap" << endl;
        continue;
      }

      edges.emplace_back(shared_ptr<Overlap>(overlap));
      stored++;
    } else if (reader.next_type() == AMOS::READ) {
      Read *read = new Read();
      if (!reader.next(read)) {
        cerr << "Error while reading read" << endl;
        continue;
      }

      reads[read->iid] = shared_ptr<Read>(read);
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

  vector<shared_ptr<Overlap>> overlaps;
  map<uint32_t, shared_ptr<Read>> reads;

  for (auto stream_name : input_streams) {
    cerr << "Starting reading from " << stream_name << endl;
    int read = 0;
    if (stream_name == "-") {
      read = read_from_stream(cin, reads, overlaps);
    } else {
      fstream file(stream_name);
      read = read_from_stream(file, reads, overlaps);
      file.close();
    cerr << "Read " << read << " objects from " << stream_name << endl;
    }
  }

  cerr << "Read " << reads.size() << " reads" << endl;
  cerr << "Read " << overlaps.size() << " overlaps" << endl;

  fill_reads(overlaps, reads);

  vector<shared_ptr<Overlap>> non_transitive_edges;
  get_non_transitives(&non_transitive_edges, overlaps);

  cerr << "Filtered " << (overlaps.size() - non_transitive_edges.size()) << " overlaps" << endl;

  for (auto& o : non_transitive_edges) {
    cout << *o;
  }
}
