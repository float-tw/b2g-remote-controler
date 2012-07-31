CC=$(HOME)/arm-toolchain/bin/arm-none-linux-gnueabi-gcc
CFLAG=-static

remote_server: remote_server.c
	$(CC) $(CFLAG) -o $@ $<

event_reader: event_reader.c
	$(CC) $(CFLAG) -o $@ $<


install: install-remote-server install-event-reader

install-remote-server: remote_server
	adb push $< /data/

install-event-reader: event_reader
	adb push $< /data/
