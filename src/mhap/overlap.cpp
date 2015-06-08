#include "overlap.h"

#define THRESHOLD 0.08

using std::endl;
using std::ostream;

namespace MHAP {

  uint32_t Overlap::read1_id() const {
    return a_id;
  }

  uint32_t Overlap::read2_id() const {
    return b_id;
  }

  bool Overlap::use_prefix(uint32_t read_id) const {
    if (read_id == a_id) {
      return a_lo <= THRESHOLD * a_len;
    } else if (read_id == b_id) {
      return b_lo <= THRESHOLD * b_len;
    }

    return false;
  }

  bool Overlap::use_suffix(uint32_t read_id) const {
    if (read_id == a_id) {
      return a_hi >= a_len * (1 - THRESHOLD);
    } else if (read_id == b_id) {
      return b_hi >= b_len * (1 - THRESHOLD);
    }

    return false;
  }

  std::ostream& operator << (std::ostream &o, const Overlap& overlap) {
    o << overlap.a_id << " " << overlap.b_id << " " << overlap.jaccard_score << " ";
    o << (double) (overlap.shared_minmers) << " ";
    o << (int) (overlap.a_fwd) << " " << overlap.a_lo << " " << overlap.a_hi << " " << overlap.a_len << " ";
    o << (int) (overlap.b_fwd) << " " << overlap.b_lo << " " << overlap.b_hi << " " << overlap.b_len << " ";
    o << endl;
    return o;
  }
}
