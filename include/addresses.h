/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * addresses support operations header file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include "debug.h"

namespace addresses {
    /**
     * data type for IP address any type
     */
    typedef struct addrinfo addr_t;

    /**
     * Get addr_t from string hostname
     * @param hostname pointer on char array with hostname or IP address (IPv4 or IPv6)
     * @return addr_t of hostname, if translate fail return .ai_family = AF_UNSPEC;
     */
    addr_t get(char *hostname);
}