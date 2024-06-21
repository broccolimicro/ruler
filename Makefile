PYTHON_RELEASE = $(shell python3 -c "import sys;t='{v[0]}.{v[1]}'.format(v=list(sys.version_info[:2]));sys.stdout.write(t)")

CXXFLAGS     = -g -O2 -Wall -fmessage-length=0 -I. -L. -Ideps/gdstk/include -I/usr/include/python$(PYTHON_RELEASE) -Ideps/pgen -Ldeps/pgen
# -g -fprofile-arcs -ftest-coverage
BSOURCES     := $(wildcard src/*.cpp)
PGRAM        := $(wildcard peg/*.peg)
PSOURCES     := $(PGRAM:peg/%.peg=ruler/%.cpp)
LSOURCES     := $(wildcard ruler/*.cpp) $(PSOURCES)
LOBJECTS     := $(LSOURCES:.cpp=.o)
BOBJECTS     := $(BSOURCES:.cpp=.o)
LDEPS        := $(LSOURCES:.cpp=.d)
BDEPS        := $(BSOURCES:.cpp=.d)
LTARGET      = libruler.a
BTARGET      = ruler-linux

all: deps grammar lib $(BTARGET)

deps: pgen

pgen:
	$(MAKE) -s $(MAKE_FLAGS) -C deps/pgen

grammar: $(PSOURCES)

ruler/conf.cpp: peg/conf.peg
	deps/pgen/pgen-linux $<
	mv peg/*.cpp peg/*.h ruler

lib: $(LTARGET)

test: lib $(BTARGET) $(TTARGET)

check: test
	./$(TTARGET)

$(LTARGET): $(LOBJECTS)
	ar rvs $(LTARGET) $(LOBJECTS)

$(BTARGET): $(BOBJECTS) $(LTARGET)
	$(CXX) $(CXXFLAGS) $(BOBJECTS) -l:$(LTARGET) -l:libpgen.a -o $(BTARGET)

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
	$(MAKE) -s $(MAKE_FLAGS) -C deps/pgen clean
	rm -f src/*.o ruler/*.o
	rm -f src/*.d ruler/*.d
	rm -f $(LTARGET) $(BTARGET)
