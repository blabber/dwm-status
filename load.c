/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#define _POSIX_C_SOURCE 199506

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "bsd.h"
#include "buffers.h"
#include "load.h"
#include "tools.h"

struct load_context {
        char            load_str[LOAD_BUFFLEN];
};

int             loadstr(double _loadval, size_t _loadbuflen, char *_loadbuf);

struct load_context *
load_context_open()
{
        struct load_context *ctx;

        if ((ctx = malloc(sizeof(*ctx))) == NULL)
                err(EXIT_FAILURE, "malloc(%d) load_context", sizeof(*ctx));

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
        char            la1[6], la5[6], la15[6];

        assert(ctx != NULL);

        if (bsd_getloadavg(la, 3) == -1)
                errx(EXIT_FAILURE, "bsd_getloadavg");

        loadstr(la[0], sizeof(la1), la1);
        loadstr(la[1], sizeof(la5), la5);
        loadstr(la[2], sizeof(la15), la15);

        tools_catitems(ctx->load_str, sizeof(ctx->load_str), la1, " ", la5, " ", la15, NULL);

        return (ctx->load_str);
}

int
loadstr(double loadval, size_t loadbuflen, char *loadbuf)
{
        int             c;

        assert(loadval >= 0 && loadval <= 99);
        assert(loadbuflen > 5);
        assert(loadstr != NULL);

        c = sprintf(loadbuf, "%.2f", loadval);

        return (c);
}
