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
#include <pcap/sll.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/in.h>
#include <netinet/icmp6.h>
#include <netinet/ip_icmp.h>

/** Convert functions for packets */
#include <arpa/inet.h>

#define IPV4_SLL 0x0800
#define IPV6_SLL 0x86dd

namespace server {
    
    typedef struct {
        uint16_t type;
        uint     len;
        const u_char   *body;
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

        bool icmp6 = false;
        bool icmp  = false;

        struct icmp6_hdr *hdr6;
        struct icmphdr   *hdr4;

        uint body_len;
    } icmp_packet;

    class server {
        public:
            server();
            l2_packet l2_decode(const u_char *packet);
            l3_packet l3_decode(l2_packet packet);
            void sniff();
            void listen(FILE *fp);
            icmp_packet icmp_decode(l3_packet packet);
        private:
            pcap_t *interface;      /* Interface from we sniffing */
            bpf_u_int32 mask;		/* The network mask of sniffing device */
            bpf_u_int32 net;        /* Net of interface */
            int linktype;
    };

}
