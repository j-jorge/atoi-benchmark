CXX := g++
CXXFLAGS := -O3 -DNDEBUG -std=c++11
RUNS := 1000000

bin/benchmark: src/benchmark.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^

plot: bin/benchmark
	mkdir -p output
	bin/benchmark $(RUNS)
	cd output; echo 'load "atoi.plot"' | gnuplot

clear:
	rm -f -r output bin
