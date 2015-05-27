
#include "afgreader/src/reader.h"

#include <iostream>
#include <set>

using namespace std;
using AMOS::Overlap;
using AMOS::Reader;

uint32_t get_non_contained(vector<Overlap*>* dst, const vector<Overlap*>& overlaps) {
  uint32_t added = 0;

  set<uint32_t> removed;
  for (auto o : overlaps) {
    // -a -------> +b
    // ------------->
    bool a_contained = o->a_hang <= 0 && o->b_hang >= 0;
    if (a_contained) {
      removed.insert(o->a_id);
      continue;
    }

    // ------------->
    // +a -------> -b
    bool b_contained = o->a_hang >= 0 && o->b_hang <= 0;
    if (b_contained) {
      removed.insert(o->b_id);
      continue;
    }
  }

  for (auto o : overlaps) {
    if (removed.count(o->a_id)) {
      continue;
    }
    if (removed.count(o->b_id)) {
      continue;
    }

    dst->push_back(o);
  }

  return added;
}

