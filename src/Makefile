CFLAGS=-ffast-math -Wall -g -O3 -mfpmath=387
LDFLAGS=
LIBS=-lpthread -ljack -lX11 -lm
OBJS=main.o thread.o event.o thread_audio.o thread_iface.o model.o division.o rank.o
PROG=organ

all: ${PROG} timing_test1

${PROG}:${OBJS}
	cc ${CFLAGS} -o ${PROG} ${OBJS} ${LDFLAGS} ${LIBS}


timing_test1: timing_test1.o division.o rank.o
	cc ${CFLAGS} -o $@ $^ ${LDFLAGS} ${LIBS}
