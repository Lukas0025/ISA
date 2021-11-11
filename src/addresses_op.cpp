#include "addresses_op.h"

namespace addresses {
    bool packet_addr_in(server::icmp_packet packet, addr_t *address_list) {
        return true;
    }

    bool packet_addrs_cmp(server::icmp_packet packetA, server::icmp_packet packetB) {
        return true;
    }
}