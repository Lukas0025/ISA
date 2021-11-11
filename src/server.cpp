#include "server.h"
#include "debug.h"
#include "aes.h"
#include "addresses_op.h"

namespace server {
    server::server(addresses::addr_t* address_list) {
        char errbuf[PCAP_ERRBUF_SIZE];
        struct bpf_program fp;		/* compilled filter */

        this->address_list = address_list;
        
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

    l2_packet server::l2_decode(const u_char *packet, struct pcap_pkthdr *header) {
        l2_packet decode;

        decode.ptr = packet;

        if (this->linktype = DLT_LINUX_SLL) {
            //load packet as SSL
            auto sll_packet  = (struct sll_header *) packet;
            decode.type      = sll_packet->sll_protocol;
            decode.body      = packet + sizeof(struct sll_header);
            decode.body_len  = header->len - sizeof(struct sll_header);
        } else { //SLL2
            auto sll2_packet = (struct sll2_header *) packet;
            decode.type      = sll2_packet->sll2_protocol;
            decode.body      = packet + sizeof(struct sll2_header);
            decode.body_len  = header->len - sizeof(struct sll2_header);
        }

        return decode;
    }

    l3_packet server::l3_decode(l2_packet packet) {
        l3_packet decode;

        decode.ptr = packet.ptr;
        
        //load next protocol in packet
        if (ntohs(packet.type) == IPV4_SLL) { //IPv4
            decode.ipv4_hdr = (struct ip*)(packet.body);
            decode.ipv4     = true;
            decode.body     = packet.body + sizeof(struct ip);
            decode.body_len = packet.body_len - sizeof(struct ip);
        } else if (ntohs(packet.type) == IPV6_SLL) { //IPv6
            decode.ipv6_hdr = (struct ip6_hdr*)(packet.body);
            decode.ipv6     = true;
            decode.body     = packet.body + sizeof(struct ip6_hdr);
            decode.body_len = packet.body_len - sizeof(struct ip6_hdr);
        } else {
            D_PRINT("no IP or IPv6, %i", ntohs(packet.type));
        }
    
        return decode;
    }

    icmp_packet server::icmp_decode(l3_packet packet) {
        icmp_packet decode;

        decode.ptr = packet.ptr;

        decode.ipv6_hdr = packet.ipv6_hdr;
        decode.ipv4_hdr = packet.ipv4_hdr;

        if (packet.ipv4) {
            decode.body     = packet.body + sizeof(struct icmphdr);
            decode.body_len = packet.body_len - sizeof(struct icmphdr);
            decode.hdr4     = (struct icmphdr *)packet.body;
            decode.seq      = ntohs(decode.hdr4->un.echo.sequence);
            decode.id       = ntohs(decode.hdr4->un.echo.id);
            decode.icmp     = true;
        } else if (packet.ipv6) {
            decode.body     = packet.body + sizeof(struct icmp6_hdr);
            decode.body_len = packet.body_len - sizeof(struct icmp6_hdr);
            decode.hdr6     = (struct icmp6_hdr *)packet.body;
            decode.seq      = ntohs(decode.hdr6->icmp6_seq);
            decode.id       = ntohs(decode.hdr6->icmp6_id);
            decode.icmp6    = true;
        } else {
            D_PRINT("no IP or IPv6");
        }

        return decode;
    }

    icmp_packet server::sniff() {
        const u_char* packet;
        struct pcap_pkthdr header;
        
        do {
            packet = pcap_next(this->interface, &header);

            if (packet == NULL) {
                D_PRINT("error packet not exist");
            }
        } while (packet == NULL);

        auto l2_pkt   = this->l2_decode(packet, &header);
        auto l3_pkt   = this->l3_decode(l2_pkt);

        return this->icmp_decode(l3_pkt);   
    }

    void server::do_transer(FILE *fp, uint16_t id, ping::icmp_enc_transf_hdr * header, icmp_packet *sync_packet) {
        auto crypt = new aes::aes();
        memcpy(crypt->iv, header->iv, MAX_IV_LEN);

        u_char buff[header->block_size + 1];
        buff[header->block_size] = '\0';

        uint32_t to_read = header->blocks_count;
        while (to_read > 0) {
            auto packet = this->sniff();
            if ((packet.id == id) && addresses::packet_src_cmp(sync_packet, &packet)) { //next packet from host
                auto dec_len = crypt->dec((u_char *)packet.body, packet.body_len, buff);
                fwrite(buff, sizeof(u_char), dec_len, fp);
                to_read--;
                //fwrite(buffer , sizeof(char), sizeof(buffer), pFile);
            } else {
                D_PRINT("droping packet not from sync host");
            }
        }

        D_PRINT("transfer done");
    }

    void server::listen(FILE *fp) {
        while (true) {

            auto packet = this->sniff();

            if (!addresses::packet_addr_in(packet, this->address_list)) {
                continue;
            }

            if (packet.seq == 0 && packet.body_len > 10) {
                auto header = (ping::icmp_enc_transf_hdr *) packet.body;

                if (strcmp((char *)header->protocol, "SECv0.0.1") == 0) { //protocol version 0.0.1
                    //read header
                    D_PRINT("init transfer with protocol %s blocks %d blocksize %d pear id is %d", header->protocol, header->blocks_count, header->block_size, packet.id);
                    return this->do_transer(fp, packet.id, header, &packet);
                }
            }               
        }
    }
}
