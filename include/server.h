/**
 * Project: packet sniffer - IPK variant ZETA
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 11.11.2021
 */
#pragma once

#include <pcap/pcap.h>
#include <string>
#include <vector>

#include "ping.h"

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
        const u_char   *ptr;

        uint16_t        type;
        uint            body_len;
        const u_char   *body;
    } l2_packet;

    /**
    * Packet on layer 3 with layer 2 support for ARP and ETHERNET header
    */
    typedef struct {
        const u_char   *ptr;

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
        const u_char   *ptr;

        //IP Headers
        struct ip6_hdr* ipv6_hdr;
        struct ip*      ipv4_hdr;

        //body
        const u_char *body;

        bool icmp6 = false;
        bool icmp  = false;

        uint16_t seq;
        uint16_t id;

        struct icmp6_hdr *hdr6;
        struct icmphdr   *hdr4;

        uint body_len;
    } icmp_packet;

    class server {
        public:
            server(addresses::addr_t*);
            l2_packet l2_decode(const u_char *packet, struct pcap_pkthdr *header);
            l3_packet l3_decode(l2_packet packet);
            icmp_packet sniff();
            void listen(FILE *fp);
            icmp_packet icmp_decode(l3_packet packet);
            void do_transer(FILE *fp, uint16_t id, ping::icmp_enc_transf_hdr * header, icmp_packet *sync_packet);
        private:
            pcap_t *interface;      /* Interface from we sniffing */
            bpf_u_int32 mask;		/* The network mask of sniffing device */
            bpf_u_int32 net;        /* Net of interface */
            int linktype;
            addresses::addr_t *address_list;
    };

}
