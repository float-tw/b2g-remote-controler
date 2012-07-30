#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <fcntl.h>

int main()
{
	int fd;
	int ret;
	int i;
	struct input_event ev;

	fd = open("/dev/input/event0", O_RDONLY);
	if( fd  < 0 )
	{
		printf("error\n");
		return 1;
	}

	while(1)
	{
		read(fd, &ev, sizeof(struct input_event));
		printf("%d %d %d\n", ev.type, ev.code, ev.value);
	}
	return 0;
}
