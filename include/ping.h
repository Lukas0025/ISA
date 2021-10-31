/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * generic ping header file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#pragma once

#include <stdexcept>
#include <errno.h>
#include <cstring>
#include "debug.h"
#include "aes.h"
#include "addresses.h"

// Define the Packet Constants
// ping packet size
#define	MAXPACKET   1500
//#define	MAXPACKET	1500

// Automatic port number
#define PORT_NO 0

// Automatic port number
#define PING_SLEEP_RATE 1000000 x

// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 10

namespace ping {

    typedef struct {
        u_char protocol[10] = "SECv0.0.1"; 
        uint32_t blocks_count;
        uint32_t block_size;
        u_char iv[MAX_IV_LEN];
    } icmp_enc_transf_hdr;

    /**
     * Interface for ping clientsD_PRINT("sending header");
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
            virtual bool send(char *data, unsigned data_len, uint16_t sequence) = 0;

            /**
             * Get maximal size of data in packet
             * @return unsigned max data size [B]
             */
            virtual unsigned max_data_len() = 0;

            /**
             * Send string using ping
             * @param data     pointer to data to send
             * @param data_len length of data to send
             * @return bool if send is ok or not
             */
            bool send_string(char *data, unsigned data_len);

            bool send_file(FILE *fp);

            bool send_file_enc(FILE *fp);
    };

    /**
     * Open ping on address
     * @param addr address for open ping (INET6 = IPv6 and INET = IPv4) 
     * @return ping_client*
     * @try runtime_error if fail
     */
    ping_client* open(addresses::addr_t addr);
}
