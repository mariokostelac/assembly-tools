#include "overlap.h"
#include <cassert>

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

  uint32_t Overlap::length() const {
    return (length_in_a() + length_in_b())/2;
  }

  uint32_t Overlap::length_in_a() const {
    int32_t olen = a_hi - a_lo + 1;
    assert(olen >= 0 && olen <= a_len);
    return olen;
  }

  uint32_t Overlap::length_in_b() const {
    int32_t olen = b_hi - b_lo + 1;
    assert(olen >= 0 && olen <= b_len);
    return olen;
  }

  uint32_t Overlap::hanging_length(uint32_t r_id) const {
    assert(r_id == a_id || r_id == b_id);
    if (r_id == a_id) {
      return hanging_length_a();
    }
    return hanging_length_b();
  }

  uint32_t Overlap::hanging_length_a() const {
    return a_len - length_in_a();
  }

  uint32_t Overlap::hanging_length_b() const {
    return b_len - length_in_b();
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
