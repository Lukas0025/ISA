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

    typedef struct icmp6_hdr icmp6_hdr;

    class ping6_client: public ping_client {
        public:
            ping6_client(addresses::addr_t addr);
            bool send(char *data, unsigned data_len, unsigned sequence);

        private:
            icmp6_hdr* create_packet(char *data, unsigned data_len);
            unsigned packet_size(unsigned data_len);

            int sock;
            struct sockaddr * addr;

    };
    
}
