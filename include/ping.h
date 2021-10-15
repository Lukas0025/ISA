/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * generic ping header file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#pragma once

#include <stdexcept>
#include <errno.h>
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

    /**
     * Interface for ping clients
     */
    class ping_client {
        public:
            ping_client() {}
            
            /**
             * Send data using ping (ICMP_ECHO) packet
             * @param data      pointer on data to send
             * @param data_len  size of data to send in Bytes
             * @param sequence  id of ping packet
             * @return bool, true if send, false if not send
             */
            virtual bool send(char *data, unsigned data_len, unsigned sequence) = 0;
    };

    /**
     * Open ping on address
     * @param addr address for open ping (INET6 = IPv6 and INET = IPv4) 
     * @return ping_client*
     * @try runtime_error if fail
     */
    ping_client* open(addresses::addr_t addr);
}