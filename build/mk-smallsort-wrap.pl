#!/usr/bin/env perl

use warnings;
use strict;

# Generate a stand-alone function wrapping a smallsort instance,
# for asm inspection.
#
# Usage: mk-smallsort-wrap.pl KEY
# 
# where KEY is a single word of the form <TYPE><N>, requesting that
# a sort is generated for N elements of type TYPE. Spaces in TYPE
# can be represented with a minus symbol for compatibility with
# make targets.
#
# Example:
# % mk-smallsort-wrap.pl int3

my $key = shift;
$key=~s/-/ /g;

my ($type,$n) = ($key=~/(.*[^\d])(\d+)$/);

$key=~s/\*/ptr/g;
$key=~s/\&/ref/g;
$key=~s/[:\s]/_/g;

print << "__end";
#include "smallsort.h"
void smallsort_${key}(${type}* a) {
    hf::smallsort_inplace<$n>(a);
}
__end
