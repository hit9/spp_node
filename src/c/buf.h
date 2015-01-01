/**
 *
 * Copyright (c) 2015, Chao Wang (hit9 <hit9@icloud.com>)
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#ifndef __BUF_H
#define __BUF_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BUF_MAX_SIZE 16 * 1024 * 1024  //16mb

typedef enum {
    BUF_OK = 0,
    BUF_ENOMEM = 1,
} buf_error_t;

typedef struct buf_st {
    uint8_t *data;      /* real data */
    size_t size;        /* real data size */
    size_t cap;         /* buf cap */
    size_t unit;        /* reallocation unit size */
} buf_t;


buf_t *buf_new(size_t);
void buf_free(buf_t *);
void buf_clear(buf_t *);
int buf_grow(buf_t *, size_t);
char *buf_str(buf_t *);
void buf_print(buf_t *);
int buf_put(buf_t *, uint8_t *, size_t);
int buf_putc(buf_t *, char);
int buf_puts(buf_t *, char *);
void buf_lrm(buf_t *, size_t);
void buf_rrm(buf_t *, size_t);

#ifdef __cplusplus
}
#endif

#endif
