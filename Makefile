PROG=		dwm-status
SRCS=		${PROG}.c clock.c battery.c load.c mpd.c bsd.c

CSTD?=		c99
WARNS?=		6

CFLAGS+=	-I/usr/local/include/
LDFLAGS+=	-L/usr/local/lib
LDADD+=		-lmpdclient -lX11

CLEANFILES=	*.[Bb][Aa][Kk] *.core
CTAGS=		ctags

MK_MAN=		no

.include <bsd.prog.mk>
