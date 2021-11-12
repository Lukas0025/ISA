/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * file with server implementation header
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
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
#define PCAP_RECBUF_SIZE 100*1024*1024

namespace server {

    /**
    * Packet on layer 2
    */    
    typedef struct {
        const u_char   *ptr;

        uint16_t        type;
        uint            body_len;
        const u_char   *body;
    } l2_packet;

    /**
    * Packet on layer 3
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

    /**
    * ICMP packet
    */
    typedef struct {
        const u_char   *ptr;

        //IP Headers
        struct ip6_hdr* ipv6_hdr;
        struct ip*      ipv4_hdr;

        //body
        const u_char *body;

        //icmp type
        bool icmp6 = false;
        bool icmp  = false;

        uint16_t seq;
        uint16_t id;

        //icmp headers
        struct icmp6_hdr *hdr6;
        struct icmphdr   *hdr4;

        uint body_len;
    } icmp_packet;

    class server {
        public:
            server();

            /**
             * Decode packet on L1 to L2
             * @param packet pointer to packet from pcap
             * @param header header of pcap packet
             * @return decoded l2_packet
             */
            l2_packet l2_decode(const u_char *packet, struct pcap_pkthdr *header);

            /**
             * Decode packet on L2 to L3
             * @param packet l2 packet
             * @return decoded l3_packet
             */
            l3_packet l3_decode(l2_packet packet);

            /**
             * Sniff ICMP packet from interface any
             * @return decoded icmp_packet
             */
            icmp_packet sniff();

            /**
             * Listen for SEC protocol transfer if hear init packet start transmission
             * @param fp pointer for file to save data
             */
            void listen(FILE *fp);

            /**
             * Decode packet on L3 to imcp packet
             * @param packet l3 packet
             * @return decoded icmp packet
             */
            icmp_packet icmp_decode(l3_packet packet);

            /**
             * Handle SEC transfer and save data to file
             * @param fp pointer for file to sove data
             * @param id id of ICMP init packet
             * @param header header of ICMP init packet
             * @param sync_packet ICMP init packet
             */
            void do_transer(FILE *fp, uint16_t id, ping::icmp_enc_transf_hdr * header, icmp_packet *sync_packet);
        private:
            pcap_t *interface;      /* Interface from we sniffing */
            bpf_u_int32 mask;		/* The network mask of sniffing device */
            bpf_u_int32 net;        /* Net of interface */
            int linktype;
    };

}
