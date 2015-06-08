
#ifndef _MHAP_OVERLAP_H
#define _MHAP_OVERLAP_H

#include <cstdint>

namespace MHAP {

  class Overlap {
    public:
      Overlap();

      Overlap(uint32_t a_id, uint32_t b_id, double jaccard_score, uint32_t shared_minmers,
          bool a_fwd, uint32_t a_lo, uint32_t a_hi, uint32_t a_len,
          bool b_fwd, uint32_t b_lo, uint32_t b_hi, uint32_t b_len)
        : a_id(a_id), b_id(b_id), jaccard_score(jaccard_score), shared_minmers(shared_minmers),
        a_fwd(a_fwd), a_lo(a_lo), a_hi(a_hi), a_len(a_len),
        b_fwd(b_fwd), b_lo(b_lo), b_hi(b_hi), b_len(b_len) {}

      uint32_t read1_id() const;
      uint32_t read2_id() const;
      bool use_prefix(uint32_t read_id) const;
      bool use_suffix(uint32_t read_id) const;

    private:
      uint32_t a_id;
      uint32_t b_id;
      double jaccard_score;
      uint32_t shared_minmers;
      bool a_fwd;
      uint32_t a_lo;
      uint32_t a_hi;
      uint32_t a_len;
      bool b_fwd;
      uint32_t b_lo;
      uint32_t b_hi;
      uint32_t b_len;
  };

}

#endif
