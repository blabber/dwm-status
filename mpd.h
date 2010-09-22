/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

struct mpd_context;

void            mpd_context_close(struct mpd_context *_ctx);
struct mpd_context *mpd_context_open(void);
char           *mpd_str(struct mpd_context *_ctx);
