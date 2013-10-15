PROG=		dwm-status
SRCS=		${PROG}.c clock.c battery.c load.c mpd.c tools.c bsd.c

CSTD?=		c89
WARNS?=		6

CFLAGS+=	-I/usr/local/include/
LDFLAGS+=	-L/usr/local/lib
LDADD+=		-lmpdclient -lX11 ${ICONV_LIB}

CLEANFILES=	*.[Bb][Aa][Kk] *.core
CTAGS=		ctags


NO_MAN=		yes

.include <bsd.prog.mk>
