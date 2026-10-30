#ifndef SAMP_RUNUTIL_H
#define SAMP_RUNUTIL_H
#include <stddef.h>
// Signed ASCII comparison; zero length reads neither input.
// 0x00017510
int Util_strnicmp(const char* first, const char* second, size_t count);
// 0x00017440
void Util_strupr(char* text);
// 0x00017380
char* Util_stristr(const char* string, const char* pattern);
// C-string encoder, not a counted binary encoder.
// 0x000176a0
void Util_Base64Encode(char* input, char* output);
// 0x00118230
extern const char g_base64Alphabet[66];
// 0x0013c4b4
extern const char* g_base64Encoding;

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

// Last matching D3D9 base is XOR-encoded; unchanged when no module matches.
// 0x0018dc3c
extern unsigned long g_clientD3D9BaseEncoded;
// 0x0018dc40
extern unsigned long g_clientD3D9ImageSize;
// Dynamic ANSI Toolhelp lookup of current process, with native failure behavior.
// 0x0009d8c0
unsigned long Util_RefreshD3D9ModuleInfo();

// Names are descriptive recovery names, not claims about original source spelling.
// 0x000b5950
int ChatIsHexDigit(char value);
// 0x000b59c0
unsigned long ChatParseColorTag(char* text);
// 0x000b5c00
void ChatStripColorTags(char* text);
// 0x000b5c40
int ChatPrepareMessage(char* text, int maximumLength);

#endif
