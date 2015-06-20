CC = g++
CFLAGS = -g -Wall -std=c++11 -O2 -I ./lib -I ./src -I ./
LDFLAGS=

default: bin/filter-contained bin/filter-transitive bin/overlap2dot

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

obj/overlap2dot_main.o: src/overlap2dot/main.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

bin/filter-contained: lib/ra/lib/libra.a obj/filter_contained_main.o obj/mhap/parser.o obj/mhap/overlap.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

bin/filter-transitive: lib/ra/lib/libra.a obj/filter_transitive_main.o obj/mhap/parser.o obj/mhap/overlap.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

bin/overlap2dot: lib/ra/lib/libra.a obj/overlap2dot.o obj/overlap2dot_main.o obj/mhap/parser.o obj/mhap/overlap.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	@rm -rf bin
	@rm -rf obj
