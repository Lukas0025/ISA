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
            /**
             * Conscructor of AES obejct
             * it generate randome IV and set default key
             */
            aes();

            /**
             * Encrypt data in buffer with key and iv
             * @param plaintext pointer on unsigned char buffer with source data
             * @param plaintext_len lenght of plaintext buffer
             * @param ciphertext pointer on unsigned char buffer for save encrypted data
             * @return size of encrypted data
             */
            unsigned enc(unsigned char *plaintext, unsigned plaintext_len, unsigned char *ciphertext);

            /**
             * Decrypt data in buffer with key and iv
             * @param plaintext pointer on unsigned char buffer for save decrypted data
             * @param ciphertext_len lenght of encrypted data
             * @param ciphertext pointer on unsigned char buffer with encrypted data
             * @return size of decrypted data
             */
            unsigned dec(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext);

            /**
             * Set key for encryptiob
             * @param key new enction key to set
             * @return void
             */
            void set_key(unsigned char key[MAX_KEY_LEN]);

            /**
             * Initialization vector for encryption
             * https://en.wikipedia.org/wiki/Initialization_vector
             */
            u_char iv[MAX_IV_LEN];

        private:
            unsigned char key[MAX_KEY_LEN];
    };
}
