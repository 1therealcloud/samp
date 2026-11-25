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

extern const char g_base64Alphabet[66];

extern const char* g_base64Encoding;

int Util_strnicmp(const char* first, const char* second, size_t count);

void Util_strupr(char* text);

char* Util_stristr(const char* string, const char* pattern);

void Util_Base64Encode(char* input, char* output);

void Util_UrlUnencode(char* enc);

char Util_toupper(char c);

int Util_wildcmp(char* wild, char* string);

char* Util_strrev(char* str);

char* Util_itoa(int v, char* s, int r);

void K_EncodeString(char* szInput, char* szOutput);

char* K_DecodeString(unsigned char* szInput);

#endif // RUNUTIL_H
