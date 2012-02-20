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
#include <stdlib.h>
#include <time.h>

#include "buffers.h"
#include "clock.h"

struct clock_context {
        char            clock_str[CLOCK_BUFFLEN];
};

struct clock_context *
clock_context_open(void)
{
        struct clock_context *ctx;

        if ((ctx = malloc(sizeof(*ctx))) == NULL)
                err(EXIT_FAILURE, "malloc clock_context");

        return (ctx);
}

void
clock_context_close(struct clock_context *ctx)
{
        assert(ctx != NULL);

        free(ctx);
}

char           *
clock_str(struct clock_context *ctx)
{
        time_t          t;
        struct tm      *tm;

        assert(ctx != NULL);

        if (time(&t) == (time_t) (-1))
                errx(EXIT_FAILURE, "time()");
        if ((tm = localtime(&t)) == NULL)
                errx(EXIT_FAILURE, "localtime()");

        if (strftime(ctx->clock_str, sizeof(ctx->clock_str), "%c", tm) == 0)
                errx(EXIT_FAILURE, "strftime()");

        return (ctx->clock_str);
}
