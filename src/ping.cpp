#include "ping.h"
#include "ping4.h"
#include "ping6.h"

namespace ping {

    ping_client* open(addresses::addr_t addr) {

        switch (addr.ai_family) {
            case AF_INET:
                D_PRINT("using IPv4");
                return new ping4_client(addr);
                
            case AF_INET6:
                D_PRINT("using IPv6");
                return new ping6_client(addr);
                break;
            
            default:
                D_PRINT("bad family");
        }

        return NULL; //dump interface object
    }

}