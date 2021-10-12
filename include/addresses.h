#include <arpa/inet.h>

#define NOTSET_IP_TYPE 0
#define IPV4_TYPE 4
#define IPV6_TYPE 6

namespace addresses {
    typedef struct {
        uint8_t type;
        struct in_addr v4;
        struct in6_addr v6;
    } addr_t;

    bool valid_ipv4(char *ipAddress);
}