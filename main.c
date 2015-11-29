#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHELL_SCRIPT "./wificonf.sh"

static void parse_serial_data(const Serial *const serial);

int main(int argc, char *argv[])
{
    Serial *serial;
    fd_set fdset;
    struct timeval tv;
    int ret;
    char *dev;
    
    if(argc < 2) {
        dev = DEFAULT_DEV;
    } else {
        dev = argv[1];
    }
    
    serial = serial_new(dev, DEFAULT_BAUD);
    if(serial_init(serial) < 0) {
        exit(1);
    }
    
    printf("serial ok, waiting for data...\n");
    
    while(1) {
        FD_ZERO(&fdset);
        FD_SET(serial->fd, &fdset);
        
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        ret = select(serial->fd + 1, &fdset, NULL, NULL, &tv);
        if(ret < 0) {
            printf("select error\n");
            break;
        } else if(ret == 0) {
            continue;
        }
        
        if(FD_ISSET(serial->fd, &fdset)) {
            if(serial_read(serial) > 0) {
                parse_serial_data(serial);
            }
        }
    }
    serial_close(serial->fd);
    
    return 0;
}

static void parse_serial_data(const Serial *const serial)
{
    FILE *fp;
    char output[512];
    char cmdline[512] = {0};
    
    if(strlen(SHELL_SCRIPT) + serial->bufflen > sizeof(cmdline) - 1) {
        printf("bad args");
        return;
    }
    
    sprintf(cmdline, "%s %s", SHELL_SCRIPT, serial->buff);
    
    printf("%s\n", cmdline);
    fp = popen(cmdline, "r");
    if(fp == NULL) {
        return;
    }
    
    while(fgets(output, sizeof(output), fp) != NULL) {
        printf("%s", output);
        serial_write(serial->fd, output, strlen(output));
    }
}
