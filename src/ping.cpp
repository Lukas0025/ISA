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

    bool ping_client::send_file_enc(FILE *fp) {
        if (fp == NULL) {
            return false;
        }

        //calc packet body size
        unsigned body_size = this->max_data_len() - (this->max_data_len() % 16) - 1;

        fseek(fp, 0, SEEK_END);
        unsigned file_size = ftell(fp); //in bytes

        D_PRINT("file size is %d", file_size);

        fseek(fp, 0, SEEK_SET);

        unsigned blocks = file_size / body_size + (file_size % body_size > 0);
        D_PRINT("block count is %d with size %d", blocks, body_size);
        D_PRINT("max packet size is %d", this->max_data_len());

        //init crypt
        auto crypt = new aes::aes();

        //send header packet
        ping::icmp_enc_transf_hdr header;

        //fill header with zeros
        //bzero(&header, sizeof(ping::icmp_enc_transf_hdr));

        memcpy(header.iv, crypt->iv, MAX_IV_LEN);
        header.block_size   = body_size;
        header.blocks_count = blocks;

        D_PRINT("sending header");
        int trys; 
        for (trys = 0; trys < PING_RETRY; trys++) {
            if (this->send((char*) &header, sizeof(ping::icmp_enc_transf_hdr), 0)) {
                break;
            }
        }

        if (trys == PING_RETRY) {
            return false;
        }

        //send body

        D_PRINT("working on body");

        unsigned char buff[MAXPACKET];
        unsigned char enc_buff[MAXPACKET];
        uint16_t id = 1;

        size_t readed;
        do {
            readed = fread(buff, sizeof(char), body_size, fp); //read lebovolný násobek 16

            if (readed > 0) {

                unsigned enc_len = crypt->enc(buff, readed, enc_buff);

                D_PRINT("encrypted %dB to %dB", readed, enc_len);

                if (ferror( fp ) != 0) {
                    D_PRINT("Error reading file");
                    return false;
                }

                for (trys = 0; trys < PING_RETRY; trys++) {
                    if (this->send((char *)enc_buff, enc_len, id)) {
                        break;
                    }
                }
                
                if (trys == PING_RETRY) {
                    return false;
                }

                id++;
            }
            
        } while(readed != 0);

        return true;
         
    }

}
