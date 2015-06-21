
#include "cmdline/cmdline.h"
#include "mhap/overlap.h"
#include "mhap/parser.h"
#include "ra/include/ra/ra.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::max;
using std::string;
using std::vector;

// map reads so we can access reads with mapped[read_id]
void map_reads(vector<Read*>* mapped, vector<Read*>& reads) {
  int max_id = -1;
  for (int i = 0; i < reads.size(); ++i) {
    max_id = max(max_id, reads[i]->getId());
  }

  mapped->resize(max_id + 1, nullptr);
  for (int i = 0; i < reads.size(); ++i) {
    (*mapped)[reads[i]->getId()] = reads[i];
  }
}

int main(int argc, char **argv) {
  cmdline::parser args;
  args.add<string>("reads", 'r', "reads file", true);
  args.add<string>("overlaps", 'x', "overlaps file", true);
  args.add<string>("overlaps_format", 'f', "overlaps file format; supported: afg, mhap", false, "afg");
  args.parse_check(argc, argv);

  const string format = args.get<string>("overlaps_format");
  const string reads_filename = args.get<string>("reads");
  const string overlaps_filename = args.get<string>("overlaps");

  vector<Overlap*> overlaps, filtered;
  vector<Read*> reads;
  vector<Read*> reads_mapped;

  readAfgReads(reads, reads_filename.c_str());
  map_reads(&reads_mapped, reads);

  std::cerr << "Read " << reads.size() << " reads" << std::endl;

  if (format == "afg") {
    readAfgOverlaps(overlaps, overlaps_filename.c_str());
  } else if (format == "mhap") {
    fstream overlaps_file(overlaps_filename);
    MHAP::read_overlaps(overlaps_file, &overlaps);
    overlaps_file.close();
  }

  for (int i = 0; i < overlaps.size(); ++i) {
    const auto a = overlaps[i]->getA();
    const auto b = overlaps[i]->getB();
    assert(reads_mapped[a] != nullptr);
    assert(reads_mapped[b] != nullptr);
  }

  filterContainedOverlaps(filtered, overlaps, reads_mapped, true);

  for (const auto o : filtered) {
    cout << *o;
  }

  for (auto r: reads) {
    delete r;
  }

  for (auto o: overlaps) {
    delete o;
  }

  cerr << "Written " << filtered.size() << " overlaps. Ratio: " << filtered.size() / (1.0 * overlaps.size()) << endl;
}
