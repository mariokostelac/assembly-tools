.PHONY: lib get-deps

CXXFLAGS = -Wall -std=c++0x -O3 -I ./lib -I ./src -I ./
LDFLAGS = -pthread

UNAME_S := $(shell uname -s)
versions := 0

ifeq ($(UNAME_S),Darwin)
	compiler := $(shell g++-4.8 --version 2>/dev/null)
	ifdef compiler
		CXX = g++-4.8
	endif

	compiler := $(shell g++-4.9 --version 2>/dev/null)
	ifdef compiler
		CXX = g++-4.9
	endif
endif

DEBUG := $(shell env | grep DEBUG | cut -d= -f2)
ifeq ($(DEBUG),1)
	CXXFLAGS += -g
	CXXFLAGS += -DDEBUG
endif

GIT_VERSION := $(shell git describe --dirty --always --tags)
CXXFLAGS += -DVERSION=\"$(GIT_VERSION)\"

all: lib bin/filter-contained bin/filter-transitive bin/overlap2dot bin/layout bin/consensus bin/zoom

get-deps:
	@echo "getting deps..."
	@git submodule update --init

lib:
	make -C lib/ra

obj/filter_contained_main.o: src/filter-contained/main.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c -o $@ $^

obj/filter_transitive_main.o: src/filter-transitive/main.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c -o $@ $^

obj/mhap/overlap.o: src/mhap/overlap.cpp
	mkdir -p obj/mhap
	$(CXX) $(CXXFLAGS) -c -o $@ $^

obj/mhap/parser.o: src/mhap/parser.cpp
	mkdir -p obj/mhap
	$(CXX) $(CXXFLAGS) -c -o $@ $^

obj/overlap2dot.o: src/overlap2dot/overlap2dot.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c -o $@ $^

obj/layout_main.o: src/layout/main.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c -o $@ $^

obj/consensus_main.o: src/consensus/main.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c -o $@ $^

obj/overlap2dot_main.o: src/overlap2dot/main.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c -o $@ $^

obj/zoom_main.o: src/zoom/main.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c -o $@ $^

bin/filter-contained: obj/filter_contained_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/filter-transitive: obj/filter_transitive_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/overlap2dot: obj/overlap2dot.o obj/overlap2dot_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/layout: obj/layout_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/consensus: obj/consensus_main.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/zoom: obj/zoom_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	make -C lib/ra clean
	@rm -rf bin
	@rm -rf obj
