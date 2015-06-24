
#include "cmdline/cmdline.h"
#include "mhap/overlap.h"
#include "mhap/parser.h"
#include "ra/include/ra/ra.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

// trimming params
int READ_LEN_THRESHOLD = 100000;

// BFS params in bubble popping
size_t MAX_NODES = 750;
int MAX_DISTANCE = 2500;
double MAX_DIFFERENCE = 0.05;

// contig extraction params
size_t MAX_BRANCHES = 12;
size_t MAX_START_NODES = 24;

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
  args.add<string>("reads_format", 's', "reads format; supported: fasta, afg", false, "fasta");
  args.add<int>("reads_id_offset", 'a', "reads id offset (first read id)", false, 0);
  args.add<string>("overlaps", 'x', "overlaps file", true);
  args.add<bool>("verbose", 'v', "verbose output", false);
  args.add<string>("overlaps_format", 'f', "overlaps file format; supported: afg, mhap", false, "afg");
  args.parse_check(argc, argv);

  const int thread_num = std::max(std::thread::hardware_concurrency(), 1U);
  const string reads_filename = args.get<string>("reads");
  const string reads_format = args.get<string>("reads_format");
  const string overlaps_filename = args.get<string>("overlaps");
  const string overlaps_format = args.get<string>("overlaps_format");
  const bool verbose_output = args.get<bool>("verbose");
  const int reads_id_offset = args.get<int>("read_id_offset");

  vector<Overlap*> overlaps, filtered;
  vector<Read*> reads;
  vector<Read*> reads_mapped;

  if (reads_format == "fasta") {
    readAfgReads(reads, reads_filename.c_str());
  } else if (reads_format == "afg") {
    readFastaReads(reads, reads_filename.c_str());
  } else {
    assert(false);
  }

  // map reads so we have reads_mapped[read_id] -> read
  map_reads(&reads_mapped, reads);

  std::cerr << "Read " << reads.size() << " reads" << std::endl;

  if (reads_format == "afg") {
    readAfgOverlaps(overlaps, overlaps_filename.c_str());
  } else if (reads_format == "mhap") {
    fstream overlaps_file(overlaps_filename);
    MHAP::read_overlaps(overlaps_file, &overlaps);
    overlaps_file.close();
  }

  // fix overlap read ids
  for (auto o: overlaps) {
    o->setA(o->getA() - reads_id_offset);
    o->setB(o->getB() - reads_id_offset);
  }

  for (auto o: overlaps) {
    const auto a = o->getA();
    const auto b = o->getB();
    assert(reads_mapped[a] != nullptr);
    assert(reads_mapped[b] != nullptr);
  }

  vector<Overlap*> nocontainments;
  filterContainedOverlaps(nocontainments, overlaps, reads_mapped, true);

  {
    int filtered =  overlaps.size() - nocontainments.size();
    cerr << "Removed " << filtered << " overlaps as contained "
      << "(" << (1.*filtered)/overlaps.size() << ")" << endl;
  }

  if (verbose_output) {
    writeAfgOverlaps(nocontainments, "nocont.afg");
  }

  vector<Overlap*> notransitives;
  filterTransitiveOverlaps(notransitives, nocontainments, thread_num, true);

  {
    int filtered = nocontainments.size() - notransitives.size();
    cerr << "Removed " << filtered << " overlaps as transitive "
      << "(" << (1.*filtered)/nocontainments.size() << ")" << endl;
  }

  if (verbose_output) {
    writeAfgOverlaps(notransitives, "nocont.notran.afg");
  }

  createReverseComplements(reads, thread_num);

  StringGraph* graph = new StringGraph(reads, notransitives);
  graph->simplify();

  std::vector<StringGraphComponent*> components;
  graph->extractComponents(components);

  std::vector<Contig*> contigs;

  for (const auto& component : components) {

    Contig* contig = component->createContig();

    if (contig != nullptr) {
      contigs.emplace_back(contig);
    }
  }

  std::cerr << "number of contigs " << contigs.size() << std::endl;

  writeAfgContigs(contigs, "contigs.afg");

  for (auto r: reads)      delete r;
  for (auto o: overlaps)   delete o;
  for (auto c: components) delete c;
  for (auto c: contigs)    delete c;

  delete graph;

  return 0;
}
