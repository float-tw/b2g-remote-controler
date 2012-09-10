#ifndef __UINPUTLIB_H__
#define __UINPUTLIB_H__

#define MAX_X (1024)
#define MAX_Y (1024)

void touch(int fd, int x, int y);
void syn(int fd);
int init_uinput();
void close_uinput(int fd);

#endif//__UINPUTLIB_H__
