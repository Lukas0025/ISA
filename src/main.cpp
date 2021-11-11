/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * main project file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "addresses.h"
#include "aes.h"
#include "ping.h"
#include "server.h"

void print_help(char *prog) {
    printf("Usage: %s  -r <file> -s <ip|hostname> [-l] [-h]\n\n", prog);
    printf("-r <file>           file to send\n");
    printf("-s <ip|hostname>    ip address/hostanme of listening client for file\n");
    printf("-l                  listen for files and save it to $(PWD)\n");
}

int main(int argc, char *argv[]) {
    char *send_file = NULL;
    char *send_addr = NULL;
    bool listen     = false;

    //get opts for parameters parse
    int opt;
    while ((opt = getopt(argc, argv, "r:s:ihl")) != -1) {
        switch (opt) {
            case 'r':
                send_file = optarg;
                break;
            case 's':
                send_addr = optarg;
                break;
            case 'l':
                listen = true;
                break;
            case 'h':
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Usage: %s  -r <file> -s <ip|hostname> [-l] [-h]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    //Check of existence of right parameters for modes
    if (!listen && (send_addr == NULL || send_file == NULL)) {
        fprintf(stderr, "send mode and not -r or -s\n");
        fprintf(stderr, "Usage: %s  -r <file> -s <ip|hostname> [-l] [-h]\n", argv[0]);
        exit(EXIT_FAILURE);
    } else if (listen && (send_addr != NULL || send_file != NULL)) {
        //TBD
    }

    auto address_list = addresses::get(send_addr);

    if (address_list == NULL) {
        fprintf(stderr, "cant resolve IP address\n");
        exit(EXIT_FAILURE);
    }

    if (!listen) {
        
        bool sended = false;
        for (addresses::addr_t *addr = address_list; addr != NULL; addr = addr->ai_next) {
            
            #ifdef DEBUG
                char host_debug[256];
                getnameinfo(addr->ai_addr, addr->ai_addrlen, host_debug, sizeof(host_debug), NULL, 0, NI_NUMERICHOST);
                D_PRINT("used address: %s", host_debug);
            #endif
            
            ping::ping_client* ping_cl;
            
            try {
                ping_cl = ping::open(*addr);
            } catch (std::runtime_error& e) {
                fprintf(stderr, "[error] when try open socket: %s\n", e.what());
                continue;
            }
            
            auto fp = fopen(send_file, "r");
            
            if (ping_cl->send_file_enc(fp)) {
                fprintf(stderr, "Sending done\n");
                sended = true;
                break;
            }
        }

        if (!sended) {
            fprintf(stderr, "Fail to send\n");
        }

    } else {
        auto fp = fopen (send_file, "wb");
        auto srv = new server::server(address_list);
        srv->listen(fp);
    }

    freeaddrinfo(address_list);

    exit(EXIT_SUCCESS);
}
