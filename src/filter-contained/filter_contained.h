#ifndef _FILTER_CONTAINED_H
#define _FILTER_CONTAINED_H

#include <vector>
#include <iostream>

using namespace std;

// fills dst vector with overlaps conincident with reads that are not contained in any read from a given set.
template <typename T>
uint32_t get_non_contained(std::vector<T>* dst, const std::vector<T>& overlaps, const uint32_t max_read_id) {
  uint32_t added = 0;

  vector<uint32_t> removed(max_read_id + 1);
  for (auto o : overlaps) {
    //    --------
    // --------------
    bool a_contained = o->a_hang <= 0 && o->b_hang >= 0;
    if (a_contained) {
      removed[o->a_id]++;
      continue;
    }

    // ------------->
    //    ------->
    bool b_contained = o->a_hang >= 0 && o->b_hang <= 0;
    if (b_contained) {
      removed[o->b_id]++;
      continue;
    }
  }

  for (auto o : overlaps) {
    if (removed[o->a_id]) {
      continue;
    }
    if (removed[o->b_id]) {
      continue;
    }

    dst->push_back(o);
    ++added;
  }

  return added;
}

// fills dst vector with overlaps conincident with reads that are not contained in any read from a given set.
template <typename T>
uint32_t get_non_contained(std::vector<T>* dst, const std::vector<T>& overlaps) {
  uint32_t max_read_id = 0;
  for (auto o : overlaps) {
    max_read_id = max(max_read_id, o->a_id);
    max_read_id = max(max_read_id, o->b_id);
  }

  return get_non_contained(dst, overlaps, max_read_id);
}

#endif
