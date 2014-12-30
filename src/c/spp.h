/*
 * Copyright (c) 2014-2015, hit9
 *
 * SPP - Simple Protocol Parser
 *
 *   Packet := Block+ '\n'
 *   Block  := Size '\n' Data '\n'
 *   Size   := literal_integer
 *   Data   := string_bytes
 *
 * Example
 *
 *   '2\nok\n\n'  - complete
 *    / => ['ok']
 */


#ifndef __SPP_H
#define __SPP_H

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPP_MAX_SIZE 16 * 1024 * 1024  // 16mb


typedef enum {
    SPP_OK = 0,
    SPP_ENOMEM = -1,
    SPP_EEXCMAXSIZE = -2,
    SPP_EBADFMT = -3,
    SPP_EUNFINISH = -4,
} spp_st_t;


typedef struct spp_st {
    char *data;  /* feed data */
    size_t size;  /* feed data size */
    void *priv;  /* user-settable arbitrary field */
    /* *
     * required handler, called with 4 arguments: spp itself,
     * received-data, received-data size and received-data index.
     */
    void (*handler)(struct spp_st*, char *, size_t, int);
} spp_t;


/* Public API for the protocol parser */
spp_t* spp_new();
int spp_feed(spp_t *, char *, size_t);
int spp_free(spp_t *);
int spp_parse(spp_t *);
int spp_clear(spp_t *);

#ifdef __cplusplus
}
#endif
#endif
