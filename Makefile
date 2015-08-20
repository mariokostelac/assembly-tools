.PHONY: lib

CC = g++
CFLAGS = -Wall -std=c++11 -O3 -I ./lib -I ./src -I ./
LDFLAGS = -pthread

UNAME_S := $(shell uname -s)
versions := 0

ifeq ($(UNAME_S),Darwin)
	compiler := $(shell g++-4.8 --version 2>/dev/null)
	ifdef compiler
		CC = g++-4.8
	endif

	compiler := $(shell g++-4.9 --version 2>/dev/null)
	ifdef compiler
		CC = g++-4.9
	endif
endif

DEBUG := $(shell env | grep DEBUG | cut -d= -f2)
ifeq ($(DEBUG),1)
	CFLAGS += -g
	CFLAGS += -DDEBUG
endif

all: lib bin/filter-contained bin/filter-transitive bin/overlap2dot bin/layout bin/consensus bin/zoom

lib:
	make -C lib/ra

obj/filter_contained_main.o: src/filter-contained/main.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

obj/filter_transitive_main.o: src/filter-transitive/main.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

obj/mhap/overlap.o: src/mhap/overlap.cpp
	mkdir -p obj/mhap
	$(CC) $(CFLAGS) -c -o $@ $^

obj/mhap/parser.o: src/mhap/parser.cpp
	mkdir -p obj/mhap
	$(CC) $(CFLAGS) -c -o $@ $^

obj/overlap2dot.o: src/overlap2dot/overlap2dot.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

obj/layout_main.o: src/layout/main.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

obj/consensus_main.o: src/consensus/main.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

obj/overlap2dot_main.o: src/overlap2dot/main.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

obj/zoom_main.o: src/zoom/main.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

bin/filter-contained: obj/filter_contained_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CC) -o $@ $^ $(LDFLAGS)

bin/filter-transitive: obj/filter_transitive_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CC) -o $@ $^ $(LDFLAGS)

bin/overlap2dot: obj/overlap2dot.o obj/overlap2dot_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CC) -o $@ $^ $(LDFLAGS)

bin/layout: obj/layout_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CC) -o $@ $^ $(LDFLAGS)

bin/consensus: obj/consensus_main.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CC) -o $@ $^ $(LDFLAGS)

bin/zoom: obj/zoom_main.o obj/mhap/parser.o obj/mhap/overlap.o lib/ra/lib/libra.a
	mkdir -p bin
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	make -C lib/ra clean
	@rm -rf bin
	@rm -rf obj
