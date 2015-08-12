
#include "cmdline/cmdline.h"
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
  for (auto r: reads) {
    max_id = max(max_id, r->getId());
  }

  mapped->resize(max_id + 1, nullptr);
  for (auto r: reads) {
    (*mapped)[r->getId()] = r;
  }
}

int main(int argc, char **argv) {
  cmdline::parser args;
  args.add<string>("reads", 'r', "reads file", true);
  args.add<string>("contigs", 'c', "contigs file", true);
  args.parse_check(argc, argv);

  const string reads_filename = args.get<string>("reads");
  const string contigs_filename = args.get<string>("contigs");

  vector<Contig*> contigs;
  vector<Read*> tmp_reads;
  vector<Read*> reads;

  readFastaReads(tmp_reads, reads_filename.c_str());
  map_reads(&reads, reads);

  readAfgContigs(contigs, contigs_filename.c_str());

  std::cerr << "Read " << reads.size() << " reads" << std::endl;
  std::cerr << "Read " << contigs.size() << " contigs" << std::endl;

  std::vector<Read*> transcripts;

  int id = 0;
  for (const auto& contig : contigs) {
    for (const auto& part: contig->getParts()) {
      assert(reads[part.src] != nullptr);
    }

    std::string consensusSeq = consensus(contig, reads);

    if (!consensusSeq.empty()) {
      std::cout << ">seq" << id << std::endl;
      std::cout << consensusSeq << std::endl;
      ++id;
    }
  }

  for (auto r: reads) {
    delete r;
  }

  for (auto c: contigs) {
    delete c;
  }

  return 0;
}
