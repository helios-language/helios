
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>
#include <helios_test.h>

#undef main // this makes it so testmain becomes main

int testmain(int argc, char *argv[]) {

    return builtins_test();

}
