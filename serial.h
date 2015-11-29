#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdio.h>

#define DEFAULT_BAUD        9600
#define DEFAULT_DEV         "/dev/ttyATH0"
#define MAX_BUFFER_SIZE     256

typedef struct tagSerial {
    int fd;
    int baud;
    char *dev;
    
    int bufflen;
    char buff[MAX_BUFFER_SIZE];
    
} Serial;

Serial *serial_new(const char *dev, const int baud);
int serial_init(Serial *serial);
int serial_read(Serial *serial);
int serial_write(const int fd, char *buff, size_t size);
int serial_flush(const int fd);
int serial_close(const int fd);

#endif
