.PHONY: clean all realclean test bench

tests:=test_smallsort

all: $(tests)

clean:
	rm -f gtest-all.o gtest_main.o

realclean: clean
	rm -f $(tests) libgtestmain.a

CXXFLAGS=-std=c++11 -g -Ofast -march=native -pthread

# directories

top=..
GTEST_DIR=$(top)/gtest-1.7.0

vpath %.h $(top)/include
vpath test% $(top)/test
vpath bench% $(top)/test

# gtest includes

CPPFLAGS=-isystem $(GTEST_DIR)/include

# build gtest

GTEST_HEADERS=$(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SOURCES=$(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o: CPPFLAGS+=-I$(GTEST_DIR)
gtest-all.o: $(GTEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o: CPPFLAGS+=-I$(GTEST_DIR)
gtest_main.o: $(GTEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $(GTEST_DIR)/src/gtest_main.cc

libgtestmain.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# build tests

test_smallsort: CPPFLAGS+=-I$(top)/include
test_smallsort: LDLIBS+=-L. -lgtestmain
test_smallsort: ../include/smallsort.h libgtestmain.a
test_smallsort: test_smallsort.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) 


# run tests

test: test_smallsort
	for test in $^; do ./$$test; done


# build benchmark

bench_smallsort: CPPFLAGS+=-I$(top)/include
bench_smallsort: ../include/smallsort.h
bench_smallsort: bench_smallsort.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) 


# run benchmarks
bench: bench_smallsort
	perf stat ./bench_smallsort array3d
	perf stat ./bench_smallsort array4d
	perf stat ./bench_smallsort array25f
	perf stat ./bench_smallsort vec3d
	perf stat ./bench_smallsort vec8f

