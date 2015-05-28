
#include "filter_transitive.h"
#include "graph/edges_set.cpp"
#include <algorithm>

using Graph::EdgesSet;

const double EPSILON = 0.15;
const double ALPHA = 3;

inline bool double_eq(double x, double y, double eps) {
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
  if (!double_eq(
        o2.hanging_length(A) + o3.hanging_length(C),
        o1.hanging_length(A),
        EPSILON * o1.length() + ALPHA)) {
    return false;
  }
  if (!double_eq(
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
  for (auto& unsorted : edges_set) {
    sort(unsorted.second.begin(), unsorted.second.end());
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
          //cerr << "Filtering out (" << src << ", " << dst << ") because transitive to "
            //<< "(" << it1->second->a_id << ", " << it1->second->b_id << "), "
            //<< "(" << it2->second->a_id << ", " << it2->second->b_id << ")"
            //<< endl;
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
