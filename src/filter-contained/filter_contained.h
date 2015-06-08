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
    if (o->use_prefix(o->read1_id()) && o->use_suffix(o->read1_id())) {
      removed[o->read1_id()]++;
      continue;
    }

    // ------------->
    //    ------->
    if (o->use_prefix(o->read2_id()) && o->use_suffix(o->read2_id())) {
      removed[o->read2_id()]++;
      continue;
    }
  }

  for (auto o : overlaps) {
    if (removed[o->read1_id()]) {
      continue;
    }
    if (removed[o->read2_id()]) {
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
    max_read_id = max(max_read_id, o->read1_id());
    max_read_id = max(max_read_id, o->read2_id());
  }

  return get_non_contained(dst, overlaps, max_read_id);
}

#endif
