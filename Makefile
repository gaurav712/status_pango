CC="gcc"
CFLAGS="-Wall"

all:
	$(CC) -o status_pango $(CFLAGS) status_pango.c

clean:
	rm -f status_pango

install: all
	cp status_pango /usr/local/bin/status_pango

uninstall:
	rm -f /usr/local/bin/status_pango
