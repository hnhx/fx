INSTALL_DIR ?= /usr/local/bin
CC = cc
VERSION = 0.1

all:
	${CC} *.c -o fx -Wall

options:
	@echo fx build options:
	@echo "CC = ${CC}"
	@echo "INSTALL_DIR = ${INSTALL_DIR}"

version:
	@echo ${VERSION}

install:
	${CC} *.c -o $(INSTALL_DIR)/fx -Wall

dist:
	mkdir -p fx-${VERSION}
	cp -R LICENSE Makefile README.md config.h fx.c utils.c utils.h fx-${VERSION}
	tar -cf fx-${VERSION}.tar fx-${VERSION}
	gzip fx-${VERSION}.tar
	rm -rf fx-${VERSION}

clean:
	rm -f fx

uninstall:
	${CC} $(INSTALL_DIR)/fx
