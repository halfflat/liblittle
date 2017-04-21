.PHONY: clean all realclean test bench

tests:=test_smallsort test_tiny_multiset test_tiny_map
benches:=bench_smallsort

top=..
sources:=$(wildcard $(top)/test/*.cc) $(wildcard $(top)/bench/*.cc)

all: $(tests) $(benches)

clean:
	rm -f gtest-all.o gtest_main.o obj/*.o obj/gbench/*.o obj/gtest/*.o

realclean: clean
	rm -f $(tests) $(benches) libbenchmark.a libgtestmain.a obj/*.d obj/gbench/*.d obj/gtest/*.d

OPTFLAGS=-Ofast -march=native

CXXFLAGS=-std=c++11 -g -pthread $(OPTFLAGS) -MMD -MP

# directories

GTEST_DIR=$(top)/external/gtest/googletest
GBENCH_DIR=$(top)/external/gbench

vpath %.h $(top)/include
vpath test% $(top)/test
vpath bench% $(top)/bench

$(shell mkdir -p obj obj/gbench obj/gtest)

# external libraries: gtest, gbench

# build gtest

gtest_sources=$(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc

GTEST_INCLUDES:=-isystem $(GTEST_DIR)/include
GTEST_CPPFLAGS:=$(GTEST_INCLUDES) -I $(GTEST_DIR)

obj/gtest/%.o: $(GTEST_DIR)/src/%.cc
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $(GTEST_CPPFLAGS) -o $@ -c $<

libgtestmain.a: $(patsubst %.cc,obj/gtest/%.o,$(notdir $(gtest_sources)))
	$(AR) $(ARFLAGS) $@ $^

# build gbench

gbench_sources=$(wildcard $(GBENCH_DIR)/src/*.cc)

GBENCH_INCLUDES:=-isystem $(GBENCH_DIR)/include
GBENCH_CPPFLAGS:=$(GBENCH_INCLUDES) -DHAVE_STD_REGEX -DNDEBUG

obj/gbench/%.o: $(GBENCH_DIR)/src/%.cc
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $(GBENCH_CPPFLAGS) -o $@ -c $^

libbenchmark.a: $(patsubst %.cc,obj/gbench/%.o,$(notdir $(gbench_sources)))
	$(AR) $(ARFLAGS) $@ $^

# build tests

TEST_CPPFLAGS:=$(GTEST_INCLUDES) -I$(top)/incluide
TEST_LDLIBS:=$(LDLIBS) -L. -lgtestmain

obj/test%.o: test%.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -I$(top)/include $(GTEST_INCLUDES) -c -o $@ $<

test%: obj/test%.o libgtestmain.a
	$(CXX) $(CXXFLAGS) $(TEST_CPPFLAGS) -o $@ $< $(LDFLAGS) $(TEST_LDLIBS)

.PRECIOUS: test% bench%

# run tests

test: $(tests)
	for test in $^; do ./$$test; done


# build benchmark

BENCH_CPPFLAGS:=$(GBENCH_INCLUDES) -I$(top)/incluide
BENCH_LDLIBS:=$(LDLIBS) -L. -lbenchmark

obj/bench%.o: bench%.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -I$(top)/include $(GBENCH_INCLUDES) -c -o $@ $<

bench%: obj/bench%.o libbenchmark.a
	$(CXX) $(CXXFLAGS) $(BENCH_CPPFLAGS) -o $@ $< $(LDFLAGS) $(BENCH_LDLIBS)

#
## run benchmarks
#bench: $(benches)
#	./bench_smallsort
#

# include automatically generated dependencies
-include $(patsubst %.cc,obj/%.d,$(notdir $(sources)))
-include $(patsubst %.cc,obj/gtest/%.d,$(notdir $(gtest_sources)))
-include $(patsubst %.cc,obj/gbench/%.d,$(notdir $(gbench_sources)))

