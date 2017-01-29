/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <err.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

#include <X11/Xlib.h>

#include "battery.h"
#include "buffers.h"
#include "clock.h"
#include "load.h"
#include "mpd.h"

enum {
	SLEEP = 1,
};

int
main(void)
{
	Display			*dpy;
	Window			 root;
	struct clock_context	*clock_ctx;
	struct battery_context	*battery_ctx;
	struct load_context	*load_ctx;
	struct mpd_context	*mpd_ctx;
	int			 screen;

	if (setlocale(LC_ALL, "") == NULL)
		errx(EXIT_FAILURE, "setlocale()");

	if ((dpy = XOpenDisplay(NULL)) == NULL)
		errx(EXIT_FAILURE, "unable to open display '%s'",
		    XDisplayName(NULL));

	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);

	if ((clock_ctx = clock_context_open()) == NULL)
		errx(EXIT_FAILURE, "clock_context_open()");
	if ((battery_ctx = battery_context_open()) == NULL)
		errx(EXIT_FAILURE, "battery_context_open()");
	if ((load_ctx = load_context_open()) == NULL)
		errx(EXIT_FAILURE, "load_context_open()");
	if ((mpd_ctx = mpd_context_open()) == NULL)
		errx(EXIT_FAILURE, "mpd_context_open()");

	for (;;) {
		wchar_t	 status[STATUS_BUFFLEN];
		char	 mbs_status[STATUS_BUFFLEN];
		wchar_t	*mpd;
		wchar_t	*battery;
		wchar_t	*clock;
		wchar_t	*load;

		if ((clock = clock_str(clock_ctx)) == NULL)
			err(EXIT_FAILURE, "clock_str");
		if ((battery = battery_str(battery_ctx)) == NULL)
			err(EXIT_FAILURE, "clock_str");
		if ((load = load_str(load_ctx)) == NULL)
			err(EXIT_FAILURE, "load_str");
		if ((mpd = mpd_str(mpd_ctx)) == NULL)
			err(EXIT_FAILURE, "mpd_str");

		if (swprintf(status, STATUS_BUFFLEN, L"%S | %S | %S | %S",
		    mpd, load, battery, clock) == 0)
			errx(EXIT_FAILURE, "swprintf");

		if (wcstombs(mbs_status, status, WCSLEN(status)) == (size_t)-1)
			err(EXIT_FAILURE, "wcstombs");

		XStoreName(dpy, root, mbs_status);
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
