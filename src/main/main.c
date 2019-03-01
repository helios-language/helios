
#include <stdio.h>

#include <tokenizer.h>

int main(int argc, char* argv[]) {
    tokenstream_t* t = tokenizer_tokenize("\"hello world\" 10 100 abc a");
    tokenstream_print(t);
    tokenstream_free(t);
}
