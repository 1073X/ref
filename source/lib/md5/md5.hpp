/* MD5
 converted to C++ class by Frank Thilo (thilo@unix-ag.org)
 for bzflag (http://www.bzflag.org)

   based on:

   md5.h and md5.c
   reference implementation of RFC 1321

   Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

#ifndef BZF_MD5_H
#define BZF_MD5_H

#include <cstring>
#include <iostream>

// a small class for calculating MD5 hashes of strings or byte arrays
// it is not meant to be fast or secure
//
// usage: 1) feed it blocks of uchars with update()
//      2) finalize()
//      3) get hexdigest() string
//      or
//      MD5(std::string).hexdigest()
//
// assumes that char is 8 bit and int is 32 bit
class md5 {
  public:
    typedef unsigned int size_type;    // must be 32bit

    md5();
    md5(const std::string& text);
    void update(const unsigned char* input, size_type length);
    void update(const char* input, size_type length);
    md5& finalize();
    std::string hexdigest() const;

    auto digest() const { return &_digest[0]; }

  private:
    enum { BLOCKSIZE = 64 };    // VC6 won't eat a const static int here

    void init();

    void transform(const uint8_t block[BLOCKSIZE]);
    static void decode(uint32_t output[], const uint8_t input[], size_type len);
    static void encode(uint8_t output[], const uint32_t input[], size_type len);

    // low level logic operations
    static inline uint32_t f(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t g(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t h(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t i(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t rotate_left(uint32_t x, int n);
    static inline void
    ff(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
    static inline void
    gg(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
    static inline void
    hh(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
    static inline void
    ii(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);

  private:
    bool _finalized;
    uint8_t _buffer[BLOCKSIZE];    // bytes that didn't fit in last 64 byte chunk
    uint32_t _count[2];            // 64bit counter for number of bits (lo, hi)
    uint32_t _state[4];            // digest so far
    uint8_t _digest[16];           // the result
};

#endif
