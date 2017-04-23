// workarounds for old gcc standard libraries


#if defined(__GNUC__) && __GNUC__<5
#define is_trivially_copyable has_trivial_copy_constructor
#endif

