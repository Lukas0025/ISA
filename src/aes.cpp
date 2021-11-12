/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * Aes encryption implemenation file
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

 /** Vetsina funkci je inspirovana
  * EVP Symmetric Encryption and Decryption. Openssl [online]. openssl.org [cit. 2021-11-12]. Dostupné z: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
  */

#include "aes.h"

//random engine
using random_bytes_engine = std::independent_bits_engine<std::default_random_engine, CHAR_BIT, unsigned char>;

namespace aes {
    aes::aes() {
        
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings();     
        
        /* A 256 bit key */
        this->set_key((unsigned char *) "01234567890123456789012345678901");
        
        /* generate IV */
        if (!RAND_bytes(this->iv, MAX_IV_LEN)) {
            //fail to use openSSL rand use normal rand
            random_bytes_engine rbe;
            std::generate(this->iv, this->iv + MAX_IV_LEN, std::ref(rbe));
        }

    }

    void aes::set_key(unsigned char key[MAX_KEY_LEN]) {
        memcpy(this->key, key, MAX_KEY_LEN);
    }

    unsigned aes::enc(unsigned char *plaintext, unsigned plaintext_len, unsigned char *ciphertext) {
        EVP_CIPHER_CTX *ctx;
        
        int len, ciphertext_len;
        
        /* Create and initialise the context */
        if(!(ctx = EVP_CIPHER_CTX_new())) {
            return 0;
        }
            
        /*
         * Initialise the encryption operation. IMPORTANT - ensure you use a key
         * and IV size appropriate for your cipher
         * In this example we are using 256 bit AES (i.e. a 256 bit key). The
         * IV size for *most* modes is the same as the block size. For AES this
         * is 128 bits
         */
        
        if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, this->key, this->iv)) {
            return 0;
        }
            
        /*
         * Provide the message to be encrypted, and obtain the encrypted output.
         * EVP_EncryptUpdate can be called multiple times if necessary
         */
        
        if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
            return 0;
        }
        
        ciphertext_len = len;
            
        /*
         * Finalise the encryption. Further ciphertext bytes may be written at
         * this stage.
         */
        
        if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
            return 0;
        }
            
        ciphertext_len += len;
        
        /* Clean up */
        EVP_CIPHER_CTX_free(ctx);
        
        return ciphertext_len;    
    }

    unsigned aes::dec(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext) {
        EVP_CIPHER_CTX *ctx;

        int len, plaintext_len;

        /* Create and initialise the context */
        if(!(ctx = EVP_CIPHER_CTX_new())) {
            return 0;
        }

        /*
        * Initialise the decryption operation. IMPORTANT - ensure you use a key
        * and IV size appropriate for your cipher
        * In this example we are using 256 bit AES (i.e. a 256 bit key). The
        * IV size for *most* modes is the same as the block size. For AES this
        * is 128 bits
        */
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, this->key, this->iv)) {
            return 0;
        }

        /*
        * Provide the message to be decrypted, and obtain the plaintext output.
        * EVP_DecryptUpdate can be called multiple times if necessary.
        */
        if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
            return 0;
        }

        plaintext_len = len;

        /*
        * Finalise the decryption. Further plaintext bytes may be written at
        * this stage.
        */
        if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
            return 0;
        }
        
        plaintext_len += len;

        /* Clean up */
        EVP_CIPHER_CTX_free(ctx);

        return plaintext_len;
    }
}
