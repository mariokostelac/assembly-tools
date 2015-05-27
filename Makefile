CC = g++
CFLAGS = -g -Wall -std=c++11 -O2 -I ./lib -I ./src -I ./
LDFLAGS=

default: bin/filter-contained bin/filter-transitive bin/overlap2dot

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

obj/filter_transitive.o: src/filter-transitive/filter_transitive.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

obj/filter_transitive_main.o: src/filter-transitive/main.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

bin/filter-contained: obj/filter_contained.o obj/filter_contained_main.o obj/afgreader/reader.o obj/afgreader/overlap.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

bin/filter-transitive: obj/filter_transitive.o obj/filter_transitive_main.o obj/afgreader/reader.o obj/afgreader/overlap.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

obj/overlap2dot_main.o: src/overlap2dot/overlap2dot.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $^

bin/overlap2dot: obj/overlap2dot_main.o obj/afgreader/overlap.o obj/afgreader/reader.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	@rm -rf bin
	@rm -rf obj
