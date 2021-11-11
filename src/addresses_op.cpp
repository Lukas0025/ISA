#include "addresses_op.h"

namespace addresses {
    bool packet_addr_in(server::icmp_packet packet, addr_t *address_list) {
        return true;
    }

    bool packet_src_cmp(server::icmp_packet *packetA, server::icmp_packet *packetB) {

        if (packetA->icmp6 != packetB->icmp6) {
            D_PRINT("not same addr type");
            return false;
        }

        if (packetA->icmp6) {
            return memcmp(&(packetA->ipv6_hdr->ip6_src), &(packetB->ipv6_hdr->ip6_src), sizeof(struct in6_addr)) == 0;
        } else {
            return memcmp(&(packetA->ipv4_hdr->ip_src), &(packetB->ipv4_hdr->ip_src), sizeof(struct in_addr)) == 0;
        }

    }
}
