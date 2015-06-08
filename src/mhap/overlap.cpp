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
      return a_lo == 0;
    } else if (read_id == b_id) {
      return b_lo == 0;
    }

    return false;
  }

  bool Overlap::use_suffix(uint32_t read_id) const {
    if (read_id == a_id) {
      return a_hi == a_len;
    } else if (read_id == b_id) {
      return b_hi == b_len;
    }

    return false;
  }
}
