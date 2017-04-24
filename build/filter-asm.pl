#!/usr/bin/env perl

use warnings;
use strict;

# Extract first function (delineated by .cfi_startproc, .cfi_endproc)
# from gas assembly listing, stripping out '#' comments and .size directives.

my $emit = 0;
while (<>) {
    if ($emit) {
	last if /\.cfi_endproc/;
	next if /\.size/;
	next if /^\s*#/;
	next if /^\s*$/;
	print;
    }
    else {
        $emit = 1 if /\.cfi_startproc/;
    }
}
