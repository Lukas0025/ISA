/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * addresses support operations with packets header file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include "debug.h"
#include "server.h"
#include "addresses.h"

namespace addresses {

    /**
     * Comparate src address in two packets
     * @param packetA pointer on server::icmp_packet
     * @param packetB pointer on server::icmp_packet
     * @return bool
     */
    bool packet_src_cmp(server::icmp_packet *packetA, server::icmp_packet *packetB);
}
