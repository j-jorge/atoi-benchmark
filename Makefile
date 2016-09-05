CFLAGS := -O3 -DNDEBUG
CXXFLAGS := $(CFLAGS) -std=c++11
RUNS ?= 1000000

BUILD ?= $(shell [ -f build/last_build_type ] && cat build/last_build_type)

CMAKE_BUILD_TYPE := 

ifeq ($(BUILD),release)
  CMAKE_BUILD_TYPE := release
  BUILD_DIR := build/release
else ifeq ($(BUILD),profile)
  CMAKE_BUILD_TYPE := release
  CXXFLAGS += -pg
  BUILD_DIR := build/profile
else
  CMAKE_BUILD_TYPE := debug
  BUILD_DIR := build/debug
endif

CMAKE_ARGS := -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)

benchmark: .cmake
	cd $(BUILD_DIR) && $(MAKE)

.cmake: .build_marker
	cd $(BUILD_DIR) \
	  && CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" \
	  cmake ../.. $(CMAKE_ARGS)

.build_marker: $(BUILD_DIR)
	echo $(BUILD) > build/last_build_type

$(BUILD_DIR):
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

plot: benchmark
	mkdir -p $(BUILD_DIR)/benchmark-plot/output
	cd $(BUILD_DIR)/benchmark-plot && ../benchmark $(RUNS)
	cd $(BUILD_DIR)/benchmark-plot/output; echo 'load "atoi.plot"' | gnuplot
	mkdir -p $(BUILD_DIR)/all-in-one-plot/output
	cd $(BUILD_DIR)/all-in-one-plot && ../all-in-one-benchmark $(RUNS)
	cd $(BUILD_DIR)/all-in-one-plot/output; echo 'load "atoi.plot"' \
	  | gnuplot

clear:
	rm -f -r output bin
	rm -fr build
