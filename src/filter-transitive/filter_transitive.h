
#ifndef _FILTER_CONTAINED_H
#define _FILTER_CONTAINED_H

#include "afgreader/src/overlap.h"
#include <memory>
#include <vector>

using std::vector;
using AMOS::Overlap;

// tests if o1 is transitive edge considering o2 and o3.
// overlaps: {(A, B), (B, C), (A, C)}, (A, C) has a chance to be transitive.
bool is_transitive(const Overlap& o1, const Overlap& o2, const Overlap& o3);

// fills dst_container with non-transitive reads.
int get_non_transitives(vector<shared_ptr<AMOS::Overlap>>* dst_container, vector<shared_ptr<Overlap>>& edges_list);

#endif
