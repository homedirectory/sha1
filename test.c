#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

#define ARR_LEN(x) (sizeof((x)) / sizeof((x)[0]))

typedef struct _TestPair {
    char* msg;
    char* hash;
} TestPair;

void test_sha1() {
    TestPair cases[7] = {
        {
            "abc",
            "a9993e364706816aba3e25717850c26c9cd0d89d"
        },
        {
            "",
            "da39a3ee5e6b4b0d3255bfef95601890afd80709"
        },
        {   // From wikipedia
            "The quick brown fox jumps over the lazy dog",
            "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"
        },
        {
            "The quick brown fox jumps over the lazy cog",
            "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3"
        },
        { // exactly 512 bits
            "0123456701234567012345670123456701234567012345670123456701234567",
            "e0c094e867ef46c350ef54a7f59dd60bed92ae83"
        },
        { // exceeding 512 bits
            "Jaska ajaa allaskaljaa. Jaskalla jalalla kaljaa. Kassalla jalka, jalalla kassa. Lakas kalja.",
            "9cd84ad78816c6c39fbed822ae8188fd8e6afd11"
        },
        { // multiple of 512
            "hoaxHOAXhoaxHOAXadjfdnas324329vg0c9xHOAXhoaxHOAXhoaxHOAXhoaxHOAX"
            "JKL$#N$LK@s49324n23m4vs:DFJ:DSJFSDRTNER:SNFxHOAXhoaxHOAXhoaxHOAX"
            "hoaxHOAXhdsflk;sajkj3:$#KoaxHOAXhoaxHOAXhoaxHOAXhoaxHOAXhoaxHOAX"
            "hoaxHOAXhoaxHOAXhoaxHn4324329dvsj'fJKFDS:LFJDS(*)J%#$@OIhoaxHOAX",
            "f80105a7737ac77aad95f2c036698455922e4ccd"
        }
    };

    for (int i = 0; i < ARR_LEN(cases); i++) {
        printf("Test case %u/%u", i+1, ARR_LEN(cases));
        char hash[20];
        char* msg = cases[i].msg;
        sha1(msg, strlen(msg), hash);
        char hash_str[41];
        sha1_to_str(hash, hash_str);

        if (strcmp(cases[i].hash, hash_str)) {
            puts("\t[FAIL]");
            printf("\tExpected: %s\n", cases[i].hash);
            printf("\tActual  : %s\n", hash_str);
        }
        else
            puts("\t[Success]");
    }
}

int main(char argc, char** argv) {
    test_sha1();
}
