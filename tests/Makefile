compiler = gcc
debug = 0

all: info fixed_array1D array1D array_stable_ops error_tools


ifeq ($(compiler), gcc)
CXX = g++
CXXFLAGS = -std=gnu++20 -DSTRICT_QUAD_PRECISION
LFLAGS = -lquadmath -lm

else ifeq ($(compiler), icpx)
CXX = icpx
CXXFLAGS = -std=c++20 -fp-model=precise
LFLAGS = -lm

else ifeq ($(compiler), clang)
CXX = clang++
CXXFLAGS = -std=c++20
LFLAGS = -lm
endif


ifeq ($(debug), 1)

CXXFLAGS += -g -fsanitize=address -O0
CXXFLAGS += -Wall -Wextra -Wdeprecated -Wconversion -Wsign-conversion

ifeq ($(compiler), gcc)
else
CXXFLAGS += -pedantic
endif

else

ifeq ($(compiler), gcc)
CXXFLAGS += -march=native
else ifeq ($(compiler), icpx)
CXXFLAGS += -xHost
else ifeq ($(compiler), clang)
CXXFLAGS += -march=native
endif

CXXFLAGS += -O3
endif


info: info.cpp
	$(CXX) $(CXXFLAGS) info.cpp -o info.x $(LFLAGS)

fixed_array1D: fixed_array1D.cpp
	$(CXX) $(CXXFLAGS) fixed_array1D.cpp -o fixed_array1D.x $(LFLAGS)

array1D: array1D.cpp
	$(CXX) $(CXXFLAGS) array1D.cpp -o array1D.x $(LFLAGS)

array_stable_ops: array_stable_ops.cpp
	$(CXX) $(CXXFLAGS) array_stable_ops.cpp -o array_stable_ops.x $(LFLAGS)

error_tools: error_tools.cpp
	$(CXX) $(CXXFLAGS) error_tools.cpp -o error_tools.x $(LFLAGS)

clean:
	rm -rf *.x *.txt

