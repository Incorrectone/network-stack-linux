/*
 * Major help from:
 * https://blog.cloudflare.com/virtual-networking-101-understanding-tap/
 * https://backreference.org/2010/03/26/tuntap-interface-tutorial/
 * Hopefully works and does not nuke my internet.
 */

#define MTU 1518

#include "tun_alloc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/if.h>
#include <linux/if_tun.h>

char tap_name[IFNAMSIZ];

int main(int argc, char *argv[]) {
    if (argc > 2 || argc == 1) {
        printf("Usage:");
        printf("Command: custom_stack <interface name>");
        exit(1);
    }
    char * interface_name = argv[1];

    int tap_fd;

    strcpy(tap_name, interface_name);

    tap_fd = tun_alloc(tap_name, IFF_TAP | IFF_NO_PI);

    if(tap_fd < 0){
        perror("Allocating interface");
        exit(1);
    }

    char buffer[MTU];

    while(1) {

        /* Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes */
        int nread = read(tap_fd, buffer, sizeof(buffer));

        if(nread < 0) {
            perror("Reading from interface");
            close(tap_fd);
            exit(1);
        }

        /* Do whatever with the data */
        printf("Read %d bytes from device %s\n", nread, tap_name);
    }

}