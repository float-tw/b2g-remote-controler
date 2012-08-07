#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>

int main()
{
	int fd;
	int ret;
	int i;
	struct uinput_user_dev uidev;
	struct input_event ev[7];

	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if( fd  < 0 )
	{
		printf("open /dev/uinput error\n");
		return 1;
	}

	// creating an input device
	
	ret = ioctl(fd, UI_SET_EVBIT, EV_ABS);

	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_PRESSURE);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);

	ret = ioctl(fd, UI_SET_EVBIT, EV_KEY);

	memset(&uidev, 0, sizeof(uidev));

	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor = 0x1234;
	uidev.id.product = 0xfedc;
	uidev.id.version = 1;
	uidev.absmin[ABS_X] = 0;
	uidev.absmax[ABS_X] = 1024;
	uidev.absmin[ABS_Y] = 0;
	uidev.absmax[ABS_Y] = 1024;

	ret = write(fd, &uidev, sizeof(uidev));
	ret = ioctl(fd, UI_DEV_CREATE);

	//scanf("%d", &ret);
	sleep(10);

	memset(ev, 0, sizeof(ev));

	ev[0].type = EV_ABS;
	ev[0].code = ABS_MT_POSITION_X;
	ev[0].value = 512;
	ev[1].type = EV_ABS;
	ev[1].code = ABS_MT_POSITION_Y;
	ev[1].value = 512;
	ev[2].type = EV_ABS;
	ev[2].code = ABS_MT_PRESSURE;
	ev[2].value = 53;
	ev[3].type = EV_ABS;
	ev[3].code = ABS_MT_TOUCH_MAJOR;
	ev[3].value = 4;
	ev[4].type = EV_ABS;
	ev[4].code = ABS_MT_TRACKING_ID;
	ev[4].value = 0;
	ev[5].type = EV_SYN;
	ev[5].code = SYN_MT_REPORT;
	ev[5].value = 0;
	ev[6].type = EV_SYN;
	ev[6].code = SYN_REPORT;
	ev[6].value = 0;


	for(i=0; i<7; i++)
	{
		ret = write(fd, &ev[i], sizeof(struct input_event));
		printf("%d\n", ret);
	}

	sleep(10);

	// destroying an input device
	ret = ioctl(fd, UI_DEV_DESTROY);

	close(fd);
	return 0;
}
