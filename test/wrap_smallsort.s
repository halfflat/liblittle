	.file	"wrap_smallsort.cc"
# GNU C++11 (GCC) version 6.2.0 (x86_64-pc-linux-gnu)
#	compiled by GNU C version 6.2.0, GMP version 6.1.1, MPFR version 3.1.3-p4, MPC version 1.0.3, isl version 0.14 or 0.13
# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -I ../include -D_GNU_SOURCE -D N=3 -D TYPE=float
# -D _FORTIFY_SOURCE=2
# -idirafter /nix/store/idwrh4bm5s4lnnb03d1j2b2rqg9x42h6-glibc-2.24-dev/include
# -idirafter /nix/store/yrj49q8gi7vq50s7yy87vlbq6vdzrkyf-gcc-6.2.0/lib/gcc/x86_64-pc-linux-gnu/6.2.0/include-fixed
# wrap_smallsort.cc -march=ivybridge -mmmx -mno-3dnow -msse -msse2 -msse3
# -mssse3 -mno-sse4a -mcx16 -msahf -mno-movbe -maes -mno-sha -mpclmul
# -mpopcnt -mno-abm -mno-lwp -mno-fma -mno-fma4 -mno-xop -mno-bmi -mno-bmi2
# -mno-tbm -mavx -mno-avx2 -msse4.2 -msse4.1 -mno-lzcnt -mno-rtm -mno-hle
# -mrdrnd -mf16c -mfsgsbase -mno-rdseed -mno-prfchw -mno-adx -mfxsr -mxsave
# -mxsaveopt -mno-avx512f -mno-avx512er -mno-avx512cd -mno-avx512pf
# -mno-prefetchwt1 -mno-clflushopt -mno-xsavec -mno-xsaves -mno-avx512dq
# -mno-avx512bw -mno-avx512vl -mno-avx512ifma -mno-avx512vbmi -mno-clwb
# -mno-pcommit -mno-mwaitx -mno-clzero -mno-pku --param l1-cache-size=32
# --param l1-cache-line-size=64 --param l2-cache-size=8192 -mtune=ivybridge
# -Ofast -O2 -Wformat=1 -Wformat-security -Werror=format-security
# -std=c++11 -fverbose-asm -fstack-protector-strong -fPIC
# -fno-strict-overflow --param ssp-buffer-size=4
# options enabled:  -fPIC -faggressive-loop-optimizations -falign-labels
# -fasynchronous-unwind-tables -fauto-inc-dec -fbranch-count-reg
# -fcaller-saves -fchkp-check-incomplete-type -fchkp-check-read
# -fchkp-check-write -fchkp-instrument-calls -fchkp-narrow-bounds
# -fchkp-optimize -fchkp-store-bounds -fchkp-use-static-bounds
# -fchkp-use-static-const-bounds -fchkp-use-wrappers
# -fcombine-stack-adjustments -fcommon -fcompare-elim -fcprop-registers
# -fcrossjumping -fcse-follow-jumps -fdefer-pop
# -fdelete-null-pointer-checks -fdevirtualize -fdevirtualize-speculatively
# -fdwarf2-cfi-asm -fearly-inlining -feliminate-unused-debug-types
# -fexceptions -fexpensive-optimizations -fforward-propagate -ffunction-cse
# -fgcse -fgcse-lm -fgnu-runtime -fgnu-unique -fguess-branch-probability
# -fhoist-adjacent-loads -fident -fif-conversion -fif-conversion2
# -findirect-inlining -finline -finline-atomics
# -finline-functions-called-once -finline-small-functions -fipa-cp
# -fipa-cp-alignment -fipa-icf -fipa-icf-functions -fipa-icf-variables
# -fipa-profile -fipa-pure-const -fipa-ra -fipa-reference -fipa-sra
# -fira-hoist-pressure -fira-share-save-slots -fira-share-spill-slots
# -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
# -fleading-underscore -flifetime-dse -flra-remat -flto-odr-type-merging
# -fmath-errno -fmerge-constants -fmerge-debug-strings
# -fmove-loop-invariants -fomit-frame-pointer -foptimize-sibling-calls
# -foptimize-strlen -fpartial-inlining -fpeephole -fpeephole2 -fplt
# -fprefetch-loop-arrays -free -freg-struct-return -freorder-blocks
# -freorder-functions -frerun-cse-after-loop
# -fsched-critical-path-heuristic -fsched-dep-count-heuristic
# -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
# -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
# -fsched-stalled-insns-dep -fschedule-fusion -fschedule-insns2
# -fsemantic-interposition -fshow-column -fshrink-wrap -fsigned-zeros
# -fsplit-ivs-in-unroller -fsplit-wide-types -fssa-backprop -fssa-phiopt
# -fstack-protector-strong -fstdarg-opt -fstrict-aliasing
# -fstrict-volatile-bitfields -fsync-libcalls -fthread-jumps
# -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp -ftree-builtin-call-dce
# -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-cselim
# -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre
# -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
# -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop -ftree-pre
# -ftree-pta -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-slsr
# -ftree-sra -ftree-switch-conversion -ftree-tail-merge -ftree-ter
# -ftree-vrp -funit-at-a-time -funwind-tables -fverbose-asm
# -fzero-initialized-in-bss -m128bit-long-double -m64 -m80387 -maes
# -malign-stringops -mavx -mavx256-split-unaligned-load
# -mavx256-split-unaligned-store -mcx16 -mf16c -mfancy-math-387
# -mfp-ret-in-387 -mfsgsbase -mfxsr -mglibc -mieee-fp -mlong-double-80
# -mmmx -mpclmul -mpopcnt -mpush-args -mrdrnd -mred-zone -msahf -msse
# -msse2 -msse3 -msse4 -msse4.1 -msse4.2 -mssse3 -mstv
# -mtls-direct-seg-refs -mvzeroupper -mxsave -mxsaveopt

	.text
	.p2align 4,,15
	.globl	_Z12sort_float_3Pf
	.type	_Z12sort_float_3Pf, @function
