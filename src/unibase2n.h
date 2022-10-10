//
// Created by LAB on 2022/10/10.
//

#ifndef LIBUNIBASE2N_UNIBASE2N_H
#define LIBUNIBASE2N_UNIBASE2N_H

#include <stdint.h>

#ifndef UNIBASE_MALLOC

#include <stdlib.h>

#define UNIBASE_MALLOC malloc
#define UNIBASE_REALLOC realloc
#define UNIBASE_FREE free
#endif


typedef struct unibase_base_s
{
    uint16_t off;
    uint16_t til;
    uint8_t bit;
} unibase_base_t;

typedef enum
{
    UNIBASE_ERR_OK,
    UNIBASE_ERR_INVALID_BIT_SIZE,
    UNIBASE_ERR_ZERO_OFFSET_START,
    UNIBASE_ERR_OFFSET_OVERFLOW,
    UNIBASE_ERR_TAIL_OVERFLOW,
    UNIBASE_ERR_TAIL_INCODING_AREA
} unibase_error_t;

// malloc on heap
unibase_base_t *unibase_base_new();

void unibase_base_del(unibase_base_t **self);

unibase_error_t unibase_base_init(unibase_base_t *self, uint16_t off, uint16_t til, uint8_t bit);

unibase_error_t unibase_base_check(unibase_base_t *self);
// internal use only
void _unibase_base_encode_len(unibase_base_t *self, /*INPUT*/int in, /*OUTPUT */int *out, /*OUTPUT */int *offset);

void _unibase_base_decode_len(unibase_base_t *self, /*INPUT*/int in, /*INPUT*/int offset, /*OUTPUT */int* out);

#endif //LIBUNIBASE2N_UNIBASE2N_H
