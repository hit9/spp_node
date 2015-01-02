#include <assert.h>
#include <stdio.h>
#include <string.h>
#ifdef __linux
#include <mcheck.h>
#endif
#include "spp.h"

typedef void (*case_t)();

static void test_case(const char *, case_t);
static char *persistent_suite [100] = {0};
static void assert_handler(spp_t *, char *, size_t, int);
static void assign_persistent_suite(int, char *);

void case_spp_new();
void case_spp_free();
void case_spp_clear();
void case_spp_feed();
void case_spp_parse();

int main(int argc, const char *argv[])
{
#ifdef __linux
    mtrace();
#endif
    test_case("spp_new", &case_spp_new);
    test_case("spp_free", &case_spp_free);
    test_case("spp_clear", &case_spp_clear);
    test_case("spp_feed", &case_spp_feed);
    test_case("spp_parse", &case_spp_parse);
    return 0;
}

static void
test_case(const char *name, case_t case_func)
{
    case_func();
    printf("OK CASE(%s)\n", name);
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

void
case_spp_new()
{
    spp_t *spp = spp_new();
    spp->handler = &assert_handler;
    assert(spp != NULL && spp->buf != NULL);
    spp_free(spp);
}


void
case_spp_free()
{
    spp_t *spp = spp_new();
    spp->handler = &assert_handler;
    spp_free(spp);
}


void
case_spp_clear()
{
    spp_t *spp = spp_new();
    spp->handler = &assert_handler;
    assert(spp_feed(spp, "somestring") == SPP_OK);
    spp_clear(spp);
    assert(spp->buf->size == 0 && spp->buf->data == NULL);
    spp_free(spp);
}


void
case_spp_feed()
{
    spp_t *spp = spp_new();
    spp->handler = &assert_handler;
    char *s1 = "hello";
    char *s2 = "world";
    assert(spp_feed(spp, s1) == SPP_OK);
    assert(spp_feed(spp, s2) == SPP_OK);
    assert(strcmp(hbuf_str(spp->buf), "helloworld") == 0);
    spp_free(spp);
}


void
case_spp_parse()
{
    spp_t *spp = spp_new();
    spp->handler = &assert_handler;

    spp_feed(spp, "2\nok\n\n");
    assign_persistent_suite(0, "ok");
    assert(spp_parse(spp) == SPP_OK);

    spp_feed(spp, "2\nok\n4\nbody\n\r\n");
    assign_persistent_suite(0, "ok");
    assign_persistent_suite(1, "body");
    assert(spp_parse(spp) == SPP_OK);

    spp_feed(spp, "2\nok\n4\nbody");
    assign_persistent_suite(0, "ok");
    assert(spp_parse(spp) == SPP_EUNFINISH);

    spp_feed(spp, "\n\n");
    assign_persistent_suite(0, "ok");
    assign_persistent_suite(1, "body");
    assert(spp_parse(spp) == SPP_OK);
    spp_free(spp);
}
