#include "ping.h"

namespace ping {

    void send(addresses::addr_t addr, char* data, unsigned len) {
        
        int sock;

        switch (addr.ai_family) {
            case AF_INET:
                D_PRINT("using IPv4");
                sock = v4sock();
                break;
                
            case AF_INET6:
                D_PRINT("using IPv6");
                sock = v6sock();
                break;
                //sockaddr_in6 *sin = reinterpret_cast<sockaddr_in6*>(&addr);
                //inet_ntop(AF_INET6, &sin->sin6_addr, ip, INET6_ADDRSTRLEN);
                //(struct sockaddr_in6 *) res->ai_addr)->sin6_addr
                
                //return pingv6::send(sin, data);
            
            default:
                D_PRINT("bad family");
        }

        send_packet(sock, addr.ai_addr, data, 0);
        send_packet(sock, addr.ai_addr, data, 1);
        send_packet(sock, addr.ai_addr, data, 2);
    }

    bool send_packet(int sock, struct sockaddr* addr, char *data, unsigned sequence) {
        ping::packet_t packet;
        
        //filling packet
        bzero(&packet, sizeof(packet));
        packet.hdr.type             = ICMP_ECHO;
        packet.hdr.un.echo.id       = getpid();
        packet.hdr.un.echo.sequence = sequence;

        if (sendto(sock, &packet, sizeof(packet), 0, addr, sizeof(*addr)) <= 0) {
            D_PRINT("Fail to send packet");
            return false;
        }

        unsigned recv_size;
        if (recvfrom(sock, &packet, sizeof(packet), 0, addr, &recv_size) <= 0) {
			D_PRINT("Fail to recv reply");
            return false;
		}

        //todo: check packet
        return true;

    }

    

    int v4sock() {
        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);

        const int ttl_val=64;
        
        // set socket options at ip to TTL and value to 64,
        // change to what you want by setting ttl_val
        if (setsockopt(sock, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0) {
            D_PRINT("Fail to set socket TTL");
            return -1;
        }

        D_PRINT("socket TTL is set");

        struct timeval tv_out;
        tv_out.tv_sec = RECV_TIMEOUT;
        tv_out.tv_usec = 0;

        // setting timeout of recv setting
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out) != 0) {
            D_PRINT("Fail to set socket TIMEOUT");
            return -1;
        }

        D_PRINT("socket TIMEOUT is set");

        return sock;
    }

/*
	icmph = (struct icmp6_hdr *)_icmph;
	icmph->icmp6_type = ICMP6_ECHO_REQUEST;
	icmph->icmp6_code = 0;
	icmph->icmp6_cksum = 0;
	icmph->icmp6_seq = htons(ntransmitted+1);
	icmph->icmp6_id = ident;
*/
    int v6sock() {
        int sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_ICMPV6);

        const int ttl_val=64;
        
        // set socket options at ip to TTL and value to 64,
        // change to what you want by setting ttl_val
        if (setsockopt(sock, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0) {
            D_PRINT("Fail to set socket TTL");
            return -1;
        }

        D_PRINT("socket TTL is set");

        struct timeval tv_out;
        tv_out.tv_sec = RECV_TIMEOUT;
        tv_out.tv_usec = 0;

        // setting timeout of recv setting
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out) != 0) {
            D_PRINT("Fail to set socket TIMEOUT");
            return -1;
        }

        D_PRINT("socket TIMEOUT is set");

        return sock;
    }
}