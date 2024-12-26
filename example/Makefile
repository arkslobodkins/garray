debug = 0

all: example0 example1 example2 example3 example4 example5 example6

CXX = g++

CXXFLAGS1 = -std=c++20 -Wconversion -Wnarrowing
LFLAGS1 = -lm

CXXFLAGS2 = -std=gnu++20 -Wconversion -Wnarrowing -DSTRICT_QUAD_PRECISION
LFLAGS2 = -lm -lquadmath

IPATH = -I ../src/

ifeq ($(debug), 0)
CXXFLAGS1 += -DSTRICT_DEBUG_OFF
CXXFLAGS2 += -O3 -DSTRICT_DEBUG_OFF
exe0 = example0.x
exe1 = example1.x
exe2 = example2.x
exe3 = example3.x
exe4 = example4.x
exe5 = example5.x
exe6 = example6.x
endif

ifeq ($(debug), 1)
CXXFLAGS1 += -O0 -g -fsanitize=address
CXXFLAGS2 += -O0 -g -fsanitize=address
exe0 = example0_debug.x
exe1 = example1_debug.x
exe2 = example2_debug.x
exe3 = example3_debug.x
exe4 = example4_debug.x
exe5 = example5_debug.x
exe6 = example6_debug.x
endif

example0: example0.cpp
	$(CXX) $(CXXFLAGS1) example0.cpp -o $(exe0) $(LFLAGS1) $(IPATH)

example1: example1.cpp
	$(CXX) $(CXXFLAGS1) example1.cpp -o $(exe1) $(LFLAGS1) $(IPATH)

example2: example2.cpp
	$(CXX) $(CXXFLAGS1) example2.cpp -o $(exe2) $(LFLAGS1) $(IPATH)

example3: example3.cpp
	$(CXX) $(CXXFLAGS2) example3.cpp -o $(exe3) $(LFLAGS2) $(IPATH)

example4: example4.cpp
	$(CXX) $(CXXFLAGS2) example4.cpp -o $(exe4) $(LFLAGS2) $(IPATH)

example5: example5.cpp
	$(CXX) $(CXXFLAGS2) example5.cpp -o $(exe5) $(LFLAGS2) $(IPATH)

example6: example6.cpp
	$(CXX) $(CXXFLAGS2) example6.cpp -o $(exe6) $(LFLAGS2) $(IPATH)

clean:
	rm -rf *.x *.txt
