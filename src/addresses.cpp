/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * addresses support functions implementation file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#include "addresses.h"

namespace addresses {
    addr_t get(char *hostname) {
        addr_t hints;
        addr_t output;

        memset(&hints, 0, sizeof(addr_t));
        hints.ai_family  = AF_UNSPEC;
        output.ai_family = AF_UNSPEC;

        // get addr info
        addr_t *res;
        int     ret = getaddrinfo(hostname, NULL, &hints, &res);

        if (ret != 0) {
            // fail to get HOST
            D_PRINT("Fail resolve addresses\n");
            return output;
        }

        output = *res;
        
        #ifdef DEBUG

            char host[256];
            for (addr_t *tmp = res; tmp != NULL; tmp = tmp->ai_next) {
                getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
                D_PRINT("%s", host);
            }

        #endif

        //todo free
        //freeaddrinfo(res);


        return output;
    }
}