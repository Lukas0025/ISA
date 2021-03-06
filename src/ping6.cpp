/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * ping on IPv6 implementation file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#include "ping6.h"

namespace ping {

    ping6_client::ping6_client(addresses::addr_t addr) {
        this->addr = addr.ai_addr;
        this->sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_ICMPV6);

        if (this->sock == -1) {
            D_PRINT("Fail to create socket");
            throw std::runtime_error(strerror(errno));
        }

        D_PRINT("Created socket %i", this->sock);

        const int ttl_val=64;
        
        // set socket options at ip to TTL and value to 64,
        // change to what you want by setting ttl_val
        if (setsockopt(this->sock, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0) {
            D_PRINT("Fail to set socket TTL");
            throw std::runtime_error(strerror(errno));
        }

        D_PRINT("socket TTL is set");

        struct timeval tv_out;
        tv_out.tv_sec = RECV_TIMEOUT;
        tv_out.tv_usec = 0;

        // setting timeout of recv setting
        if (setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out) != 0) {
            D_PRINT("Fail to set socket TIMEOUT");
            throw std::runtime_error(strerror(errno));
        }

        D_PRINT("socket TIMEOUT is set");
    }

    icmp6_hdr* ping6_client::create_packet(char *data, unsigned data_len) {
        auto packet = (char*) malloc(sizeof(icmp6_hdr) + data_len);
        
        //fill with zeros
        bzero(packet, this->packet_size(data_len));

        //copy data
        memcpy(packet + sizeof(icmp6_hdr), data, data_len);
        
        return (icmp6_hdr*) packet;
    }

    unsigned ping6_client::packet_size(unsigned data_len) {
        return sizeof(icmp6_hdr) + data_len;
    }

    bool ping6_client::send(char *data, unsigned data_len, uint16_t sequence) {
        auto packet = this->create_packet(data, data_len);
        
        //set packet headers
        packet->icmp6_type = ICMP6_ECHO_REQUEST;
        packet->icmp6_code = 0;
        packet->icmp6_seq  = sequence;

        //send packet
        if (sendto(this->sock, packet, this->packet_size(data_len), 0, this->addr, sizeof(struct sockaddr_in6)) <= 0) {
            D_PRINT("Fail to send packet %i, %s", errno, strerror(errno));
            return false;
        }

        //recv reply
        unsigned recv_size = 0;
        if (recvfrom(this->sock, packet, this->packet_size(data_len), 0, this->addr, &recv_size) <= 0) {
			D_PRINT("Fail to recv reply %i, %s", errno, strerror(errno));
            return false;
		}

        free(packet);

        return true;
    }

    unsigned ping6_client::max_data_len() {
        return MAXPACKET - sizeof(icmp6_hdr);
    }
}