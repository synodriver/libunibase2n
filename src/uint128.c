//
// Created by LAB on 2022/10/10.
//
#include <stdbool.h>
#include "uint128.h"

unibase_uint128be_t
readuint128be(uint8_t *b)
{
#ifdef WORDS_BIGENDIAN
    unibase_uint128be_t ret = {*((uint64_t *) b), *((uint64_t *) b + 1)};
    return ret;
#else
    unibase_uint128be_t ret = {swap_uint64(*((uint64_t *) b)), swap_uint64(*((uint64_t *) b + 1))};
    return ret;
#endif
}


static inline bool _check_overflow(uint64_t a, uint64_t b)
{
    if ((a&0x8000000000000000) && (b&0x8000000000000000) ) // 最高位都是1
    {
        return true;
    }
    if((a&0x8000000000000000) || (b&0x8000000000000000)) // 一个1 一个0
    {
        if (!( (a+b)&0x8000000000000000)) // 加起来之后最高位是0 一定溢出
        {
            return true;
        }
    }
    return false;
}

void unibase_uint128be_add(unibase_uint128be_t *self, unibase_uint128be_t *n)
{
    if (_check_overflow(self->b, n->b))
    {
        self->b += n->b;
        self->a += n->a + 1;
        return;
    }
    self->b += n->b;
    self->a += n->a;
}
