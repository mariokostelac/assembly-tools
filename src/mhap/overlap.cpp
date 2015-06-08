#include "overlap.h"

namespace MHAP {

  uint32_t Overlap::read1_id() const {
    return a_id;
  }

  uint32_t Overlap::read2_id() const {
    return b_id;
  }

  bool Overlap::use_prefix(uint32_t read_id) const {
    if (read_id == a_id) {
      if (a_fwd) {
        return a_lo == 0;
      } else {
        return a_hi == a_len;
      }
    } else if (read_id == b_id) {
      if (b_fwd) {
        return b_lo == 0;
      } else {
        return b_hi == b_len;
      }
    }

    return false;
  }

  bool Overlap::use_suffix(uint32_t read_id) const {
    if (read_id == a_id) {
      if (a_fwd) {
        return a_hi == a_len;
      } else {
        return a_lo == 0;
      }
    } else if (read_id == b_id) {
      if (a_fwd) {
        return b_hi == b_len;
      } else {
        return b_lo == 0;
      }
    }

    return false;
  }
}
