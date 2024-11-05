compiler = gcc

all: array1D fixedarray1D

CXXFLAGS = -std=c++20 -O3 -no-pie
LFLAGS = -lm -lbenchmark -lpthread
IPATH = -I ../src/

ifeq ($(compiler), gcc)
CXX = g++
CXXFLAGS += -march=native
exe1 = array_gcc.x
exe2 = fixedarray_gcc.x

else ifeq ($(compiler), icpx)
CXX = icpx
CXXFLAGS += -xHost -fp-model=precise #-xcore-avx512 -fno-alias
exe1 = array_icpx.x
exe2 = fixedarray_icpx.x

else ifeq ($(compiler), clang)
CXX = clang++
CXXFLAGS += -march=native
exe1 = array_clang.x
exe2 = fixedarray_clang.x
endif
CXXFLAGS += -DSTRICT_DEBUG_OFF -DEIGEN_NO_DEBUG

array1D: array1D_bm.cpp
	$(CXX) $(CXXFLAGS) array1D_bm.cpp -o $(exe1) $(LFLAGS) $(IPATH)

fixedarray1D: fixedarray1D_bm.cpp
	$(CXX) $(CXXFLAGS) fixedarray1D_bm.cpp -o $(exe2) $(LFLAGS) $(IPATH)

clean:
	rm -rf *.x
