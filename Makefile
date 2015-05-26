CC = g++
CFLAGS = -g -Wall -std=c++11 -O2 -I ./lib -I ./src -I ./
LDFLAGS=

default: bin/filter-contained

prepare:

obj/afgreader/reader.o: lib/afgreader/src/reader.cpp
	mkdir -p obj/afgreader
	$(CC) $(CFLAGS) -c $^ -o $@

obj/afgreader/overlap.o: lib/afgreader/src/overlap.cpp
	mkdir -p obj/afgreader
	$(CC) $(CFLAGS) -c $^ -o $@

obj/filter_contained.o: src/filter-contained/filter_contained.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

obj/filter_contained_main.o: src/filter-contained/main.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

bin/filter-contained: obj/filter_contained.o obj/filter_contained_main.o obj/afgreader/reader.o obj/afgreader/overlap.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	@rm -rf bin
	@rm -rf obj
