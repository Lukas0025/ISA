#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "debug.h"

namespace addresses {
    typedef struct addrinfo addr_t;

    addr_t get(char *hostname);
}