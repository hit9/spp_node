#include <stdio.h>
#include <string.h>
#include "spp.h"

static int print_string(spp_t * spp, char *str, size_t size, int id) {
    printf("%.*s %d ", (int)size, str, id);
};

int main(int argc, const char *argv[])
{
    char * s = "3\nabc\n1\nq\n\na";
    int size = 0;
    spp_t *spp = spp_new();
    spp->handler = &print_string;
    spp_feed(spp, s, strlen(s));
    printf("%d\n", spp_get(spp));
    spp_free(spp);
    return 0;
}
