#include "serial.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>

Serial *serial_new(const char *dev, const int baud)
{
    Serial *serial = NULL;
    
    serial = (Serial *)malloc(sizeof(Serial));
    if(serial == NULL) {
        printf("serial is NULL, mem not enough!\n");
        exit(1);
    }
    serial->fd = 0;
    serial->dev = strdup(dev);
    serial->baud = baud;
    return serial;
}

int serial_init(Serial *serial)
{
    struct termios toptions;
    int fd;
    
    if(serial->dev == NULL) {
        printf("dev is null");
        return -1;
    }
    
    fd = open(serial->dev, O_RDWR | O_NONBLOCK );
    
    if (fd == -1)  {
        printf("serial_init: Unable to open port %s\n", serial->dev);
        return -1;
    }
    
    if (tcgetattr(fd, &toptions) < 0) {
        printf("serial_init: Couldn't get term attributes\n");
        return -1;
    }

    speed_t brate = serial->baud;

    switch(serial->baud) {
    case 4800:   brate = B4800;   break;
    case 9600:   brate = B9600;   break;
#ifdef B14400
    case 14400:  brate = B14400;  break;
#endif
    case 19200:  brate = B19200;  break;
#ifdef B28800
    case 28800:  brate = B28800;  break;
#endif
    case 38400:  brate = B38400;  break;
    case 57600:  brate = B57600;  break;
    case 115200: brate = B115200; break;
    
    default:
        printf("baud not supported\n");
        return -1;
    }
    
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;

    toptions.c_cflag &= ~CRTSCTS; // no flow control

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 0;
    
    tcsetattr(fd, TCSANOW, &toptions);
    if( tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
        printf("could not set term attributes\n");
        return -1;
    }

    serial->fd = fd;
    return fd;
}

int serial_read(Serial *serial)
{
    if(serial == NULL) {
        printf("serial is null\n");
        return -1;
    }
    serial->bufflen = read(serial->fd, serial->buff, MAX_BUFFER_SIZE);
    return serial->bufflen;
}

int serial_write(const int fd, char *buff, size_t size)
{
    return write(fd, buff, size);
}

int serial_flush(const int fd)
{
    int ret;
    usleep(10000); 
    ret = tcflush(fd, TCIOFLUSH);
    usleep(1000000); 
    return ret;
}

int serial_close(const int fd)
{
    return close(fd);
}
