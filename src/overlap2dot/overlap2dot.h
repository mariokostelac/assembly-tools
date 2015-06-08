#include <vector>
#include <string>
#include "afgreader/src/reader.h"
#include "afgreader/src/overlap.h"
#include "graph/edges_set.cpp"

using std::string;
using AMOS::Overlap;
using AMOS::Reader;
using Graph::EdgesSet;

const string get_edge_style(const bool use_start, const bool use_end);

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

