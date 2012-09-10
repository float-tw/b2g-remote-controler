#include "uinputLib.h"
#include <unistd.h>
#include <stdio.h>

int main()
{
	int fd;
	sleep(10);// this cause uinput creat fail
	fd = init_uinput();
	printf("fd = %d\n", fd);
	touch(fd, 240, 400);
	syn(fd);
	sleep(10);
	close_uinput(fd);
	return 0;
}
