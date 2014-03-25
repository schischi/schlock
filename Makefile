CFILES = src/main.c   \
		 src/buf.c    \
		 src/cred.c   \
		 src/lock.c
CC = gcc
#CC = clang
EXE = schlock
PREFIX = /usr

CFLAGS=-std=c99
LDFLAGS=-lX11 -lpam

${EXE}:
	$(CC) $(CFILES) $(CFLAGS) $(LDFLAGS) -o $(EXE)

install: ${EXE}
	install -m 0775 ${EXE} ${PREFIX}/bin

clean:
	@rm -rf $(EXE)
