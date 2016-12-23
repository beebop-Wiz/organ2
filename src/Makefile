CFLAGS=-ffast-math
LDFLAGS=
LIBS=-lpthread -ljack -lX11
OBJS=main.o thread.o event.o thread_audio.o thread_iface.o model.o division.o
PROG=organ

${PROG}:${OBJS}
	cc ${CFLAGS} -o ${PROG} ${OBJS} ${LDFLAGS} ${LIBS}
