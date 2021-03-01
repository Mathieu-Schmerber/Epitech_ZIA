//
// Created by Cyprien on 2/26/2021.
//

#include <iostream>
#include <vector>
#include "Cryption.hpp"

// TODO, la documentation

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

    std::cout << "[encrypt] DecryptedData: " << UserData << std::endl;
    AES_cbc_encrypt(UserData, EncryptedData, UserDataSizePadded, static_cast<const AES_KEY *>(AesEncryptKey), IV, AES_ENCRYPT);
    std::cout << "[encrypt] EncryptedData: " << EncryptedData << std::endl;

    return std::string();
}

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
    std::cout << "DecryptedData: " << DecryptedData << std::endl;
    std::cout << "EncryptedData: " << EncryptedData << std::endl;

    return std::string();
}















