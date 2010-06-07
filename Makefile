PROG=		dwm-status
SRCS=		${PROG}.c clock.c battery.c load.c

CSTD?=		c89
WARNS?=		6

CLEANFILES=	*.[Bb][Aa][Kk] *.core
CTAGS=		ctags


NO_MAN=		yes

.include <bsd.prog.mk>
