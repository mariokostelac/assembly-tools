
#include "afgreader/src/reader.h"
#include "graph/edges_set.cpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

using std::shared_ptr;
using AMOS::Overlap;
using AMOS::Read;
using AMOS::Reader;
using Graph::EdgesSet;

const double EPSILON = 0.15;
const double ALPHA = 3;

bool compare_edges(const pair<int, shared_ptr<Overlap>>& lhs, const pair<int, shared_ptr<Overlap>>& rhs) {
  return lhs.first < rhs.first;
}

inline bool eq(double x, double y, double eps) {
  return y <= x + eps && x <= y + eps;
}

// tests if o1 is transitive edge considering o2 and o3.
bool is_transitive(const Overlap& o1, const Overlap& o2, const Overlap& o3) {
  auto A = o1.a_id;
  auto B = o1.b_id;
  auto C = o2.a_id;
  if (C == A) {
    C = o2.b_id;
  }
  if (o2.use_suffix(C) == o3.use_suffix(C)) {
    return false;
  }
  if (o1.use_suffix(A) != o2.use_suffix(A)) {
    return false;
  }
  if (o1.use_suffix(B) != o3.use_suffix(B)) {
    return false;
  }
  if (!eq(
        o2.hanging_length(A) + o3.hanging_length(C),
        o1.hanging_length(A),
        EPSILON * o1.length() + ALPHA)) {
    return false;
  }
  if (!eq(
        o2.hanging_length(C) + o3.hanging_length(B),
        o1.hanging_length(B),
        EPSILON * o1.length() + ALPHA)) {
    return false;
  }
  return true;
}

int get_non_transitives(vector<shared_ptr<Overlap>>* dst_container, vector<shared_ptr<Overlap>>& edges_list) {
  int valid = 0;

  // create that data structure to allow fast access to all edges coincident
  // with particular node.
  EdgesSet<shared_ptr<Overlap>> edges_set;
  for (const auto& edge : edges_list) {
    edges_set.add(edge->a_id, edge->b_id, edge);
    edges_set.add(edge->b_id, edge->a_id, edge);
  }

  // sort so we can reduce complexity on iterating edges.
  for (auto unsorted : edges_set) {
    unsorted.second.sort();
  }

  // we are iterating all (x, y), (x, a), (y, a) in hope that we will find that
  // (x,y) is transitive edge when considering (x, a) and (y, a).
  // x -> y, x -> a -> y; x -> y can be information overhead.
  for (const auto& overlap : edges_list) {
    auto& src = overlap->a_id;
    auto& dst = overlap->b_id;

    auto& v1 = edges_set[src], v2 = edges_set[dst];
    auto it1 = v1.begin(), it2 = v2.begin();

    bool transitive = false;
    while (!transitive && it1 != v1.end() && it2 != v2.end()) {
      if (it1->first == overlap->a_id || it1->first == overlap->b_id) {
        ++it1;
        continue;
      }
      if (it2->first == overlap->a_id || it2->first == overlap->b_id) {
        ++it2;
        continue;
      }
      if (it1->first == it2->first) {
        if (is_transitive(*overlap, *it1->second, *it2->second)) {
          transitive = true;
        }
        ++it1;
        ++it2;
      } else if (it1->first < it2->first) {
        ++it1;
      } else {
        ++it2;
      }
    }

    if (!transitive) {
      dst_container->emplace_back(overlap);
    }
  }

  return valid;
}

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
        cerr << "Error while reading overlap" << endl;
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
  vector<istream*> input_streams;

  if (argc == 2) {
    input_streams.emplace_back(&cin);
  } else {
    for (int i = 1; i < argc; ++i) {
      istream* in_stream;
      if (strcmp(argv[i], "-") == 0) {
        in_stream = &cin;
      } else {
        in_stream = new ifstream(argv[i]);
      }

      input_streams.emplace_back(&cin);
    }
  }

  vector<shared_ptr<Overlap>> overlaps;
  map<uint32_t, shared_ptr<Read>> reads;

  for (auto in_stream : input_streams) {
    read_from_stream(*in_stream, reads, overlaps);
  }

  cerr << "Read " << overlaps.size() << " reads" << endl;
  cerr << "Read " << reads.size() << " overlaps" << endl;

  fill_reads(overlaps, reads);

  vector<shared_ptr<Overlap>> non_transitive_edges;
  get_non_transitives(&non_transitive_edges, overlaps);

  cerr << "Filtered " << (overlaps.size() - non_transitive_edges.size()) << " overlaps" << endl;
}
