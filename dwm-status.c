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

#include <X11/Xlib.h>

#include "battery.h"
#include "clock.h"
#include "load.h"
#include "mpd.h"

enum {
        SLEEP = 1,
        STRLEN = 112,
};

int
main(void)
{
        Display        *dpy;
        Window          root;
        struct clock_context *clock_ctx;
        struct battery_context *battery_ctx;
        struct load_context *load_ctx;
        struct mpd_context *mpd_ctx;
        int             screen;

        if (setlocale(LC_ALL, "") == NULL)
                errx(EX_SOFTWARE, "setlocale()");

        if ((dpy = XOpenDisplay(NULL)) == NULL) {
                errx(EX_SOFTWARE, "unable to open display '%s'", XDisplayName(NULL));
        }
        screen = DefaultScreen(dpy);
        root = RootWindow(dpy, screen);


        if ((clock_ctx = clock_context_open()) == NULL)
                errx(EX_SOFTWARE, "clock_context_open()");
        if ((battery_ctx = battery_context_open()) == NULL)
                errx(EX_SOFTWARE, "battery_context_open()");
        if ((load_ctx = load_context_open()) == NULL)
                errx(EX_SOFTWARE, "load_context_open()");
        if ((mpd_ctx = mpd_context_open()) == NULL)
                errx(EX_SOFTWARE, "mpd_context_open()");

        for (;;) {
                char            status[STRLEN];
                char           *clock;
                char           *battery;
                char           *load;
                char           *mpd;

                if ((clock = clock_str(clock_ctx)) == NULL)
                        err(EX_SOFTWARE, "clock_str");
                if ((battery = battery_str(battery_ctx)) == NULL)
                        err(EX_SOFTWARE, "clock_str");
                if ((load = load_str(load_ctx)) == NULL)
                        err(EX_SOFTWARE, "load_str");
                if ((mpd = mpd_str(mpd_ctx)) == NULL)
                        err(EX_SOFTWARE, "mpd_str");

                snprintf(status, sizeof(status), "%s | %s | %s | %s", mpd, load, battery, clock);
                XStoreName(dpy, root, status);
                XFlush(dpy);

                sleep(SLEEP);
        }

        /*
         * This code will never be reached (at least at the moment).
         * Nonetheless I regard it good style to implement cleanup code.
         */
        mpd_context_close(mpd_ctx);
        load_context_close(load_ctx);
        battery_context_close(battery_ctx);
        clock_context_close(clock_ctx);
        XCloseDisplay(dpy);

        return (0);
}
