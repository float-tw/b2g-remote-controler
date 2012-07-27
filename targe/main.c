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
	struct input_event ev[5];

	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if( fd  < 0 )
	{
		printf("open /dev/uinput error\n");
		return 1;
	}

	// creating an input device
	
	ret = ioctl(fd, UI_SET_EVBIT, EV_ABS);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_X);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_Y);

	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_PRESSURE);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
	ret = ioctl(fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);

	//ret = ioctl(fd, UI_SET_EVBIT, EV_REL);
	//ret = ioctl(fd, UI_SET_RELBIT, REL_X);
	//ret = ioctl(fd, UI_SET_RELBIT, REL_Y);

	ret = ioctl(fd, UI_SET_EVBIT, EV_KEY);
	//ret = ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH);
	//printf("%d\n", ret);
	//ret = ioctl(fd, UI_SET_KEYBIT, BTN_MOUSE);
	//ret = ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
	//ret = ioctl(fd, UI_SET_KEYBIT, KEY_LEFT);

	memset(&uidev, 0, sizeof(uidev));

	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor = 0x1234;
	uidev.id.product = 0xfedc;
	uidev.id.version = 1;
	uidev.absmin[ABS_X] = 0;
	uidev.absmax[ABS_X] = 480;
	uidev.absmin[ABS_Y] = 0;
	uidev.absmax[ABS_Y] = 800;
	//uidev.absmin[REL_X] = 0;
	//uidev.absmax[REL_X] = 65535;
	//uidev.absmin[REL_Y] = 0;
	//uidev.absmax[REL_Y] = 65535;

	ret = write(fd, &uidev, sizeof(uidev));
	ret = ioctl(fd, UI_DEV_CREATE);

	//scanf("%d", &ret);
	sleep(1);

	memset(ev, 0, sizeof(ev));

	ev[0].type = EV_ABS;
	ev[0].code = ABS_X;
	ev[0].value = 240;
	ev[1].type = EV_ABS;
	ev[1].code = ABS_Y;
	ev[1].value = 400;
	ev[2].type = EV_KEY;
	ev[2].code = BTN_LEFT;
	ev[2].value = 1;
	ev[3].type = EV_KEY;
	ev[3].code = BTN_LEFT;
	ev[3].value = 0;
	ev[4].type = EV_SYN;
	ev[4].code = SYN_REPORT;
	ev[4].value = 0;

	//ev[0].type = EV_REL;
	//ev[0].code = REL_X;
	//ev[0].value = -1000;
	//ev[1].type = EV_REL;
	//ev[1].code = REL_Y;
	//ev[1].value = -1000;
	//ev[2].type = EV_KEY;
	//ev[2].code = KEY_LEFT;
	//ev[2].value = 0;
	//ev[3].type = EV_SYN;
	//ev[3].code = SYN_REPORT;
	//ev[3].value = 0;
	

	for(i=0; i<5; i++)
	{
		ret = write(fd, &ev[i], sizeof(struct input_event));
		printf("%d\n", ret);
	}

	sleep(1);

	// destroying an input device
	ret = ioctl(fd, UI_DEV_DESTROY);

	close(fd);
	return 0;
}
