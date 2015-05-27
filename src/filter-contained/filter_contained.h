#ifndef _FILTER_CONTAINED_H
#define _FILTER_CONTAINED_H

#include "afgreader/src/overlap.h"
#include <vector>

uint32_t get_non_contained(std::vector<AMOS::Overlap*>* dst, const std::vector<AMOS::Overlap*>& overlaps);

#endif
