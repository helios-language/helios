
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>
#include <helios_test.h>

#undef main // this makes it so testmain becomes main

int testmain(int argc, char *argv[]) {
<<<<<<< HEAD
    return builtins_test();
=======
    builtins_test();
    return 0;
>>>>>>> 6d1a2a1cc828dabc14660c7322a45760b2cd3d9c
}
