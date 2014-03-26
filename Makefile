SCHLOCK_CFILES = schlock-src/main.c   \
		 		 schlock-src/buf.c    \
		 		 schlock-src/cred.c   \
		 		 schlock-src/lock.c   \
		 		 common/xutils.c

SCHLOCK_SVG_CFILES = schlock-svg-src/main.c   \
					 schlock-svg-src/config.c \
					 common/xutils.c
CC = gcc
#CC = clang
PREFIX = /usr

SCHLOCK_EXE = schlock
SCHLOCK_CFLAGS=-std=c99
SCHLOCK_LDFLAGS=-lX11 -lpam -lXrandr

SCHLOCK_SVG_EXE = schlock-svg
SCHLOCK_SVG_CFLAGS= $(shell pkg-config --cflags xext x11 cairo-xlib-xrender librsvg-2.0 libconfig)
SCHLOCK_SVG_LDFLAGS= $(shell pkg-config --libs xrandr xext x11 cairo-xlib-xrender librsvg-2.0 libconfig)

all: clean ${SCHLOCK_EXE} ${SCHLOCK_SVG_EXE}

${SCHLOCK_EXE}:
	$(CC) $(SCHLOCK_CFILES) $(SCHLOCK_CFLAGS) $(SCHLOCK_LDFLAGS) -o $(SCHLOCK_EXE)

${SCHLOCK_SVG_EXE}:
	$(CC) $(SCHLOCK_SVG_CFILES) $(SCHLOCK_SVG_CFLAGS) $(SCHLOCK_SVG_LDFLAGS) -o $(SCHLOCK_SVG_EXE)

install: ${SCHLOK_EXE} ${SCHLOCK_SVG_EXE}
	install -m 0775 ${SCHLOCK_EXE} ${PREFIX}/bin
	install -m 0775 ${SCHLOCK_SVG_EXE} ${PREFIX}/bin
	mkdir -p /usr/share/schlock
	install -m 0664 res/config /usr/share/schlock/
	install -m 0664 res/hmm_rage.svg /usr/share/schlock/
	install -m 0664 res/hmm.svg /usr/share/schlock/

clean:
	@rm -rf $(SCHLOCK_EXE)
	@rm -rf $(SCHLOCK_SVG_EXE)
