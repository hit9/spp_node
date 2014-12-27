#include "spp.h"


spp_t *
spp_new()
{
    spp_t *spp = malloc(sizeof(spp_t));
    if (spp == NULL) return NULL;
    spp->buf = 0;
    spp->bsz = 0;
    spp->priv = NULL;
    spp->handler = NULL;
    return spp;
}


int
spp_feed(spp_t *spp, char *buf, size_t len)
{
    char *buf_ = realloc(spp->buf, sizeof(char) * len);

    if (buf_ == NULL) return SPP_ENOMEM;

    spp->buf = buf_;
    memcpy(spp->buf + spp->bsz, buf, len);
    spp->bsz += len;
    return SPP_OK;
}


int
spp_free(spp_t *spp)
{
    free(spp->buf);
    /* free(spp); */
    return SPP_OK;
}


int
spp_get(spp_t *spp)
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
            // TOOD: free completed
            return SPP_OK;
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
