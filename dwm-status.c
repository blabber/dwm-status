/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <err.h>
#include <locale.h>
#include <stdio.h>
#include <sysexits.h>
#include <unistd.h>

#include "battery.h"
#include "clock.h"

enum {
        SLEEP = 3
};

int
main(void)
{
        struct clock_context *clock_ctx;
        struct battery_context *battery_ctx;

        if (setlocale(LC_ALL, "") == NULL)
                err(EX_SOFTWARE, "setlocale()");

        if ((clock_ctx = clock_context_open()) == NULL)
                err(EX_SOFTWARE, "clock_context_open()");
        if ((battery_ctx = battery_context_open()) == NULL)
                err(EX_SOFTWARE, "battery_context_open()");

        for (;;) {
                char           *clock;
                char           *battery;

                if ((clock = clock_str(clock_ctx)) == NULL)
                        err(EX_SOFTWARE, "clock_str");
                if ((battery = battery_str(battery_ctx)) == NULL)
                        err(EX_SOFTWARE, "clock_str");

                /*
                 * XXX During development this tool is printing the status to
                 * stdout. Later this will be replaced with the correspanding
                 * xsetroot call.
                 */
                printf(" | %s | %s\n", battery, clock);

                sleep(SLEEP);
        }

        /*
         * This code will never be reached (at least at the moment).
         * Nonetheless I regard it good style to implement cleanup code.
         */
        battery_context_close(battery_ctx);
        clock_context_close(clock_ctx);

        return (0);
}
