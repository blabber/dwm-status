/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

struct clock_context;

void            clock_context_close(struct clock_context *_ctx);
struct clock_context *clock_context_open(void);
char           *clock_str(struct clock_context *_ctx);
