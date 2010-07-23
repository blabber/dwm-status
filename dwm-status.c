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
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

#include "battery.h"
#include "clock.h"
#include "load.h"

enum {
        SLEEP = 3,
        STRLEN = 80
};

int
main(void)
{
        struct clock_context *clock_ctx;
        struct battery_context *battery_ctx;
        struct load_context *load_ctx;
        char           *command;

        if (setlocale(LC_ALL, "") == NULL)
                err(EX_SOFTWARE, "setlocale()");

        if ((clock_ctx = clock_context_open()) == NULL)
                err(EX_SOFTWARE, "clock_context_open()");
        if ((battery_ctx = battery_context_open()) == NULL)
                err(EX_SOFTWARE, "battery_context_open()");
        if ((load_ctx = load_context_open()) == NULL)
                err(EX_SOFTWARE, "load_context_open()");

        if ((command = malloc(STRLEN)) == NULL)
                err(EX_SOFTWARE, "malloc command");

        for (;;) {
                char           *clock;
                char           *battery;
                char           *load;

                if ((clock = clock_str(clock_ctx)) == NULL)
                        err(EX_SOFTWARE, "clock_str");
                if ((battery = battery_str(battery_ctx)) == NULL)
                        err(EX_SOFTWARE, "clock_str");
                if ((load = load_str(load_ctx)) == NULL)
                        err(EX_SOFTWARE, "load_str");


                snprintf(command, STRLEN, "xsetroot -name '| %s | %s | %s'", load, battery, clock);
                system(command);/* errorhandling */

                sleep(SLEEP);
        }

        /*
         * This code will never be reached (at least at the moment).
         * Nonetheless I regard it good style to implement cleanup code.
         */
        free(command);
        load_context_close(load_ctx);
        battery_context_close(battery_ctx);
        clock_context_close(clock_ctx);

        return (0);
}
