#ifndef SAMP_RUNUTIL_H
#define SAMP_RUNUTIL_H
// samp.dll RVA: 0x00017380
char *Util_stristr(const char *string,const char *pattern);
// C-string encoder, not a counted binary encoder.
// samp.dll RVA: 0x000176a0
void Util_Base64Encode(char *input,char *output);
// samp.dll RVA: 0x00118230
extern const char g_base64Alphabet[66];
// samp.dll RVA: 0x0013c4b4
extern const char *g_base64Encoding;

// samp.dll RVA: 0x000172e0
void Util_UrlUnencode(char *enc);
// samp.dll RVA: 0x00017360
char Util_toupper(char c);
// samp.dll RVA: 0x00017470
int Util_wildcmp(char *wild, char *string);
// samp.dll RVA: 0x000175b0
char *Util_strrev(char *str);
// samp.dll RVA: 0x00017600
char *Util_itoa(int v, char *s, int r);
// samp.dll RVA: 0x0009d840
void K_EncodeString(char *szInput, char *szOutput);
// samp.dll RVA: 0x0009d880
char *K_DecodeString(unsigned char *szInput);

#endif

// samp.dll RVA: 0x000b55e0
bool Util_IsDecimal(const char *text);
// samp.dll RVA: 0x000b5920
void Util_SanitizeAscii(char *text);

// Last matching D3D9 base is XOR-encoded; unchanged when no module matches.
// samp.dll RVA: 0x0018dc3c
extern unsigned long g_clientD3D9BaseEncoded;
// samp.dll RVA: 0x0018dc40
extern unsigned long g_clientD3D9ImageSize;
// Dynamic ANSI Toolhelp lookup of current process, with native failure behavior.
// samp.dll RVA: 0x0009d8c0
unsigned long Util_RefreshD3D9ModuleInfo();
