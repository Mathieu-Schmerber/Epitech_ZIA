/**
 * \file Cryption.hpp
 * \brief Class to crypt and uncrypt messages
 * \author Cyprien.R
**/

#ifndef ZIA_CRYPTION_HPP
#define ZIA_CRYPTION_HPP

#include <string>
#include <algorithm>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/opensslv.h>

/**
 * \class Cryption Cryption.hpp "Cryption.hpp"
 * \brief Cryption is a class to crypt and uncrypt messages
**/
class Cryption {
public:
    Cryption();
    std::string encrypt(std::string &);
    std::string decrypt(std::string &);

private:
    uint8_t Key[32]{};
    uint8_t IV[AES_BLOCK_SIZE]{}; // Generate an AES Key
    uint8_t IVd[AES_BLOCK_SIZE]{};
    AES_KEY *AesEncryptKey = new AES_KEY();
    AES_KEY *AesDecryptKey = new AES_KEY();

};


struct ctr_state {
    unsigned char ivec[16];
    unsigned int num;
    unsigned char ecount[16];
};


#endif //ZIA_CRYPTION_HPP
