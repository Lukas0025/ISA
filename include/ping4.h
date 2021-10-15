/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * ping on IPv4 header file
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
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>

namespace ping {

    /**
     * ICMP header type
     */
    typedef struct icmphdr icmphdr;

    /**
     * Ping client on IPv4 class
     */
    class ping4_client: public ping_client {
        public:
            
            /**
             * Conscructor
             * @param addr addresses::addr_t only IPv4 type (INET)
             * @try runtime_error if fail
             */
            ping4_client(addresses::addr_t addr);

            /**
             * Send data using ping (ICMP_ECHO) packet
             * @param data      pointer on data to send
             * @param data_len  size of data to send in Bytes
             * @param sequence  id of ping packet
             * @return bool, true if send, false if not send
             */
            bool send(char *data, unsigned data_len, uint16_t sequence);

            /**
             * Get maximal size of data in packet
             * @return unsigned max data size [B]
             */
            unsigned max_data_len();

        private:
            
            /**
             * Create ICMP packet with data and header fill with zeros
             * @param data pointer to data for packet
             * @param data_len lenght of data for packet
             * @return pointer on packet recast as icmphdr*
             */
            icmphdr* create_packet(char *data, unsigned data_len);

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