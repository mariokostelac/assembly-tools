.PHONY: lib

CC = g++
CFLAGS = -g -Wall -std=c++11 -O3 -I ./lib -I ./src -I ./
LDFLAGS = -pthread

default: bin/filter-contained bin/filter-transitive bin/overlap2dot bin/layout

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

obj/overlap2dot_main.o: src/overlap2dot/main.cpp
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

clean:
	@rm -rf bin
	@rm -rf obj
