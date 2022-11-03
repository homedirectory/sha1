#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

int main(char argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s message\n", argv[0]);
        exit(1);
    }

    uchar* msg = argv[1];
    uchar hash[20];
    sha1(msg, strlen(msg), hash);
    for (int i = 0; i < 20; i++)
        printf("%02x", hash[i]);
    puts("");
}
