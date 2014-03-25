SCHLOCK_CFILES = schlock-src/main.c   \
		 		 schlock-src/buf.c    \
		 		 schlock-src/cred.c   \
		 		 schlock-src/lock.c   \
		 		 common/xutils.c

SCHLOCK_SVG_CFILES = schlock-svg-src/main.c \
					 common/xutils.c
CC = gcc
#CC = clang
PREFIX = /usr

SCHLOCK_EXE = schlock
SCHLOCK_CFLAGS=-std=c99
SCHLOCK_LDFLAGS=-lX11 -lpam -lXrandr

SCHLOCK_SVG_EXE = schlock-svg
SCHLOCK_SVG_CFLAGS= $(shell pkg-config --cflags xext x11 cairo-xlib-xrender librsvg-2.0)
SCHLOCK_SVG_LDFLAGS= $(shell pkg-config --libs xrandr xext x11 cairo-xlib-xrender librsvg-2.0)

all: ${SCHLOCK_EXE} ${SCHLOCK_SVG_EXE}

${SCHLOCK_EXE}: clean
	$(CC) $(SCHLOCK_CFILES) $(SCHLOCK_CFLAGS) $(SCHLOCK_LDFLAGS) -o $(SCHLOCK_EXE)

${SCHLOCK_SVG_EXE}: clean
	$(CC) $(SCHLOCK_SVG_CFILES) $(SCHLOCK_SVG_CFLAGS) $(SCHLOCK_SVG_LDFLAGS) -o $(SCHLOCK_SVG_EXE)

#install: ${EXE}
#	install -m 0775 ${EXE} ${PREFIX}/bin

clean:
	@rm -rf $(SCHLOCK_EXE)
	@rm -rf $(SCHLOCK_SVG_EXE)
