#include "stack.h"

#include <stdio.h>
#include <stddef.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

void print_ethernet_frame(ethernet_frame *frame, size_t total_length) {
    unsigned char *s = (unsigned char *)frame;

    for (size_t i = 0; i < total_length; i++) {
        printf("%02x ", s[i]);

        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
}