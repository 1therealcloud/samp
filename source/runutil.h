#ifndef SAMP_RUNUTIL_H
#define SAMP_RUNUTIL_H

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
