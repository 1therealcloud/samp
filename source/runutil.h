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

// 0x00118230
extern const char g_base64Alphabet[66];
// 0x0013c4b4
extern const char* g_base64Encoding;

// 0x00017510
int Util_strnicmp(const char* first, const char* second, size_t count);
// 0x00017440
void Util_strupr(char* text);
// 0x00017380
char* Util_stristr(const char* string, const char* pattern);
// 0x000176a0
void Util_Base64Encode(char* input, char* output);

// 0x000172e0
void Util_UrlUnencode(char* enc);
// 0x00017360
char Util_toupper(char c);
// 0x00017470
int Util_wildcmp(char* wild, char* string);
// 0x000175b0
char* Util_strrev(char* str);
// 0x00017600
char* Util_itoa(int v, char* s, int r);
// 0x0009d840
void K_EncodeString(char* szInput, char* szOutput);
// 0x0009d880
char* K_DecodeString(unsigned char* szInput);

// 0x000b55e0
bool Util_IsDecimal(const char* text);
// 0x000b5920
void Util_SanitizeAscii(char* text);

#endif // RUNUTIL_H
