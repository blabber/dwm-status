/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

enum {
        BATTERY_BUFFLEN = 9,
        CLOCK_BUFFLEN = 26,
        LOAD_BUFFLEN = 18,
        MPD_BUFFLEN = 64,

        /*
         * Sum of all buffer sizes, minus string terminators, plus space for
         * separators, plus final string terminator
         */
        STATUS_BUFFLEN = BATTERY_BUFFLEN + CLOCK_BUFFLEN + LOAD_BUFFLEN + MPD_BUFFLEN - (4 * 1) + (3 * 3) + 1,
};
