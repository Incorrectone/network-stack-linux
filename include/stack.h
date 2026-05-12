#pragma once

#ifndef ETHER_H
#define ETHER_H

#include <stdint.h>
#include <stddef.h>
#include <linux/if_ether.h>

// Standard Ethernet Frame without CRC Check
typedef struct {
    struct ethhdr ethernet_header;
     uint8_t payload[1500];
} ethernet_frame;

void print_ethernet_frame(ethernet_frame *frame, size_t total_length);

void process_arp(int tap_fd, ethernet_frame * received_frame, size_t total_length);

#endif