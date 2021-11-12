/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * AES ecryption file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#pragma once

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <random>
#include <climits>
#include <algorithm>
#include <functional>

#include <cstring>

#define MAX_IV_LEN  16 + 1 //for \0
#define MAX_KEY_LEN 32 + 1 //for \0

namespace aes {
    class aes {
        public:
            aes();
            unsigned enc(unsigned char *plaintext, unsigned plaintext_len, unsigned char *ciphertext);
            unsigned dec(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext);
            void set_key(unsigned char key[MAX_KEY_LEN]);
            u_char iv[MAX_IV_LEN];

        private:
            unsigned char key[MAX_KEY_LEN];
    };
}
