#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <time.h>

#include "uinputLib.h"

void touch(int fd, int x, int y)
{
	// touch on one point
	struct input_event ev;
	memset(&ev, 0, sizeof(ev));
	
	ev.type = EV_ABS;
	ev.code = ABS_MT_POSITION_X;
	ev.value = x;
	write(fd, &ev, sizeof(struct input_event));
	ev.type = EV_ABS;
	ev.code = ABS_MT_POSITION_Y;
	ev.value = y;
	write(fd, &ev, sizeof(struct input_event));
	ev.type = EV_ABS;
	ev.code = ABS_MT_PRESSURE;
	ev.value = 53;
	write(fd, &ev, sizeof(struct input_event));
	ev.type = EV_ABS;
	ev.code = ABS_MT_TOUCH_MAJOR;
	ev.value = 4;
	write(fd, &ev, sizeof(struct input_event));
	ev.type = EV_ABS;
	ev.code = ABS_MT_TRACKING_ID;
	ev.value = 0;
	write(fd, &ev, sizeof(struct input_event));
	ev.type = EV_SYN;
	ev.code = SYN_MT_REPORT;
	ev.value = 0;
	write(fd, &ev, sizeof(struct input_event));
	ev.type = EV_SYN;
	ev.code = SYN_REPORT;
	ev.value = 0;
	write(fd, &ev, sizeof(struct input_event));
	return;
}

void syn(int fd)
{
	// leave touch pad
	struct input_event ev;
	memset(&ev, 0, sizeof(ev));
	write(fd, &ev, sizeof(struct input_event));
	ev.type = EV_SYN;
	ev.code = SYN_MT_REPORT;
	ev.value = 0;
	write(fd, &ev, sizeof(struct input_event));
	ev.type = EV_SYN;
	ev.code = SYN_REPORT;
	ev.value = 0;
	write(fd, &ev, sizeof(struct input_event));
	return;
}

int init_uinput()
{
	// init and creat uinput device
	int fd;
	struct uinput_user_dev uidev;
	printf("init uinput");

	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if( fd  < 0 )
	{
		printf("open /dev/uinput error\n");
		return 1;
	}

	ioctl(fd, UI_SET_EVBIT, EV_ABS);
	ioctl(fd, UI_SET_EVBIT, EV_SYN);

	ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_PRESSURE);
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);

	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput");

	uidev.id.bustype = BUS_VIRTUAL;
	uidev.id.vendor = 0;
	uidev.id.product = 0;
	uidev.id.version = 0;
	uidev.absmin[ABS_MT_POSITION_X] = 0;
	uidev.absmax[ABS_MT_POSITION_X] = MAX_X;
	uidev.absmin[ABS_MT_POSITION_Y] = 0;
	uidev.absmax[ABS_MT_POSITION_Y] = MAX_Y;
	uidev.absflat[ABS_MT_POSITION_X] = 0;
	uidev.absflat[ABS_MT_POSITION_Y] = 0;
	uidev.absfuzz[ABS_MT_POSITION_X] = 0;
	uidev.absfuzz[ABS_MT_POSITION_Y] = 0;

	write(fd, &uidev, sizeof(uidev));

	// creat uinput device
	ioctl(fd, UI_DEV_CREATE);

	return fd;
}

void close_uinput(int fd)
{
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
	return;
}

#if 0

int main()
{
	int fd;
	int ret;
	int i;
	struct uinput_user_dev uidev;
	struct input_event ev[7];
	struct input_absinfo info;

	/*
	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if( fd  < 0 )
	{
		printf("open /dev/uinput error\n");
		return 1;
	}

	// creating an input device
	
	ret = ioctl(fd, UI_SET_EVBIT, EV_ABS);
	ret = ioctl(fd, UI_SET_EVBIT, EV_SYN);

	ret = ioctl(fd, UI_SET_ABSBIT, ABS_X);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_Y);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_PRESSURE);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);

	memset(&uidev, 0, sizeof(uidev));

	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
	//uidev.id.bustype = BUS_USB;
	uidev.id.bustype = BUS_VIRTUAL;
	uidev.id.vendor = 0;
	uidev.id.product = 0;
	uidev.id.version = 0;
	uidev.absmin[ABS_X] = 0;
	uidev.absmax[ABS_X] = 1024;
	uidev.absmin[ABS_Y] = 0;
	uidev.absmax[ABS_Y] = 1024;
	uidev.absmin[ABS_MT_POSITION_X] = 0;
	uidev.absmax[ABS_MT_POSITION_X] = 1024;
	uidev.absmin[ABS_MT_POSITION_Y] = 0;
	uidev.absmax[ABS_MT_POSITION_Y] = 1024;
	uidev.absmin[ABS_RX] = -1024;
	uidev.absmax[ABS_RX] = 1024;
	uidev.absmin[ABS_RY] = -1024;
	uidev.absmax[ABS_RY] = 1024;
	uidev.absflat[ABS_X] = 0;
	uidev.absflat[ABS_Y] = 0;
	uidev.absfuzz[ABS_X] = 0;
	uidev.absfuzz[ABS_Y] = 0;
	

	ret = write(fd, &uidev, sizeof(uidev));

	

	ret = ioctl(fd, UI_DEV_CREATE);
	*/
	fd = init_uinput();

	//scanf("%d", &ret);
	sleep(1);

	memset(ev, 0, sizeof(ev));


	//ioctl(fd, EVIOCGABS(ABS_X), &info);
	//printf("%d %d\n", info.minimum, info.maximum);
	//ioctl(fd, EVIOCGABS(ABS_Y), &info);
	//printf("%d %d\n", info.minimum, info.maximum);

	srand(time(NULL));

	for(i=0; i<1; i++)
	{
		//touch(fd, rand()%1024, rand()%1024);
		touch(fd, 512, 512);
		usleep(500000);
	}
	syn(fd);

	sleep(1);

	// destroying an input device
	ret = ioctl(fd, UI_DEV_DESTROY);

	close(fd);
	return 0;
}

#endif
