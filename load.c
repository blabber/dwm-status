/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "bsd.h"
#include "buffers.h"
#include "load.h"

struct load_context {
	wchar_t	load_str[LOAD_BUFFLEN];
};

static int	loadstr(double _loadval, size_t _loadbuflen, wchar_t *_loadbuf);

struct load_context *
load_context_open(void)
{
	struct load_context *ctx;

	if ((ctx = malloc(sizeof(*ctx))) == NULL)
		err(EXIT_FAILURE, "malloc load_context");

	return (ctx);
}

void
load_context_close(struct load_context *ctx)
{
	assert(ctx != NULL);

	free(ctx);
}

wchar_t *
load_str(struct load_context *ctx)
{
	double	la[3];
	wchar_t	la1[6], la5[6], la15[6];

	assert(ctx != NULL);

	if (bsd_getloadavg(la, 3) == -1)
		errx(EXIT_FAILURE, "bsd_getloadavg");

	if (loadstr(la[0], WCSLEN(la1), la1) == 0)
		errx(EXIT_FAILURE, "loadstr");
	if (loadstr(la[1], WCSLEN(la5), la5) == 0)
		errx(EXIT_FAILURE, "loadstr");
	if (loadstr(la[2], WCSLEN(la15), la15) == 0)
		errx(EXIT_FAILURE, "loadstr");

	if (swprintf(ctx->load_str, WCSLEN(ctx->load_str), L"%S %S %S",
	    la1, la5, la15) == 0)
		errx(EXIT_FAILURE, "swprintf");

	return (ctx->load_str);
}

static int
loadstr(double loadval, size_t loadbuflen, wchar_t *loadbuf)
{
	int	c;

	assert(loadval >= 0 && loadval <= 99);
	assert(loadbuflen > 5);
	assert(loadstr != NULL);

	c = swprintf(loadbuf, loadbuflen, L"%.2f", loadval);

	return (c);
}
