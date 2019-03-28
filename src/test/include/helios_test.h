
#ifndef HELIOS_TEST
#define HELIOS_TEST

#include <builtins_test.h>
#include <parser_test.h>

#ifdef TEST
#define main __real_main
#define testmain main
#endif

#endif
