/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

struct load_context;

void			 load_context_close(struct load_context *_ctx);
struct load_context	*load_context_open(void);
wchar_t			*load_str(struct load_context *_ctx);
