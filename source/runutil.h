//----------------------------------------------------
//
//	Utility and runtime procedures header
//	(c) 2005 Kye Bitossi
//
//  Version: $Id: runutil.h,v 1.3 2006/03/20 17:44:19 kyeman Exp $
//
//----------------------------------------------------

#ifndef RUNUTIL_H
#define RUNUTIL_H

#include <stddef.h>

// 0x000e61a0
extern const char g_base64Alphabet[66];
// R5 address to verify
extern const char* g_base64Encoding;

// R5 address to verify
int Util_strnicmp(const char* first, const char* second, size_t count);
// 0x00017930
void Util_strupr(char* text);
// 0x00017870
char* Util_stristr(const char* string, const char* pattern);
// 0x00017b90
void Util_Base64Encode(char* input, char* output);

// R5 address to verify
void Util_UrlUnencode(char* enc);
// R5 address to verify
char Util_toupper(char c);
// R5 address to verify
int Util_wildcmp(char* wild, char* string);
// R5 address to verify
char* Util_strrev(char* str);
// R5 address to verify
char* Util_itoa(int v, char* s, int r);
// R5 address to verify
void K_EncodeString(char* szInput, char* szOutput);
// R5 address to verify
char* K_DecodeString(unsigned char* szInput);

// legacy

// R5 address to verify
bool Util_IsDecimal(const char* text);
// R5 address to verify
void Util_SanitizeAscii(char* text);

#endif // RUNUTIL_H
