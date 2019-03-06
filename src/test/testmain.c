
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <test.h>

#undef main


int main(int argc, char * argv[]){
    for(uint32_t i = 0; i < argc; i++){
        if(strcmp(argv[i],"--test")==0 || strcmp(argv[i],"-t")==0){
            return testmain(argc,argv);
        }
    }
    return __RUN_MAIN(argc,argv);
}



