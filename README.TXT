This is the simple tool I use to refresh my dwm status line. It's self
contained in the way that it doesn't spawn any external processes.

It displays

 - date and time
 - battery level and state
 - load averages
 - the song currently played by MusicPD

It refreshes every second with the exception of the currently played song which
is refreshed every five seconds.

You may us this as is or you can use this as a boilerplate to bootstrap your
own dwm-status tool.

Dependencies:

 - a POSIX system
 - libiconv
 - Xlib
 - libmpdclient

Caveat:

 - The Makefile is a BSD-style makefile using the FreeBSD make infrastructure,
   you may have to provide your own Makefile or compile by hand.

 - You may have to rewrite battery.c if you are not using FreeBSD.

Have fun.
