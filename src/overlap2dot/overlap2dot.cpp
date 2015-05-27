#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "afgreader/src/reader.h"
#include "afgreader/src/overlap.h"
#include "graph/edges_set.cpp"

using namespace std;
using AMOS::Overlap;
using AMOS::Reader;
using Graph::EdgesSet;

const string get_edge_style(const bool use_start, const bool use_end) {
  if (use_start && use_end) {
    return "box";
  } else if (use_start) {
    return "dot";
  } else if (use_end) {
    return "odot";
  }

  return "none";
}

template <typename V>
int dot_graph(ostream& output, vector<V>& overlaps) {
  int lines = 0;

  output << "graph overlaps {\n";
  lines++;

  for (const auto& overlap : overlaps) {
    output << overlap->a_id << " -- " << overlap->b_id << " [";

    string tail_style = get_edge_style(overlap->use_prefix(overlap->a_id), overlap->use_suffix(overlap->b_id));
    string head_style = get_edge_style(overlap->use_prefix(overlap->a_id), overlap->use_suffix(overlap->b_id));

    output << "dir=both arrowtail=" << tail_style << " arrowhead=" << head_style << "];" << endl;
    lines++;
  }

  output << "}\n";
  lines++;

  return lines;
}

int read_from_stream(istream& input, vector<Overlap*>& overlaps) {
  Reader reader(input);

  int stored = 0;
  while (reader.has_next()) {
    if (reader.next_type() == AMOS::OVERLAP) {
      Overlap *overlap = new Overlap();
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

  vector<Overlap*> overlaps;

  for (auto stream_name : input_streams) {
    cerr << "Starting reading from " << stream_name << endl;
    int read = 0;
    if (stream_name == "-") {
      read = read_from_stream(cin, overlaps);
    } else {
      fstream file(stream_name);
      read = read_from_stream(file, overlaps);
      file.close();
      cerr << "Read " << read << " objects from " << stream_name << endl;
    }
  }

  cerr <<  "Read " << overlaps.size() << " overlaps." << endl;

  dot_graph(cout, overlaps);
}
