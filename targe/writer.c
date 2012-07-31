#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>                                                                                                                  
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define PORT 2222

int send_event(int fd, struct input_event* ev, int len)
{
	int i;
	for(i=0; i<len; ++i)
		write(fd, &ev[i], sizeof(struct input_event));
	return 0;
}

int main()
{
	int fd;
	int socket_fd, accept_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
	int ret;
	int x, y;
	int in_buff_len, read_len;
	char buff[128];
	struct input_event ev[7];

	// TCP socket
	if ( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
		perror("socket");
		exit(1);
	}

	// Initail
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero( &(my_addr.sin_zero), 8 );

	// Binding
	if ( bind(socket_fd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1 ){
		perror("bind");
		exit(1);
	}

	// Listening
	if ( listen(socket_fd, 1) == -1 ){
		perror("listen");
		exit(1);
	}

	// Accept
	sin_size = sizeof(struct sockaddr_in);
	if ( (accept_fd = accept(socket_fd, (struct sockaddr*)&their_addr, &sin_size)) == -1 ){
		perror("accept");
		exit(1);
	}

	// open event0
	fd = open("/dev/input/event0", O_WRONLY | O_NONBLOCK);
	if( fd < 0 ){
		perror("open event0");
		exit(1);
	}

	// init event
	ev[0].type = EV_ABS;
	ev[0].code = ABS_MT_POSITION_X;
	ev[0].value = 0;
	ev[1].type = EV_ABS;
	ev[1].code = ABS_MT_POSITION_Y;
	ev[1].value = 0;
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

	// read command
	while(1)
	{
		in_buff_len = 0;
		read_len = 0;
		do
		{
			in_buff_len += read_len;
			read_len = read(accept_fd, buff + in_buff_len, 1);
			if( read_len < 0 )
				break;
		}while( buff[in_buff_len] != '\n' );
		if( read_len < 0 )
			break;
		buff[in_buff_len + 1] = '\0';
		
		if( sscanf(buff, "%d %d", &x, &y) != 2 )
			send_event(fd, &ev[5], 2);
		else
		{
			if(x<0 && y<0)
				break;
			printf("%d %d\n", x, y);
			ev[0].value = x*1024/480;
			ev[1].value = y*1024/800;
			send_event(fd, ev, 7);
		}
	}

	close(fd);
	close(socket_fd);

	return 0;
}
