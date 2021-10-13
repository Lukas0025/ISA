#pragma once

#include "debug.h"
#include "addresses.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

// Define the Packet Constants
// ping packet size
#define PING_PACKET_SIZE 64

// Automatic port number
#define PORT_NO 0

// Automatic port number
#define PING_SLEEP_RATE 1000000 x

// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 10

namespace ping {
    typedef struct {
        struct icmphdr hdr;
        char msg[PING_PACKET_SIZE - sizeof(struct icmphdr)];
    } packet_t;

    void send(addresses::addr_t addr, char* data, unsigned len);
    bool send_packet(int sock, struct sockaddr* addr, char *data, unsigned sequence);
    int v4sock();
}