#include "server.h"

namespace server {
    server::server() {
        char errbuf[PCAP_ERRBUF_SIZE];
        
        if (pcap_lookupnet("any", &this->net, &this->mask, errbuf) == -1) {
            this->net = 0;
            this->mask = 0;
        }
        
        this->interface = pcap_open_live("any", BUFSIZ, 1, 1000, errbuf);
        
        if (this->interface == NULL) {
            throw std::runtime_error(errbuf);
        }

        if (pcap_datalink(this->interface) == DLT_LINUX_SLL) {
            this->linktype = DLT_LINUX_SLL;
        } else if (pcap_datalink(this->interface) == DLT_LINUX_SLL2) {
            this->linktype = DLT_LINUX_SLL2;
        } else {
            throw std::runtime_error("unsuported linktype protocol");
        }
    }

    l2_packet server::l2_decode(u_char *packet) {
        l2_packet decode;

        if (this->linktype = DLT_LINUX_SLL) {
            //load packet as SSL
            auto sll_packet  = (sll_header *) packet;
            decode.type      = sll_packet->sll_protocol;
            decode.body      = (char *)(sll_packet + sizeof(sll_header));
        } else { //SLL2
            auto sll2_packet = (sll2_header *) packet;
            decode.type      = sll2_packet->sll2_protocol;
            decode.body      = (char *)(sll2_packet + sizeof(sll2_header));
        }
    }

    l3_packet server::l3_decode(l2_packet packet) {
        l3_packet decode;
        
        //load next protocol in packet
        if (ntohs(packet.type) == LINKTYPE_IPV4) { //IPv4
            decode.ipv4_hdr = (struct ip*)(packet.body);
            decode.ipv4     = true;
            decode.body     = packet.body + sizeof(struct ip);
            decode.body_len = decode.ipv4_hdr->total_length;
        } else if (ntohs(packet.type) == LINKTYPE_IPV6) { //IPv6
            decode.ipv6_hdr = (struct ip6_hdr*)(packet.body);
            decode.ipv6     = true;
            decode.body     = packet.body + sizeof(struct ip6_hdr);
            decode.body_len = decode.ipv6_hdr->payload_len;
        }
    
        return decode;
    }

    icmp_packet server::sniff() {
        u_char* packet;
        (struct pcap_pkthdr) header;
        
        packet = pcap_next(this->interface, &header);

        auto l2_pkt   = this->l2_decode(packet);
        auto l3_pkt   = this->l3_decode(l2_pkt);
        return this->icmp_decode(l3_pkt);   
    }


    void server::listen(FILE *fp) {
        while (true) {
               
        }
    }
}