/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * generic ping file implementation
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

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

        throw std::runtime_error("bad INET family");

        return NULL; //dump interface object
    }

    bool ping_client::send_string(char *data, unsigned data_len) {
        
        unsigned full_blocks_count = data_len / this->max_data_len();

        for (unsigned i = 0; i < full_blocks_count; i++) {
            this->send(data + i * this->max_data_len(), this->max_data_len(), i);
        }

        //not full block
        if (data_len % this->max_data_len()) {
            this->send(data + full_blocks_count * this->max_data_len(), data_len % this->max_data_len(), full_blocks_count);
        }

        return true;

    }

    bool ping_client::send_file(FILE *fp) {
        if (fp == NULL) {
            return false;
        }

        char buff[MAXPACKET];
        uint16_t id = 0;

        size_t readed;
        do {
            readed = fread(buff, sizeof(char), this->max_data_len(), fp);
                
            if (ferror( fp ) != 0) {
                D_PRINT("Error reading file");
                return false;
            }

            this->send(buff, readed, id);

            id++;
            
        } while(readed != 0);

        return true;
         
    }

}