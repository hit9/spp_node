#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#ifdef __linux
#include <mcheck.h>
#endif
#include "spp.h"

#define SUIT_CAP 100
#define CASE(NAME) printf("CASE(" NAME ")")
#define PASS() printf(" => PASSED\n")

static char *persistent_suite [SUIT_CAP] = {0};

static void assert_handler(spp_t *, char *, size_t, int);
static void assign_persistent_suite(int, char *);

int main(int argc, const char *argv[])
{
#ifdef __linux
    mtrace();
#endif

    CASE("spp_new");
    spp_t * spp = spp_new();
    spp->handler = &assert_handler;
    PASS();

    CASE("spp_feed");
    char *s1 = "2\nok\n4\nbody\n\r\n";
    assert(spp_feed(spp, s1, strlen(s1)) == SPP_OK);
    PASS();

    CASE("spp_parse (ok suite)");
    assign_persistent_suite(0, "ok");
    assign_persistent_suite(1, "body");
    assert(spp_parse(spp) == SPP_OK);
    assert(spp_parse(spp) == SPP_EUNFINISH);
    PASS();

    CASE("spp_parse (bad format suite)");
    char *s2 = "a\nbadformat\r\n";
    assert(spp_feed(spp, s2, strlen(s2)) == SPP_OK);
    assert(spp_parse(spp) == SPP_EBADFMT);
    PASS();

    CASE("spp_clear");
    assert(spp_clear(spp) == SPP_OK);
    PASS();

    CASE("spp_parse (twice)");
    char *s3 = "2\nok\n";
    char *s4 = "4\nbody\n\n";
    assign_persistent_suite(0, "ok");
    assign_persistent_suite(1, "body");
    assert(spp_feed(spp, s3, strlen(s3)) == SPP_OK);
    assert(spp_parse(spp) == SPP_EUNFINISH);
    assert(spp_feed(spp, s4, strlen(s4)) == SPP_OK);
    assert(spp_parse(spp) == SPP_OK);

    CASE("spp_free");
    assert(spp_free(spp) == SPP_OK);
    PASS();
    return 0;
}

static void
assert_handler (spp_t *spp, char *str, size_t size, int idx)
{
    assert(memcmp(persistent_suite[idx], str, size) == 0);
}


static void
assign_persistent_suite(int idx, char *str)
{
    persistent_suite[idx] = str;
}
