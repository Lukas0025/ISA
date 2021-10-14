#pragma once

#include "debug.h"
#include "addresses.h"

// Define the Packet Constants
// ping packet size
#define	MAXPACKET   64
//#define	MAXPACKET	128000

// Automatic port number
#define PORT_NO 0

// Automatic port number
#define PING_SLEEP_RATE 1000000 x

// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 10

namespace ping {
    /*typedef struct {
        struct icmphdr hdr;
        char msg[MAXPACKET - sizeof(struct icmphdr)];
    } packet4_t;

    typedef struct {
        struct icmp6_hdr hdr;
        char msg[MAXPACKET - sizeof(struct icmphdr)];
    } packet6_t;*/

    class ping_client {
        public:
            ping_client() {}
            virtual bool send(char *data, unsigned data_len, unsigned sequence) = 0;
    };

    ping_client* open(addresses::addr_t addr);
}