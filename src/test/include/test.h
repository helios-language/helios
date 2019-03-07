
#ifndef TEST_H
#define TEST_H

/*
    This header moves the main function from main to here, so the main function is ran when -t is not provided, but
    testmain is ran when -t is provided. A more elegant solution is needed like a proper argument parser for the main
    function.
*/

int __RUN_MAIN(int argc, char *argv[]);
#define main __RUN_MAIN

int testmain(int argc, char *argv[]);

#endif
