/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#define _POSIX_C_SOURCE 199506

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "tools.h"

int
tools_catitems(char *outbuf, size_t outbuflen,...)
{
        va_list         ap;
        char           *op, *ip;
        int             ccount = 0, cmax;

        assert(outbuf != NULL);

        cmax = outbuflen - 1;
        op = outbuf;

        va_start(ap, outbuflen);
        while ((ip = va_arg(ap, char *)) != NULL) {
                while (*ip != '\0' && ccount++ < cmax) {
                        *(op++) = *(ip++);
                }
        }
        *op = '\0';
        va_end(ap);

        return (ccount);
}
