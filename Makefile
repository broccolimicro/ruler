CXXFLAGS     = -g -O2 -Wall -fmessage-length=0 -I. -L. -Ideps/gdstk/include
# -g -fprofile-arcs -ftest-coverage
BSOURCES     := $(wildcard src/*.cpp)
LSOURCES     := $(wildcard ruler/*.cpp)
LOBJECTS     := $(LSOURCES:.cpp=.o)
BOBJECTS     := $(BSOURCES:.cpp=.o)
LDEPS        := $(LSOURCES:.cpp=.d)
BDEPS        := $(BSOURCES:.cpp=.d)
LTARGET      = libruler.a
BTARGET      = ruler-linux

all: lib $(BTARGET)

lib: $(LTARGET)

test: lib $(BTARGET) $(TTARGET)

check: test
	./$(TTARGET)

$(LTARGET): $(LOBJECTS)
	ar rvs $(LTARGET) $(LOBJECTS)

$(BTARGET): $(BOBJECTS) $(LTARGET)
	$(CXX) $(CXXFLAGS) $(BOBJECTS) -l:$(LTARGET) -o $(BTARGET)

ruler/%.o: ruler/%.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

test/gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(CXX) $(CXXFLAGS) $(GTEST_I) $< -c -o $@

-include $(LDEPS)
-include $(BDEPS)
-include $(TDEPS)

clean:
	rm -f src/*.o ruler/*.o
	rm -f src/*.d ruler/*.d
	rm -f $(LTARGET) $(BTARGET)
