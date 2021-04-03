PREFIX=~/.local

all:
	gcc ./minfo.c -o ./minfo -Wall
install:
	mkdir -p $(PREFIX)/bin
	cp ./minfo $(PREFIX)/bin/
tc: all
	./minfo
