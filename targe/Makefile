CC=gcc
TOOLCHAIN=$(HOME)/arm-toolchain/bin/arm-none-linux-gnueabi-gcc

host: main.c
	$(CC) -o main main.c

targe: main.c
	$(TOOLCHAIN) -static -o main main.c

install: main
	adb push main /data/

reader: reader.c
	$(TOOLCHAIN) -static -o reader reader.c

install-reader: reader
	adb push reader /data/

