#include "overlap.h"
#include <cassert>

#define THRESHOLD 0.08

using std::endl;
using std::ostream;

namespace MHAP {

  bool MhapOverlap::isUsingPrefix(int read_id) const {

    if (read_id == (int) a_id) {
      if (getAHang() <= 0) return true;

    } else if (read_id == (int) b_id) {
      if (b_rc == false && getAHang() >= 0) return true;
      if (b_rc == true && getBHang() <= 0) return true;
    }

    return false;
  }

  bool MhapOverlap::isUsingSuffix(int read_id) const {

    if (read_id == (int) a_id) {
      if (getBHang() >= 0) return true;

    } else if (read_id == (int) b_id) {
      if (b_rc == false && getBHang() <= 0) return true;
      if (b_rc == true && getAHang() >= 0) return true;
    }

    return false;
  }

  //bool MhapOverlap::isUsingPrefix(int read_id) const {
    //if (isReallyUsingPrefix(read_id)) {
      //return true;
    //}

    //if (isReallyUsingSuffix(read_id)) {
      //return false;
    //}

    //// overlap does not use prefix nor suffix!
    //if (read_id == a_id) {
      //int a_before = a_lo;
      //int a_after = a_len - a_hi;

      //return a_before < a_after;
    //} else if (read_id == b_id) {
      //int b_before = b_lo;
      //int b_after = b_len - b_hi;
      //if (b_rc) {
        //std::swap(b_before, b_after);
      //}

      //return b_before < b_after;
    //} else {
      //assert(false);
    //}

    //return false;
  //}

  //bool MhapOverlap::isUsingSuffix(int read_id) const {
    //if (isReallyUsingSuffix(read_id)) {
      //return true;
    //}

    //if (isReallyUsingPrefix(read_id)) {
      //return false;
    //}

    //// overlap does not use prefix nor suffix!
    //if (read_id == a_id) {
      //int a_before = a_lo;
      //int a_after = a_len - a_hi;

      //return a_before > a_after;
    //} else if (read_id == b_id) {
      //int b_before = b_lo;
      //int b_after = b_len - b_hi;
      //if (b_rc) {
        //std::swap(b_before, b_after);
      //}

      //return b_before > b_after;
    //} else {
      //assert(false);
    //}

    //return false;
  //}

  bool MhapOverlap::isReallyUsingPrefix(int read_id) const {

    if ((uint) read_id == a_id) {
      return a_lo <= THRESHOLD * a_len;
    } else if ((uint) read_id == b_id) {
      return b_lo <= THRESHOLD * b_len;
    }

    assert(false);
  }

  bool MhapOverlap::isReallyUsingSuffix(int read_id) const {

    if ((uint) read_id == a_id) {
      return a_hi >= a_len * (1 - THRESHOLD);
    } else if ((uint) read_id == b_id) {
      return b_hi >= b_len * (1 - THRESHOLD);
    }

    assert(false);
  }

  int MhapOverlap::getLength(int read_id) const {
    if (read_id == (int) a_id) {
      return getLengthA();
    } else if (read_id == (int) b_id) {
      return getLengthB();
    }

    assert(false);
  }

  int MhapOverlap::getLengthA() const {
    assert(a_read != nullptr);

    int len = a_read->getSequence().length();
    if (getAHang() > 0) {
      len -= getAHang();
    }
    if (getBHang() < 0) {
      len -= abs(getBHang());
    }

    return len;
  }

  int MhapOverlap::getLengthB() const {
    assert(b_read != nullptr);

    int len = b_read->getSequence().length();
    if (getAHang() < 0) {
      len -= abs(getAHang());
    }
    if (getBHang() > 0) {
      len -= getBHang();
    }

    return len;
  }

  int MhapOverlap::getLength() const {
    return (getLength(a_id) + getLength(b_id))/2;
  }

  uint MhapOverlap::hangingLength(int r_id) const {
    assert((uint) r_id == a_id || (uint) r_id == b_id);
    if ((uint) r_id == a_id) {
      return hangingLengthA();
    }
    return hangingLengthB();
  }

  uint32_t MhapOverlap::hangingLengthA() const {
    return a_len - getLength(a_id);
  }

  uint32_t MhapOverlap::hangingLengthB() const {
    return b_len - getLength(b_id);
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
