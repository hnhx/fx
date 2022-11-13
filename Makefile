PREFIX = /usr/local

SRC = fx.c util.c

all: fx

fx: config.h util.c
	$(CC) $(SRC) -o fx $(CFLAGS)

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin 
	cp -f fx $(DESTDIR)$(PREFIX)/bin 
	chmod 755 $(DESTDIR)$(PREFIX)/bin/fx

clean:
	rm -f fx

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/fx
