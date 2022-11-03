#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// is this a big-endian architecture?
int isBE() {
    static int x = 1;
    return *((uchar*) &x) == 0;
}

/* 
 * msg - original message
 * len - message length in *bytes*
 * dst - 20 byte buffer for resulting 160-bits digest
 */
void sha1(const uchar* msg, uint64 len_bytes, uchar* dst) {
    uint32 h0 = 0x67452301;
    uint32 h1 = 0xEFCDAB89;
    uint32 h2 = 0x98BADCFE;
    uint32 h3 = 0x10325476;
    uint32 h4 = 0xC3D2E1F0;

    uint64 len_bits = len_bytes * 8;
    uint32 len_bits_mod_512 = len_bits % 512;
    uint32 pad_n_bits = len_bits_mod_512 < 448 ? 448 - len_bits_mod_512 : 
        (512 - len_bits_mod_512) + 448;
    uint32 pad_n_bytes = pad_n_bits / 8;

    // original message len (bits) + pad_n_bits + 64 bits of orig. message length
    uint64 res_len_bytes = (len_bits + pad_n_bits + 64) / 8;
    uchar message[res_len_bytes];
    // copy original message contents
    memcpy(message, msg, len_bytes);

    // pre-processing
    // 1. append the bit '1' to the message
    // 2. append 447 bits '0' to the message

    // 1. a single byte 0x80
    message[len_bytes] = 0x80;
    for (int i = 0; i < pad_n_bytes - 1; i++)
        // 2. rest bytes 0x00
        message[len_bytes+i+1] = 0x00;

    // append the original message length in bits as BE uint64
    uint64 len_bits_be = getBE64(len_bits);
    for (int i = 0; i < 8; i++)
        message[len_bytes+pad_n_bytes+i] = ((uchar*) &len_bits_be)[i];

    // process the message in successive 512-bit (64-byte) chunks
    for (int i = 0; i < res_len_bytes / 64; i++) {
        uchar* chunk = message + (i*64);
        // total num of words with extension is 80
        uint32 words[80];

        // first break chunk into 16 32-bit (4-byte) BE words
        for (int j = 0; j < 16; j++)
            words[j] = get_uint32be(chunk+(j*4));

        // then extend to 80 words
        for (int j = 16; j < 80; j++)
            words[j] = leftrotate32(words[j-3] ^ words[j-8] ^ words[j-14] ^ words[j-16], 1);
        
        // initialize hash values for this chunk
        uint32 a = h0;
        uint32 b = h1;
        uint32 c = h2;
        uint32 d = h3;
        uint32 e = h4;

        // main loop
        uint32 f, k;
        for (int j = 0; j <= 19; j++) {
            f = (b & c) | ((~b) & d);
            k = 0x5a827999;
            main_loop_calc(&a, &b, &c, &d, &e, f, k, words[j]);
        }
        for (int j = 20; j <= 39; j++) {
            f = b ^ c ^ d;
            k = 0x6ed9eba1;
            main_loop_calc(&a, &b, &c, &d, &e, f, k, words[j]);
        }
        for (int j = 40; j <= 59; j++) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8f1bbcdc;
            main_loop_calc(&a, &b, &c, &d, &e, f, k, words[j]);
        }
        for (int j = 60; j <= 79; j++) {
            f = b ^ c ^ d;
            k = 0xca62c1d6;
            main_loop_calc(&a, &b, &c, &d, &e, f, k, words[j]);
        }

        // add this chunk's hash to result so far
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // produce the final hash value (big-endian) as 160-bit number
    uint32 hs_be[5] = {
        getBE32(h0),
        getBE32(h1),
        getBE32(h2),
        getBE32(h3),
        getBE32(h4),
    };
    for (int i = 0; i < 5; i++) {
        uchar* hash_part = dst + (i*4);
        uint32 hn = hs_be[i];
        for (int j = 0; j < 4; j++)
            hash_part[j] = ((uchar*) &hn)[j];
    }
}

// shared code for the main loop
void main_loop_calc(uint32* a, uint32* b, uint32* c, uint32* d, uint32* e, 
        uint32 f, uint32 k, uint32 word) 
{
    uint32 temp = leftrotate32(*a, 5) + f + *e + k + word;
    *e = *d;
    *d = *c;
    *c = leftrotate32(*b, 30);
    *b = *a;
    *a = temp;
}

uint32 leftrotate32(uint32 num, uint32 shift) {
    if (shift == 0 || shift == 32)
        return num;
    return num << shift | num >> (32 - shift);
}

// constructs a 4-byte big-endian word from buf
uint32 get_uint32be(uchar* buf) {
    uint32 x = 0;
    x += ((uint32) buf[0]) << 24;
    x += ((uint32) buf[1]) << 16;
    x += ((uint32) buf[2]) << 8;
    x += buf[3];
    return x;
}

// get 32-bit value in big-endian
uint32 getBE32(uint32 x) {
    if (isBE())
        return x;

    uint32 b1 = x & 0x000000ff;
    uint32 b2 = x & 0x0000ff00;
    uint32 b3 = x & 0x00ff0000;
    uint32 b4 = x & 0xff000000;

    return (b1 << 24) | (b2 << 8) | (b3 >> 8) | (b4 >> 24);
}

// get 64-bit value in big-endian
uint64 getBE64(uint64 x) {
    if (isBE())
        return x;

    uint64 b1 = x & 0x00000000000000ff;
    uint64 b2 = x & 0x000000000000ff00;
    uint64 b3 = x & 0x0000000000ff0000;
    uint64 b4 = x & 0x00000000ff000000;
    uint64 b5 = x & 0x000000ff00000000;
    uint64 b6 = x & 0x0000ff0000000000;
    uint64 b7 = x & 0x00ff000000000000;
    uint64 b8 = x & 0xff00000000000000;

    return (b1 << 56) | (b2 << 40) | (b3 >> 24) | (b4 >> 8) |
        (b5 >> 8) | (b6 >> 24) | (b7 >> 32) | (b8 >> 56);
}

void sha1_to_str(uchar* hash, char* dst) {
    int i, j;
    for (i = 0, j = 0; i < 20; i++) {
        uchar b = hash[i];
        uchar q1 = (b & 0xf0) >> 4;
        dst[j++] = int4_to_hex(q1);
        uchar q2 = b & 0x0f;
        dst[j++] = int4_to_hex(q2);
    }
    dst[j] = '\0';
}

char int4_to_hex(uchar x) {
    if (x > 16)
        return -1;
    else if (x <= 9)
        return 48 + x;
    return 97 - 10 + x;
}
