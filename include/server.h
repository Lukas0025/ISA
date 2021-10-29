/**
 * Project: packet sniffer - IPK variant ZETA
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 19.4.2021
 */
#pragma once

#include <pcap/pcap.h>
#include <string>
#include <vector>

/** Headers scructs of protocols */
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

/** Convert functions for packets */
#include <arpa/inet.h>

namespace server {
    
    typedef struct {
        uint16_t type;
        unsigned char *body;
    } l2_packet;

    /**
    * Packet on layer 3 with layer 2 support for ARP and ETHERNET header
    */
    typedef struct {
        //Decoded info
        bool ipv4 = false;
        bool ipv6 = false;

        //IP Headers
        struct ip6_hdr* ipv6_hdr;
        struct ip*      ipv4_hdr;

        //body
        const u_char *body;
        uint body_len;
    } l3_packet;

    typedef struct {
        //body
        const u_char *body;
        uint body_len;
    } icmp_packet;

}