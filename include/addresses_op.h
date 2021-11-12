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
    bool packet_src_cmp(server::icmp_packet *packetA, server::icmp_packet *packetB);
}
