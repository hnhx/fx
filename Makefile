INSTALL_DIR ?= /usr/local/bin

all:
	cc *.c -o fx -Wall

install:
	cc *.c -o $(INSTALL_DIR)/fx -Wall

uninstall:
	rm $(INSTALL_DIR)/fx
