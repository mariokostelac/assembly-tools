#include "overlap.h"
#include <cassert>

#define THRESHOLD 0.08

using std::endl;
using std::ostream;

namespace MHAP {

  bool MhapOverlap::isUsingPrefix(int read_id) const {

    if ((uint) read_id == a_id) {
      return a_lo <= THRESHOLD * a_len;
    } else if ((uint) read_id == b_id) {
      return b_lo <= THRESHOLD * b_len;
    }

    assert(false);
  }

  bool MhapOverlap::isUsingSuffix(int read_id) const {

    if ((uint) read_id == a_id) {
      return a_hi >= a_len * (1 - THRESHOLD);
    } else if ((uint) read_id == b_id) {
      return b_hi >= b_len * (1 - THRESHOLD);
    }

    assert(false);
  }

  int MhapOverlap::getLength() const {
    return (lengthInA() + lengthInB())/2;
  }

  uint32_t MhapOverlap::lengthInA() const {
    int32_t olen = a_hi - a_lo + 1;
    assert(olen >= 0 && olen <= (int32_t) a_len);
    return (uint32_t) olen;
  }

  uint32_t MhapOverlap::lengthInB() const {
    int32_t olen = b_hi - b_lo + 1;
    assert(olen >= 0 && olen <= (int32_t) b_len);
    return (uint32_t) olen;
  }

  uint MhapOverlap::hangingLength(int r_id) const {
    assert((uint) r_id == a_id || (uint) r_id == b_id);
    if ((uint) r_id == a_id) {
      return hangingLengthA();
    }
    return hangingLengthB();
  }

  uint32_t MhapOverlap::hangingLengthA() const {
    return a_len - lengthInA();
  }

  uint32_t MhapOverlap::hangingLengthB() const {
    return b_len - lengthInB();
  }

  bool MhapOverlap::isInnie() const {
    return b_rc;
  }

  int MhapOverlap::getAHang() const {
    if (!a_rc && !b_rc) {
      // -----|------|---->
      //  ah -|------|---->
      //
      // -ah -|------|---->
      // -----|------|---->
      return a_lo - b_lo;
    } else if (!a_rc && b_rc) {
      // -----|------|---->
      //  ah <|------|-----
      //
      // -ah -|------|---->
      // <----|------|-----
      return a_lo - (b_len - b_hi);
    }

    assert(false);
  }

  int MhapOverlap::getBHang() const {
    if (!a_rc && !b_rc) {
      //     -|------|-> bh
      // -----|------|------>
      //
      // -----|------|------>
      //     -|------|-> -bh
      int b_after = b_len - b_hi;
      int a_after = a_len - a_hi;
      return b_after - a_after;
    } else if (!a_rc && b_rc) {
      //     -|------|-> bh
      // <----|------|-------
      //
      // -----|------|------>
      //     <|------|-- -bh
      int a_after = a_len - a_hi;
      int b_after = b_lo;
      return b_after - a_after;
    }

    assert(false);
  }

  Overlap* MhapOverlap::clone() const {
    auto copy = new MhapOverlap();
    copy->a_id = a_id;
    copy->b_id = b_id;
    copy->jaccard_score = jaccard_score;
    copy->shared_minmers = shared_minmers;
    copy->a_rc = a_rc;
    copy->a_lo = a_lo;
    copy->a_hi = a_hi;
    copy->a_len = a_len;
    copy->b_rc = b_rc;
    copy->b_lo = b_lo;
    copy->b_hi = b_hi;
    copy->b_len = b_len;
    return copy;
  }

  void MhapOverlap::print(std::ostream& o) const {
    o << a_id << " " << b_id << " " << jaccard_score << " ";
    o << (double) (shared_minmers) << " ";
    o << (int) (a_rc) << " " << a_lo << " " << a_hi << " " << a_len << " ";
    o << (int) (b_rc) << " " << b_lo << " " << b_hi << " " << b_len << " ";
    o << endl;
  }
}
