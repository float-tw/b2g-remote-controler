CROSS?=$(HOME)/arm-toolchain/arm-none-linux/bin/arm-none-linux-gnueabi-gcc
CFLAG=-static -Wall

all: remote_server event_reader

remote_server: remote_server.c
	$(CROSS) $(CFLAG) $< -o $@

event_reader: event_reader.c
	$(CROSS) $(CFLAG) $< -o $@

uinputLib.o: uinputLib.c
	$(CROSS) $(CFLAG) -c $< -o $@

install: install-remote-server install-event-reader

install-remote-server: remote_server
	adb push $< /data/

install-event-reader: event_reader
	adb push $< /data/

clean:
	rm -f remote_server event_reader gsnap
