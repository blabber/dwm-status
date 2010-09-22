PROG=		dwm-status
SRCS=		${PROG}.c clock.c battery.c load.c mpd.c

CSTD?=		c89
WARNS?=		6

CFLAGS+=	-I/usr/local/include/
LDFLAGS+=	-L/usr/local/lib
LDADD+=		-lmpdclient -liconv

CLEANFILES=	*.[Bb][Aa][Kk] *.core
CTAGS=		ctags


NO_MAN=		yes

.include <bsd.prog.mk>
