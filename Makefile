CC	=gcc
CFLAGS	=-Wall -Werror -pedantic -Os
LDFLAGS	=
LDLIBS	=

BINDIR	=${HOME}/bin

all:	crc16

crc16:	crc16.h main.c
	${CC} ${CFLAGS} ${LDFLAGS} -o crc16 main.c ${LDLIBS}

main.s: main.c
	${CC} ${CFLAGS} -o $@ -S $<

checks: crc16 msg msg-ascii
	./crc16 msg
	./crc16 -a msg-ascii

clean:
	${RM} *.s *.o a.out core

distclean clobber: clean
	${RM} crc16

install: crc16
	install -d ${BINDIR}
	install -c -s crc16 ${BINDIR}

uninstall:
	${RM} ${BINDIR}/crc16
