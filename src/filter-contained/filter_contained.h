#ifndef _FILTER_CONTAINED_H
#define _FILTER_CONTAINED_H

#include "afgreader/src/overlap.h"
#include <vector>

// fills dst vector with overlaps conincident with reads that are not contained in any read from a given set.
uint32_t get_non_contained(std::vector<AMOS::Overlap*>* dst, const std::vector<AMOS::Overlap*>& overlaps);

#endif
