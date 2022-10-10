#ifndef LIBUNIBASE2N_UINT128_H
#define LIBUNIBASE2N_UINT128_H

#include <stdlib.h>
#include <stdint.h>
#if defined(_WIN32) || defined(_WIN64)
#define swap_uint32 _byteswap_ulong
#define swap_uint64 _byteswap_uint64
#else
#define swap_uint32 __builtin_bswap32
#endif /* _WIN32 */



typedef struct unibase_uint128be_s
{
    uint64_t a;
    uint64_t b; // low
} unibase_uint128be_t;

unibase_uint128be_t readuint128be(uint8_t *b);

void unibase_uint128be_add(unibase_uint128be_t *self, unibase_uint128be_t * n);
#endif //LIBUNIBASE2N_UINT128_H
