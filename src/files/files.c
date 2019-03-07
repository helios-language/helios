#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>

char* readfiletostring(char* filename){
    FILE * file = fopen(filename,"r");
    char * buffer;
    if (file){
        fseek (file, 0, SEEK_END);
        uint32_t length = ftell (file);
        fseek (file, 0, SEEK_SET);
        buffer = malloc (length+1);
        if (buffer){
            fread (buffer, 1, length, file);
            buffer[length] = '\n';
        }else{
            printf("allocation error at %s line %i\n",__FILE__,__LINE__);        
            exit(-1);
        }
        fclose (file);
    }else{
        printf("could not find '%s'.\n");
        exit(-1);
    }

    return buffer;
}



