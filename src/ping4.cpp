#include "ping4.h"
#include "errors.h"

namespace ping {
    ping4::ping4(addresses::addr_t addr) {
        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);

        const int ttl_val=64;
        
        // set socket options at ip to TTL and value to 64,
        // change to what you want by setting ttl_val
        if (setsockopt(sock, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0) {
            D_PRINT("Fail to set socket TTL");
            errors::die();
        }

        D_PRINT("socket TTL is set");

        struct timeval tv_out;
        tv_out.tv_sec = RECV_TIMEOUT;
        tv_out.tv_usec = 0;

        // setting timeout of recv setting
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out) != 0) {
            D_PRINT("Fail to set socket TIMEOUT");
            errors::die();
        }

        D_PRINT("socket TIMEOUT is set");

        return sock;
    }
}