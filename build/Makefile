.PHONY: clean all realclean test bench

tests:=test_comparator test_tinysort test_multiset test_map
benches:=bench_tinysort bench_multiset

top=..
sources:=$(wildcard $(top)/test/*.cc) $(wildcard $(top)/bench/*.cc)

# by default: build all unit tests and benchmarks

all:: $(tests) $(benches)

clean:
	rm -f gtest-all.o gtest_main.o obj/*.o obj/gbench/*.o obj/gtest/*.o

realclean: clean
	rm -f $(tests) $(benches) libbenchmark.a libgtestmain.a obj/*.d obj/gbench/*.d obj/gtest/*.d

# run all unit tests with `test` target:

test: $(tests)
	for test in $^; do ./$$test; done

# generate (filtered) assembly for a given tiny::sort instantation to stdout
# (run make with -s to avoid output of recipe text)

tinysort-asm-%: tinysort-%.s
	@perl filter-asm.pl "$<"

# compiler flags:
# `-pthread` required for googletest; `-MMD -MP` for automatic dependency generation

OPTFLAGS?=-O3 -march=native
CPPFLAGS?=-DHF_USE_ASM_KERNELS

CXXFLAGS:=-std=c++11 -g -pthread $(OPTFLAGS) -MMD -MP

vpath %.h $(top)/include
vpath test% $(top)/test
vpath bench% $(top)/bench

$(shell mkdir -p obj obj/gbench obj/gtest)

define cxx-compile
$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<
endef

define cxx-link
$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
endef

define lib-link
$(AR) $(ARFLAGS) $@ $^
endef

# external libraries: gtest, gbench

GTEST_DIR=$(top)/external/gtest/googletest
GBENCH_DIR=$(top)/external/gbench

# build gtest

gtest_sources=$(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc

obj/gtest/%.o: CPPFLAGS+=$(GTEST_INCLUDES) -I $(GTEST_DIR)
obj/gtest/%.o: $(GTEST_DIR)/src/%.cc
	$(cxx-compile)

libgtestmain.a: $(patsubst %.cc,obj/gtest/%.o,$(notdir $(gtest_sources)))
	$(lib-link)

GTEST_INCLUDES:=-isystem $(GTEST_DIR)/include

# build gbench

gbench_sources=$(wildcard $(GBENCH_DIR)/src/*.cc)

obj/gbench/%.o: CPPFLAGS+=$(GBENCH_INCLUDES) -DHAVE_STD_REGEX -DNDEBUG
obj/gbench/%.o: $(GBENCH_DIR)/src/%.cc
	$(cxx-compile)

libbenchmark.a: $(patsubst %.cc,obj/gbench/%.o,$(notdir $(gbench_sources)))
	$(lib-link)

GBENCH_INCLUDES:=-isystem $(GBENCH_DIR)/include

# liblittle unit tests

obj/test%.o: CPPFLAGS+=$(GTEST_INCLUDES) -I$(top)
obj/test%.o: test%.cc
	$(cxx-compile)

test%: LDFLAGS+=-L.
test%: LDLIBS+=-lgtestmain
test%: obj/test%.o libgtestmain.a
	$(cxx-link)

.PRECIOUS:: test%

# build benchmark

obj/bench%.o: CPPFLAGS+=-I$(top) $(GBENCH_INCLUDES)
obj/bench%.o: bench%.cc
	$(cxx-compile)

bench%: LDFLAGS+=-L.
bench%: LDLIBS+=-lbenchmark
bench%: obj/bench%.o libbenchmark.a
	$(cxx-link)

.PRECIOUS:: bench%

# generate and inspect assembly for tiny sort implementation:
# build tinysort-<type><count>.s

tinysort-%.s: CPPFLAGS+=-I$(top)
tinysort-%.s: CXXFLAGS:=-std=c++11 $(OPTFLAGS)
tinysort-%.s: tinysort-%.cc
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -S -o $@ $^

tinysort-%.cc: mk-tinysort-wrap.pl
	@perl mk-tinysort-wrap.pl "$*" > $@

# include automatically generated dependencies
-include $(patsubst %.cc,obj/%.d,$(notdir $(sources)))
-include $(patsubst %.cc,obj/gtest/%.d,$(notdir $(gtest_sources)))
-include $(patsubst %.cc,obj/gbench/%.d,$(notdir $(gbench_sources)))

