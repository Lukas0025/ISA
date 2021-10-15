/**
 * Project: packet sniffer - IPK variant ZETA
 * sniffer TOP Header file
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

#define MAC_LENGTH 6
#define IPV4_LENGTH 4
#define ARP_PROT -1
#define UNDEF_PROT -2
#define MAC_ADDR_STRLEN 18

/**
 * ARP protocol header
 * header by https://en.wikipedia.org/wiki/Address_Resolution_Protocol (April 2021)
 */
struct arp_header {
    u_short hw_type;
    u_short prot_type;
    u_char  hw_len;
    u_char  prot_len;
    u_short opcode;
    u_char  src_mac[MAC_LENGTH];
    u_char  src_ip[IPV4_LENGTH];
    u_char  dst_mac[MAC_LENGTH];
    u_char  dst_ip[IPV4_LENGTH];
};

/**
 * Packet on layer 1
 */
typedef struct {
    //raw pcap data
    struct pcap_pkthdr header;
    const u_char *body;
} l1_packet;

/**
 * Packet on layer 3 with layer 2 support for ARP and ETHERNET header
 */
typedef struct {
    //Decoded info
    bool ipv4 = false;
    bool ipv6 = false;
    bool arp  = false;

    //IP Headers
    struct ip6_hdr* ipv6_hdr;
    struct ip*      ipv4_hdr;

    //ARP
    struct arp_header* arp_hdr;

    //ether header
    struct ether_header* ether_hdr;

    //body
    const u_char *body;
    uint body_len;
} l3_packet;

/**
 * Packet on layer 4 for protocols TCP, UDP and ICMP/ICMPv6
 */
typedef struct {
    bool tcp  = false;
    bool udp  = false;
    bool icmp = false;

    //headers
    struct tcphdr* tcp_hdr;
    struct udphdr* udp_hdr;

    //body
    const u_char *body;
    uint body_len;
} l4_packet;

/**
 * Sniffer class. Object for sniff packets from device
 */
class sniffer {
    public:
        /**
         * Returns list of available interfaces names in string
         * @return list of names of interfaces
         */
        static std::vector<std::string> devices();

        /**
         * Print hexdump of binary
         * @param bytes - bytes from print
         * @param len - len of print
         * @return void
         */
        static void hex_dump (const u_char* bytes, const int len);

        /**
         * L3 decoder (ETHERNET from L2 and IP from L3
         * @param packet - L1 packet to decode
         * @return L3 packet
         */
        l3_packet l3_decode(l1_packet packet);

        /**
         * L4 decoder decode L3 to L4 packet
         * @param packet - L3 packet to decode
         * @return L4 packet
         */
        l4_packet l4_decode(l3_packet packet);

        /**
         * get IP protocol from IP packet
         * @param packet - L3 packet
         * @return int IP type
         */
        int get_protocol(l3_packet packet);

        /**
         * Get port of destination
         * @param packet - L4 packet
         * @pre check if is UDP or TCP packet
         * @return port number if no port return 0
         */
        uint16_t get_dst_port(l4_packet packet);

        /**
         * Get port of source
         * @param packet - L4 packet
         * @pre check if is UDP or TCP packet
         * @return port number if no port return 0
         */
        uint16_t get_src_port(l4_packet packet);

        /**
         * Return src IP of l3 packet
         * @param packet - l3 packet
         * @return string of src IP address
         */
        std::string get_src(l3_packet packet);

        /**
         * Return dsc IP of l3 packet
         * @param packet - l3 packet
         * @return string of dsc IP address
         */
        std::string get_dst(l3_packet packet);
        
        /**
         * Object conscructor create sniffer object on interface
         * @param interface - name of interface
         * @return sniffer object
         */
        sniffer(std::string interface);

        /**
         * Object descructor - free all resources
         */
        ~sniffer();

        /**
         * Set filter to sniffer
         */
        void set_filter(std::string filter);
        
        /**
         * Sniff one packet from interface
         * @return sniffed_packet
         */
        l1_packet sniff();

    private:
        pcap_t *interface;      /* Interface from we sniffing */
        bpf_u_int32 mask;		/* The network mask of sniffing device */
        bpf_u_int32 net;        /* Net of interface */
};