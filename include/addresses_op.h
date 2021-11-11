#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include "debug.h"
#include "server.h"
#include "addresses.h"

namespace addresses {
    bool packet_addr_in(server::icmp_packet packet, addr_t *address_list);
    bool packet_addrs_cmp(server::icmp_packet packetA, server::icmp_packet packetB);
}