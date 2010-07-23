/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <sysexits.h>
#include <time.h>

#include "clock.h"

enum {
        STRLEN = 24
};

struct clock_context {
        char            clock_str[STRLEN];
};

void
clock_context_close(struct clock_context *ctx)
{
        assert(ctx != NULL);

        free(ctx);
}

struct clock_context *
clock_context_open(void)
{
        struct clock_context *ctx;

        if ((ctx = malloc(sizeof(*ctx))) == NULL)
                err(EX_SOFTWARE, "malloc(%d) clock_context", sizeof(struct clock_context));

        return (ctx);
}

char           *
clock_str(struct clock_context *ctx)
{
        time_t          t;
        struct tm      *tm;

        assert(ctx != NULL);

        if (time(&t) == (time_t) - 1)
                err(EX_SOFTWARE, "time()");
        if ((tm = localtime(&t)) == NULL)
                err(EX_SOFTWARE, "localtime()");

        strftime(ctx->clock_str, sizeof(ctx->clock_str), "%c", tm);

        return (ctx->clock_str);
}
