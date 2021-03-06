/**
 * \file Cyrption.cpp
 * \brief functions of Cyrption's class
 * \authors Cyprien.R
**/

#include <iostream>
#include <vector>
#include "Cryption.hpp"

/**
 * \brief Cryption constructor
**/
Cryption::Cryption()
{
    RAND_bytes(Key, sizeof(Key));
    RAND_bytes(IV, sizeof(IV));

    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        IVd[i] = IV[i];
    }
    AES_set_encrypt_key(Key, 256, AesEncryptKey); // à envoyer
    AES_set_decrypt_key(Key, 256, AesDecryptKey); // à garder
}

/**
 * \brief encrypt string
 *
 * \param txt : string to encrypt
**/
std::string Cryption::encrypt(std::string &txt)
{
    int RequiredPadding = (AES_BLOCK_SIZE - (static_cast<int>(txt.length()) % AES_BLOCK_SIZE));
    std::vector<unsigned char> PaddedTxt(txt.begin(), txt.end());
    for (int i = 0; i < RequiredPadding; i++)
        PaddedTxt.push_back(0);
    auto PaddedTxt_cpy = std::vector(PaddedTxt);

    unsigned char *UserData = &PaddedTxt[0];
    unsigned char *EncryptedData = &PaddedTxt_cpy[0];
    size_t UserDataSizePadded = PaddedTxt.size();

    LOG(DEBUG) << "[encrypt] DecryptedData: " << UserData;
    AES_cbc_encrypt(UserData, EncryptedData, UserDataSizePadded, static_cast<const AES_KEY *>(AesEncryptKey), IV, AES_ENCRYPT);
    LOG(DEBUG) << "[encrypt] EncryptedData: " << EncryptedData;

    return std::string();
}

/**
 * \brief decrypt string
 *
 * \param txt : string to decrypt
**/
std::string Cryption::decrypt(std::string &txt)
{
    std::vector<unsigned char> PaddedTxt(txt.begin(), txt.end());   // Easier to Pad as a vector
    int RequiredPadding = (AES_BLOCK_SIZE - (static_cast<int>(txt.length()) % AES_BLOCK_SIZE));   // Calculate required padding

    for (int i = 0; i < RequiredPadding; i++)
        PaddedTxt.push_back(0);
    auto PaddedTxt_cpy = std::vector(PaddedTxt);

    unsigned char *EncryptedData = &PaddedTxt[0];
    unsigned char *DecryptedData = &PaddedTxt_cpy[0];
    size_t UserDataSizePadded = PaddedTxt.size();

    /** Decrypt the data. Note that we use the same function call. Only change is the last parameter **/
    AES_cbc_encrypt(EncryptedData, DecryptedData, UserDataSizePadded, (const AES_KEY *) AesDecryptKey, IVd, AES_DECRYPT);
    LOG(DEBUG) << "DecryptedData: " << DecryptedData;
    LOG(DEBUG) << "EncryptedData: " << EncryptedData;

    return std::string();
}