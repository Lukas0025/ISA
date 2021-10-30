#include "server.h"
#include "debug.h"

namespace server {
    server::server() {
        char errbuf[PCAP_ERRBUF_SIZE];
        struct bpf_program fp;		/* compilled filter */
        
        if (pcap_lookupnet("any", &this->net, &this->mask, errbuf) == -1) {
            this->net = 0;
            this->mask = 0;
        }
        
        this->interface = pcap_open_live("any", BUFSIZ, 1, 1000, errbuf);
        
        if (this->interface == NULL) {
            D_PRINT("%s", errbuf);
        }
	

        if (pcap_datalink(this->interface) == DLT_LINUX_SLL) {
            this->linktype = DLT_LINUX_SLL;
        } else if (pcap_datalink(this->interface) == DLT_LINUX_SLL2) {
            this->linktype = DLT_LINUX_SLL2;
        } else {
            D_PRINT("%s", "unsuported linktype protocol");
        }

        if (pcap_compile(this->interface, &fp, "(icmp and icmp[icmptype] == icmp-echo) or (icmp6 and icmp6[icmp6type] == icmp6-echo)", 0, this->net) == -1) {
		    D_PRINT("Couldn't parse filter");
	    }

        if (pcap_setfilter(this->interface, &fp) == -1) {
		    D_PRINT("%s", pcap_geterr(this->interface));
	    }
    }

    l2_packet server::l2_decode(const u_char *packet) {
        l2_packet decode;

        if (this->linktype = DLT_LINUX_SLL) {
            //load packet as SSL
            auto sll_packet  = (struct sll_header *) packet;
            decode.type      = sll_packet->sll_protocol;
            decode.body      = packet + sizeof(struct sll_header);
        } else { //SLL2
            auto sll2_packet = (struct sll2_header *) packet;
            decode.type      = sll2_packet->sll2_protocol;
            decode.body      = packet + sizeof(struct sll2_header);
            D_PRINT("using sll2");
        }

        return decode;
    }

    l3_packet server::l3_decode(l2_packet packet) {
        l3_packet decode;
        
        //load next protocol in packet
        if (ntohs(packet.type) == IPV4_SLL) { //IPv4
            decode.ipv4_hdr = (struct ip*)(packet.body);
            decode.ipv4     = true;
            decode.body     = packet.body + sizeof(struct ip);
            decode.body_len = 0;
        } else if (ntohs(packet.type) == IPV6_SLL) { //IPv6
            decode.ipv6_hdr = (struct ip6_hdr*)(packet.body);
            decode.ipv6     = true;
            decode.body     = packet.body + sizeof(struct ip6_hdr);
            decode.body_len = 0;
        } else {
            D_PRINT("no IP or IPv6, %i", ntohs(packet.type));
        }
    
        return decode;
    }

    icmp_packet server::icmp_decode(l3_packet packet) {
        icmp_packet decode;

        if (packet.ipv4) {
            decode.body = packet.body + sizeof(struct icmphdr);
            decode.hdr4 = (struct icmphdr *)packet.body;
            decode.icmp = true;
        } else if (packet.ipv6) {
            decode.body = packet.body + sizeof(struct icmp6_hdr);
            decode.hdr6 = (struct icmp6_hdr *)packet.body;
            decode.icmp6 = true;
        } else {
            D_PRINT("no IP or IPv6");
        }

        return decode;
    }

    void server::sniff() {
        const u_char* packet;
        struct pcap_pkthdr header;
        
        packet = pcap_next(this->interface, &header);

        auto l2_pkt   = this->l2_decode(packet);
        auto l3_pkt   = this->l3_decode(l2_pkt);
        auto icmp     = this->icmp_decode(l3_pkt);

        printf("packet %d\n", ntohs(icmp.hdr4->un.echo.sequence));
        printf("%s\n\n", icmp.body);

        //return this->icmp_decode(l3_pkt);   
    }


    void server::listen(FILE *fp) {
        while (true) {
               
        }
    }
}