_Z12sort_float_3Pf:
.LFB1916:
	.cfi_startproc
	vmovss	(%rdi), %xmm0	# *data_7(D), iftmp.0_8
	vmovss	4(%rdi), %xmm1	# MEM[(float &)data_7(D) + 4], iftmp.0_2
	vmovss	8(%rdi), %xmm2	# MEM[(float &)data_7(D) + 8], _13
	vucomiss	%xmm0, %xmm1	# iftmp.0_8, iftmp.0_2
	ja	.L2	#,
	vucomiss	%xmm0, %xmm2	# iftmp.0_8, _13
	jbe	.L7	#,
.L3:
	vucomiss	%xmm1, %xmm0	# iftmp.0_2, iftmp.0_8
	vmovss	%xmm2, 8(%rdi)	# _13, MEM[(float &)data_7(D) + 8]
	jbe	.L4	#,
	vmovaps	%xmm1, %xmm2	# iftmp.0_2, iftmp.0_2
	vmovaps	%xmm0, %xmm1	# iftmp.0_8, iftmp.0_2
	vmovaps	%xmm2, %xmm0	# iftmp.0_2, iftmp.0_8
.L4:
	vmovss	%xmm1, 4(%rdi)	# iftmp.0_2, MEM[(float &)data_7(D) + 4]
	vmovss	%xmm0, (%rdi)	# iftmp.0_8, *data_7(D)
	ret
	.p2align 4,,10
	.p2align 3
.L2:
	vucomiss	%xmm1, %xmm2	# iftmp.0_2, _13
	ja	.L6	#,
	vmovaps	%xmm1, %xmm3	# iftmp.0_2, iftmp.0_2
	vmovaps	%xmm0, %xmm1	# iftmp.0_8, iftmp.0_2
	vmovaps	%xmm3, %xmm0	# iftmp.0_2, iftmp.0_8
.L7:
	vmovaps	%xmm2, %xmm3	# _13, _13
	vmovaps	%xmm0, %xmm2	# iftmp.0_8, _13
	vmovaps	%xmm3, %xmm0	# _13, iftmp.0_8
	jmp	.L3	#
	.p2align 4,,10
	.p2align 3
.L6:
	vmovss	%xmm2, 8(%rdi)	# _13, MEM[(float &)data_7(D) + 8]
	vmovss	%xmm1, 4(%rdi)	# iftmp.0_2, MEM[(float &)data_7(D) + 4]
	vmovss	%xmm0, (%rdi)	# iftmp.0_8, *data_7(D)
	ret
	.cfi_endproc
.LFE1916:
	.size	_Z12sort_float_3Pf, .-_Z12sort_float_3Pf
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.type	_GLOBAL__sub_I_wrap_smallsort.cc, @function
_GLOBAL__sub_I_wrap_smallsort.cc:
.LFB2201:
	.cfi_startproc
	leaq	_ZStL8__ioinit(%rip), %rdi	#,
	subq	$8, %rsp	#,
	.cfi_def_cfa_offset 16
	call	_ZNSt8ios_base4InitC1Ev@PLT	#
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi	#,
	leaq	__dso_handle(%rip), %rdx	#,
	addq	$8, %rsp	#,
	.cfi_def_cfa_offset 8
	leaq	_ZStL8__ioinit(%rip), %rsi	#,
	jmp	__cxa_atexit@PLT	#
	.cfi_endproc
.LFE2201:
	.size	_GLOBAL__sub_I_wrap_smallsort.cc, .-_GLOBAL__sub_I_wrap_smallsort.cc
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_wrap_smallsort.cc
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 6.2.0"
	.section	.note.GNU-stack,"",@progbits
