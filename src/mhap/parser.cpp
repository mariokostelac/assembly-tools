
#include "parser.h"

namespace MHAP {

  int read_overlaps(istream& input, vector<Overlap*>& overlaps) {
    int read = 0;

    uint32_t a_id;
    uint32_t b_id;
    double jaccard_score;
    double shared_minmers;
    uint32_t a_fwd;
    uint32_t a_lo;
    uint32_t a_hi;
    uint32_t a_len;
    uint32_t b_fwd;
    uint32_t b_lo;
    uint32_t b_hi;
    uint32_t b_len;

    while (!input.eof() && !input.fail()) {
      input >> a_id >> b_id >> jaccard_score >> shared_minmers;
      input >> a_fwd >> a_lo >> a_hi >> a_len;
      input >> b_fwd >> b_lo >> b_hi >> b_len;

      Overlap* overlap = new Overlap(
          a_id, b_id, jaccard_score, shared_minmers,
          a_fwd, a_lo, a_hi, a_len,
          b_fwd, b_lo, b_hi, b_len);

      overlaps.push_back(overlap);
      read++;
    }

    return read;
  }
}
