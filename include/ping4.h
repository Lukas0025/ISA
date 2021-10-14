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

    typedef struct icmphdr icmphdr;

    class ping4_client: public ping_client {
        public:
            ping4_client(addresses::addr_t addr);
            bool send(char *data, unsigned data_len, unsigned sequence);

        private:
            icmphdr* create_packet(char *data, unsigned data_len);
            unsigned packet_size(unsigned data_len);

            int sock;
            struct sockaddr * addr;

    };

}