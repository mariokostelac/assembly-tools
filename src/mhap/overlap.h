
#ifndef _MHAP_OVERLAP_H
#define _MHAP_OVERLAP_H

#include <cassert>
#include <cstdint>
#include <iostream>
#include "ra/include/ra/ra.hpp"

namespace MHAP {

  class MhapOverlap: public Overlap {
    public:
      MhapOverlap() {};

      MhapOverlap(uint32_t a_id, uint32_t b_id, double jaccard_score, uint32_t shared_minmers,
          bool a_rc, uint32_t a_lo, uint32_t a_hi, uint32_t a_len,
          bool b_rc, uint32_t b_lo, uint32_t b_hi, uint32_t b_len)
        : a_id(a_id), b_id(b_id), jaccard_score(jaccard_score), shared_minmers(shared_minmers),
        a_rc(a_rc), a_lo(a_lo), a_hi(a_hi), a_len(a_len),
        b_rc(b_rc), b_lo(b_lo), b_hi(b_hi), b_len(b_len) {

          assert(!a_rc);
        }

      int getA() const {
        return a_id;
      }

      void setA(int a) {
        a_id = a;
      }

      int getB() const {
        return b_id;
      }

      void setB(int b) {
        b_id = b;
      }

      int getLength() const;

      int getAHang() const;

      int getBHang() const;

      bool isInnie() const;

      // checks whether the start of read is contained in overlap
      // - respects direction of read (important for reverse complements)!
      bool isUsingPrefix(int readId) const;

      // checks whether the end of read is contained in overlap
      // - respects direction of read (important for reverse complements)!
      bool isUsingSuffix(int readId) const;

      uint hangingLength(int readId) const;

      Overlap* clone() const;

      void print(std::ostream& str) const;

    private:
      uint32_t a_id;
      uint32_t b_id;
      double jaccard_score;
      uint32_t shared_minmers;
      bool a_rc;
      uint32_t a_lo;
      uint32_t a_hi;
      uint32_t a_len;
      bool b_rc;
      uint32_t b_lo;
      uint32_t b_hi;
      uint32_t b_len;

      uint32_t lengthInA() const;
      uint32_t lengthInB() const;

      uint32_t hangingLengthA() const;
      uint32_t hangingLengthB() const;
  };
}

#endif
