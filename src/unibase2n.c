//
// Created by LAB on 2022/10/10.
//
#include "unibase2n.h"

static const char *powtab = "\x00\x04\x03\x00\x02\x00\x00\x00\x01";

static inline int
bits_ones_count8(uint8_t bit)
{
    return (0x01 & bit) +
           ((0x02 & bit) >> 1) +
           ((0x04 & bit) >> 2) +
           ((0x08 & bit) >> 3) +
           ((0x10 & bit) >> 4) +
           ((0x20 & bit) >> 5) +
           ((0x40 & bit) >> 6) +
           ((0x80 & bit) >> 7);
}


unibase_base_t *
unibase_base_new()
{
    unibase_base_t *self = (unibase_base_t *) UNIBASE_MALLOC(sizeof(unibase_base_t));
    if (self == NULL)
    {
        return NULL;
    }
    return self;
}

void unibase_base_del(unibase_base_t **self)
{
    if((*self)!=NULL)
    {
        UNIBASE_FREE(*self);
        *self=NULL;
    }
}

unibase_error_t
unibase_base_init(unibase_base_t *self, uint16_t off, uint16_t til, uint8_t bit)
{
    self->off = off;
    self->til = til;
    self->bit = bit;
    return unibase_base_check(self);
}

unibase_error_t
unibase_base_check(unibase_base_t *self)
{
    uint16_t off = self->off;
    uint16_t til = self->til;
    uint8_t bit = self->bit;
    if (off == 0)
    {
        return UNIBASE_ERR_ZERO_OFFSET_START;
    }
    if (bit >= 16 || bit == 0)
    {
        return UNIBASE_ERR_INVALID_BIT_SIZE;
    }
    uint32_t offe = ((uint32_t) off) + (1 << bit); // [off, offe)
    if (offe > 0x10000)
    {
        return UNIBASE_ERR_OFFSET_OVERFLOW;
    }
    uint32_t tile = (uint32_t) til;
    if (bit > 8 && bit % 2 == 0)
    {
        tile += (uint32_t) (bit / 2);
    }
    else
    {
        tile += (uint32_t) bit;
    }
    if (tile > 0x10000)
    {
        return UNIBASE_ERR_TAIL_OVERFLOW;
    }
    if (tile > (uint32_t) off && tile <= offe)
    {
        return UNIBASE_ERR_TAIL_INCODING_AREA;
    }
    return UNIBASE_ERR_OK;
}


void _unibase_base_encode_len(unibase_base_t *self, int in, /*OUT */int *out, /*OUT */int *offset)
{
    if (in < 0)
    {
        *out = 0;
        *offset = 0;
        return;
    }
    if (bits_ones_count8(self->bit) == 1) // 1/2/4/8
    {
        *out = in << (uint8_t) powtab[self->bit];
        *offset = 0;
        return;
    }
    if (self->bit % 2 == 0)
    {
        int hb = ((int) self->bit) >> 1;
        *out = in / hb * 8;
        *offset = in % hb;
        if (*offset == 0)
        {
            return;
        }
        int remain = *offset * 8;
        *out += (remain / (int) self->bit + 1 + 1) * 2;
        return;
    }
    // 其他奇数
    *out = in / (int) self->bit * 16;
    *offset = in % (int) self->bit;
    if (*offset == 0)
    {
        return;
    }
    int remain = *offset * 8;
    *out += (remain / (int) self->bit + 1 + 1) * 2;
}

void _unibase_base_decode_len(unibase_base_t *self, /*INPUT*/int in, /*INPUT*/int offset, /*OUTPUT */int *out)
{
    if (in <= 0 || offset < 0)
    {
        *out = 0;
        return;
    }
    if (bits_ones_count8(self->bit) == 1)
    {
        *out = in >> powtab[self->bit];
        return;
    }
    if (self->bit % 2 == 0)
    {
        int hb = ((int) self->bit) >> 1;
        if (offset != 0)
        {
            int remain = offset * 8;
            in -= (remain / (int) self->bit + 1) * 2;
        }
        *out = (in >> 3) * hb + offset;
        return;
    }
// 其他奇数
    if (offset != 0)
    {
        int remain = offset * 8;
        in -= (remain / (int) self->bit + 1) * 2;

    }
    *out = (in >> 4) * ((int) self->bit) + offset;
}
