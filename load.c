/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "load.h"

enum {
        STRLEN = 18,
};

struct load_context {
        char            load_str[STRLEN];
};

struct load_context *
load_context_open()
{
        struct load_context *ctx;

        if ((ctx = malloc(sizeof(*ctx))) == NULL)
                err(EX_SOFTWARE, "malloc(%d) load_context", sizeof(*ctx));

        return (ctx);
}

void
load_context_close(struct load_context *ctx)
{
        assert(ctx != NULL);

        free(ctx);
}

char           *
load_str(struct load_context *ctx)
{
        double          la[3];

        assert(ctx != NULL);

        if (getloadavg(la, 3) == -1)
                errx(EX_SOFTWARE, "getloadavg");

        snprintf(ctx->load_str, sizeof(ctx->load_str), "%.2f %.2f %.2f", la[0], la[1], la[2]);

        return (ctx->load_str);
}
