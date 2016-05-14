//
// Blowfish C++ implementation
//
// CC0 - PUBLIC DOMAIN
// This work is free of known copyright restrictions.
// http://creativecommons.org/publicdomain/zero/1.0/
//

#pragma once

#ifndef __blowfish__
#define __blowfish__

#include <stdint.h>
#include <cstddef>
#include <vector>

class Blowfish {
public:
  Blowfish(const std::vector<unsigned char> &key);
  void Encrypt(std::vector<unsigned char> &Data);
  void Decrypt(std::vector<unsigned char> &Data);

private:
  void SetKey(const unsigned char *key, size_t byte_length);
  void EncryptBlock(uint32_t *left, uint32_t *right) const;
  void DecryptBlock(uint32_t *left, uint32_t *right) const;
  uint32_t Feistel(uint32_t value) const;

private:
  uint32_t pary_[18];
  uint32_t sbox_[4][256];
};

#endif /* defined(__blowfish__) */
