#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include "uinputLib.h"

#define PORT 2222

int main()
{
	char *fbdev = "/dev/graphics/fb0";
	int fd;
	int socket_fd, accept_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	struct fb_var_screeninfo fbinfo;
	int sin_size;
	int on;
	int x, y;
	int screen_w, screen_h;
	int in_buff_len, read_len;
	char buff[128];

#if 0
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

	on = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

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
	if ( (accept_fd = accept(socket_fd, (struct sockaddr*)&their_addr, (socklen_t*)&sin_size)) == -1 ){
		perror("accept");
		exit(1);
	}
#endif

int i;
#if 0
for(i=0;i<0;i++){
	// get fb info
	fd = open(fbdev, O_RDWR);
printf("fd = %d\n", fd);
	if(fd < 0){
		fprintf(stderr, "cannot open %s\n", fbdev);
		return 1;
	}
	if(ioctl(fd, FBIOGET_VSCREENINFO, &fbinfo) < 0){
		fprintf(stderr, "failed to get fbinfo: %s\n", strerror(errno));
		return 1;
	}
	screen_w = fbinfo.xres;
	screen_h = fbinfo.yres;
	printf("screen : %d %d\n", screen_w, screen_h);
	close(fd);
}
#endif

	// init uinput
	fd = init_uinput();
	printf("fd = %d\n", fd);
	sleep(100);

#if 0
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
			syn(fd);
		else
		{
			if(x<0 && y<0)
				break;
			printf("%d %d\n", x, y);
			x = x*1024/screen_w;
			y = y*941/screen_h;
			touch(fd, x, y);
		}
	}
#endif

	close_uinput(fd);
	//close(accept_fd);
	//close(socket_fd);

	return 0;
}
