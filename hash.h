#include <stdint.h>

typedef uint8_t uchar;
typedef uint32_t uint32;
typedef uint64_t uint64;

int isBE();
void sha1(const uchar*, uint64, uchar*);
uint32 get_uint32be(uchar*);
void main_loop_calc(uint32*, uint32*, uint32*, uint32*, uint32*, uint32, uint32, uint32);
uint32 leftrotate32(uint32, uint32);
uint32 get_uint32be(uchar*);
uint32 getBE32(uint32);
uint64 getBE64(uint64);
void uint32le_to_uint160be(uint32, uchar*);
void uint160_leftbyteshift(uchar*, int);
void uint160_or(uchar*, uchar*, uchar* res);
void sha1_to_str(uchar* hash, char* dst);
char int4_to_hex(uchar x);
