#include "spp.h"


/**
 * New a spp parser.
 */
spp_t *
spp_new()
{
    spp_t *spp = malloc(sizeof(spp_t));
    if (spp == NULL)
        return NULL;
    spp->buf = NULL;
    spp->bsz = 0;
    spp->priv = NULL;
    spp->handler = NULL;
    return spp;
}


/**
 * Feed a spp parser with data.
 */
int
spp_feed(spp_t *spp, char *buf, size_t len)
{
    size_t new_bsz = len + spp->bsz;
    char *new_buf = realloc(spp->buf, sizeof(char) * new_bsz);

    if (new_buf == NULL) return SPP_ENOMEM;

    spp->buf = new_buf;
    memcpy(spp->buf + spp->bsz, buf, len);
    spp->bsz = new_bsz;
    return SPP_OK;
}


/**
 * Free a spp parser and its data.
 */
int
spp_free(spp_t *spp)
{
    if (spp->buf != NULL)
        free(spp->buf);
    free(spp);
    return SPP_OK;
}


/**
 * Clear s spp parser (its buf)
 */
int
spp_clear(spp_t *spp)
{
    if (spp->buf != NULL)
        free(spp->buf);
    spp->buf = NULL;
    spp->bsz = 0;
    return SPP_OK;
}


/**
 * Slice spp parser start at somewhere (not an api).
 */
int
spp_buf_slice(spp_t *spp, char * start)
{
    size_t dis = start - spp->buf;
    size_t new_bsz = spp->bsz - dis;
    char *new_buf = malloc(sizeof(char) * new_bsz);

    if (new_buf == NULL) return SPP_ENOMEM;

    memcpy(new_buf, start, new_bsz);
    free(spp->buf);
    spp->buf = new_buf;
    spp->bsz = new_bsz;
    return SPP_OK;
}


/**
 * Parse data.
 */
int
spp_parse(spp_t *spp)
{
    int id = 0;
    size_t len = spp->bsz;
    char *ptr = spp->buf;

    while(len > 0) {
        char *ch = (char *)memchr(ptr, '\n', len);

        if (ch == NULL) break;

        ch += 1;
        int dis = ch - ptr;

        if(dis == 1 || (dis == 2 && ptr[0] == '\r')) {
            return spp_buf_slice(spp, ch);
        }

        int sz = (int)strtol(ptr, NULL, 10);

        len -= dis + sz;
        ptr += dis + sz;

        if (len >= 1 && ptr[0] == '\n') {
            len -= 1;
            ptr += 1;
        } else if (len >= 2 && ptr[0] == '\r' && ptr[1] == '\n') {
            len -= 2;
            ptr += 2;
        } else {
            break;
        }

        (spp->handler)(spp, ch, sz, id++);
    }
    return SPP_EBADFMT;
}
