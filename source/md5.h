// See md5.cpp for code license.

#ifndef MD5_H
#define MD5_H

#ifdef __alpha
typedef unsigned int uint32;
#else
typedef unsigned long uint32;
#endif

struct MD5Context
{
    uint32 buf[4];
    uint32 bits[2];
    unsigned char in[64];
};

// 0x00062b30
void MD5Init(struct MD5Context* context);
// 0x000633e0
void MD5Update(struct MD5Context* context, unsigned char const* buf, unsigned len);
// 0x000634b0
void MD5Final(unsigned char digest[16], struct MD5Context* context);
// 0x00062b60
void MD5Transform(uint32 buf[4], uint32 const in[16]);

/*
 * This is needed to make RSAREF happy on some MS-DOS compilers.
 */
typedef struct MD5Context MD5_CTX;

// Original x86 context extent; the finalizer deliberately clears sizeof(pointer).
typedef char NativeMD5ContextExtent[(sizeof(MD5Context) == 88 && sizeof(uint32) == 4) ? 1 : -1];

#endif /* !MD5_H */
