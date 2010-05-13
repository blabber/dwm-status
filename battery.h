/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

struct battery_context;

void            battery_context_close(struct battery_context *_ctx);
struct battery_context *battery_context_open(void);
char           *battery_str(struct battery_context *_ctx);
