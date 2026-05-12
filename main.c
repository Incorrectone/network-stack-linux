/*
 * Major help from:
 * https://blog.cloudflare.com/virtual-networking-101-understanding-tap/
 * https://backreference.org/2010/03/26/tuntap-interface-tutorial/
 * Hopefully works and does not nuke my internet.
 */

#define MTU 1518

#include "tun_alloc.h"
#include "stack.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/if_ether.h>
#include <netinet/in.h>


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

    ethernet_frame received_frame;

    while(1) {

        /* Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes */
        int nread = read(tap_fd, &received_frame, sizeof(received_frame));

        if(nread < 0) {
            perror("Reading from interface");
            close(tap_fd);
            exit(1);
        }

        /* Do whatever with the data */
        uint16_t ether_type = ntohs(received_frame.ethernet_header.h_proto);

        switch (ether_type){
            case(ETH_P_ARP):
                process_arp(tap_fd, &received_frame, nread);
                break;
            default:
                print_ethernet_frame(&received_frame, nread);
                break;
        }
    }

}