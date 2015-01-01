/**
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

#include "buf.h"

/**
 * New buf
 */
buf_t *
buf_new(size_t unit)
{
    buf_t *buf = malloc(sizeof(buf_t));

    if (buf != NULL) {
        buf->data = NULL;
        buf->size = 0;
        buf->cap = 0;
        buf->unit = unit;
    }

    return buf;
}

/**
 * Free buf
 */
void
buf_free(buf_t *buf)
{
    if (buf->data != NULL)
        free(buf->data);
    if (buf != NULL)
        free(buf);
}


/**
 * Free buf data
 */
void
buf_clear(buf_t *buf)
{
    if (buf->data != NULL)
        free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->cap = 0;
}

/**
 * Increase buf allocated size to `size`
 */
int
buf_grow(buf_t *buf, size_t size)
{
    assert(buf != NULL && buf->unit != 0);

    if (size > BUF_MAX_SIZE)
        return BUF_ENOMEM;

    if (size <= buf->cap)
        return BUF_OK;

    size_t cap = buf->cap + buf->unit;

    while (cap < size)
        cap += buf->unit;

    uint8_t *data = realloc(buf->data, cap);

    if (data == NULL)
        return BUF_ENOMEM;

    buf->data = data;
    buf->cap = cap;
    return BUF_OK;
}

/**
 * Get data as c string (terminate with '\0')
 */
char *
buf_str(buf_t *buf)
{
    assert(buf && buf->unit);

    if (buf->size < buf->cap && buf->data[buf->size] == '\0')
        return (char *)buf->data;

    if (buf->size + 1 <= buf->cap ||
            buf_grow(buf, buf->size + 1) == BUF_OK) {
        buf->data[buf->size] = '\0';
        return (char *)buf->data;
    }

    return NULL;
}

/**
 * Put a char to buf
 */
int
buf_putc(buf_t *buf, char ch)
{
    int res = buf_grow(buf, buf->size + 1);

    if (res != BUF_OK)
        return res;

    buf->data[buf->size] = ch;
    buf->size += 1;
    return BUF_OK;
}

/**
 * Print buf to stdout
 */
void
buf_print(buf_t *buf)
{
    printf("%.*s\n", (int)buf->size, buf->data);
}

/**
 * Put data to buf
 */
int
buf_put(buf_t *buf, uint8_t *data, size_t size)
{
    int res = buf_grow(buf, buf->size + size);

    if (res != BUF_OK)
        return res;

    memcpy(buf->data + buf->size, data, size);
    buf->size += size;
    return BUF_OK;
}

/**
 * Put string to buf
 */
int
buf_puts(buf_t *buf, char *str)
{
    return buf_put(buf, (uint8_t *)str, strlen(str));
}


/**
 * Remove left data from buf by number of bytes
 */
void
buf_lrm(buf_t *buf, size_t size)
{
    assert(buf != NULL && buf->unit != 0);

    if (size > buf->size) {
        buf->size = 0;
        return;
    }

    buf->size -= size;
    memmove(buf->data, buf->data + size, buf->size);
}


/**
 * Remove right data from buf by number of bytes
 */
void
buf_rrm(buf_t *buf, size_t size)
{
    assert(buf != NULL && buf->unit != 0);

    if (size > buf->size) {
        buf->size = 0;
        return;
    }
    buf->size -= size;
}
