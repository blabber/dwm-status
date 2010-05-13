PROG=		dwm-status
SRCS=		${PROG}.c clock.c battery.c

CSTD?=		c89
WARNS?=		6
WFORMAT?=	1
NO_WERROR=	yes

CLEANFILES=	*.[Bb][Aa][Kk] *.core
CTAGS=		ctags


NO_MAN=		yes

.include <bsd.prog.mk>
