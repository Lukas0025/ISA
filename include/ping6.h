/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * ping on IPv6 header file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#pragma once

#include "debug.h"
#include "addresses.h"
#include "ping.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>

namespace ping {

    /**
     * ICMP6 header type
     */
    typedef struct icmp6_hdr icmp6_hdr;

    /**
     * Ping client on IPv6, ping_client interface realization
     */
    class ping6_client: public ping_client {
        public:
            
            /**
             * Conscructor
             * @param addr addresses::addr_t only IPv6 type (INET6)
             * @try runtime_error if fail
             */
            ping6_client(addresses::addr_t addr);
            
            /**
             * Send data using ping (ICMP_ECHO) packet
             * @param data      pointer on data to send
             * @param data_len  size of data to send in Bytes
             * @param sequence  id of ping packet
             * @return bool, true if send, false if not send
             */
            bool send(char *data, unsigned data_len, unsigned sequence);

            /**
             * Get maximal size of data in packet
             * @return unsigned max data size [B]
             */
            unsigned max_data_len();

        private:

            /**
             * Create ICMP6 packet with data and header fill with zeros
             * @param data pointer to data for packet
             * @param data_len lenght of data for packet
             * @return pointer on packet recast as icmp6_hdr*
             */
            icmp6_hdr* create_packet(char *data, unsigned data_len);

            /**
             * Calc size of packet
             * @param data_len lenght of data in packet (without header)
             * @return usigned size of packet with header [B]
             */
            unsigned packet_size(unsigned data_len);

            /**
             * ID of socket
             */
            int sock;

            /**
             * Destination address
             */
            struct sockaddr * addr;

    };
    
}
