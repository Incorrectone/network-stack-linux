#include "stack.h"

#include <stddef.h>
#include <stdio.h>

void process_arp(int tap_fd, ethernet_frame * received_frame, size_t total_length) {
    printf("Arp frame: \n");
    print_ethernet_frame(received_frame, total_length);


}