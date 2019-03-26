#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *readfiletostring(char *filename) {
    FILE *file = fopen(filename, "r");
    char *buffer;
    if (file) {
        fseek(file, 0, SEEK_END);
        uint32_t length = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = calloc(length + 2, 1);
        if (buffer) {
            fread(buffer, 1, length, file);
            buffer[length] = '\n';
        } else {
            printf("allocation error at %s line %i\n", __FILE__, __LINE__);
            exit(-1);
        }
        fclose(file);
    } else {
        printf("could not find '%s'.\n", filename);
        exit(-1);
    }

    return buffer;
}

// void writeprogramtofile(char *filename, program *prgm) {
//     FILE *file = fopen(filename, "wb");
//     fprintf(file, "%04" PRIx16, prgm->magic);
//     fprintf(file, "%016" PRIx64, prgm->instructionfilled);

//     for (uint64_t i = 0; i < prgm->instructionfilled; i++) {
//         fprintf(file, "%04" PRIx16, (uint16_t)prgm->instructions[i].opcode);
//         fprintf(file, "%016" PRIx64,
//         (uint64_t)prgm->instructions[i].argument);
//     }

//     for (uint64_t i = 0; i < prgm->constantfilled; i++) {
//         fprintf(file, "%08" PRIx32, (uint32_t)prgm->constants[i].size);
//         for (uint32_t j = 0; j < prgm->constants[i].size; j++) {
//             fprintf(file, "%02" PRIx8,
//             (uint8_t)(prgm->constants[i].bytes[j]));
//         }
//     }
// }
