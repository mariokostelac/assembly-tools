remove-containments:
	@echo "Preparing bin folder..."
	@mkdir -p bin
	@echo "Compiling..."
	@g++ -O2 -std=c++11 src/remove_containments.cpp src/amos/reader.cpp src/amos/overlap.cpp -o bin/remove-containments
