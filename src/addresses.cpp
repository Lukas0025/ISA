namespace addresses {

    bool valid_ipv4(char *ipAddress) {
        struct sockaddr_in sa;
        int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
        return result != 0;
    }

    bool valid_ipv6(char *ipAddress) {
        
    }

    addr hostname_2_ip(char *hostname) {
        addr output;
        output.type = NOTSET_IP_TYPE;

        struct addrinfo hints, *res, *result;
        int errcode;
        char addrstr[100];
        void *ptr;
        
        memset (&hints, 0, sizeof (hints));
        hints.ai_family   = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags   |= AI_CANONNAME;
        
        errcode = getaddrinfo (host, NULL, &hints, &result);
        if (errcode != 0) {
            return output;
        }
        
        res = result;
        
        while (res) {
            inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);
            
            switch (res->ai_family) {
                case AF_INET:
                    ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
                    break;
                case AF_INET6:
                    ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
                    break;
            }
        
            inet_ntop (res->ai_family, ptr, addrstr, 100);
            printf ("IPv%d address: %s (%s)\n", res->ai_family == PF_INET6 ? 6 : 4,
                addrstr, res->ai_canonname);
                res = res->ai_next;
        }
        
        freeaddrinfo(result);
        return output;
    }

}