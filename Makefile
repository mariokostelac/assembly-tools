filter-contained:
	@echo "Preparing bin folder..."
	@mkdir -p bin
	@echo "Compiling..."
	@g++ -O2 -std=c++11 src/filter_contained.cpp src/amos/reader.cpp src/amos/overlap.cpp -o bin/filter-contained

clean:
	@rm -rf bin
