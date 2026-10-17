// Utility procedures derived from the supplied SA-MP 0.2x reference.
// Original copyright (c) 2005 Kye Bitossi.
// Only functions with independently verified 0.3.DL-R1 machine-code matches.

#include "runutil.h"
#include <string.h>

// samp.dll RVA: 0x000172e0
void Util_UrlUnencode(char *enc)
{
	char *write_pos = enc;

	while(*enc)
	{
		if(*enc=='%')
		{
			*write_pos = (*++enc>'/'&&*enc<':')?((*enc-('0'))<<4):((*enc-('7'))<<4);
			*write_pos |= (*++enc>'/'&&*enc<':')?(*enc-'0'):(*enc-'7');
		}
		else if (*enc=='+')
			*write_pos= ' ';
		else
			*write_pos= *enc;

		write_pos++; enc++;
	}
	*write_pos='\0';
}

// samp.dll RVA: 0x00017360
char Util_toupper(char c) {return ((c>(char)0x60) && (c<(char)0x7b))? c-0x20:c;}

// samp.dll RVA: 0x00017470
int Util_wildcmp(char *wild, char *string)
{
	char *cp, *mp;
	
	while((*string) && (*wild != '*'))
	{
		if((*wild != *string) && (*wild != '?'))
		{
			return 0;
		}
		wild++;
		string++;
	}
		
	while (*string)
	{
		if (*wild == '*')
		{
			if (!*++wild)
			{
				return 1;
			}
			mp = wild;
			cp = string+1;
		}
		else if ((*wild == *string) || (*wild == '?'))
		{
			wild++;
			string++;
		}
		else
		{
			wild = mp;
			string = cp++;
		}
	}
		
	while (*wild == '*')
	{
		wild++;
	}

	return !*wild;
}

// samp.dll RVA: 0x000175b0
char *Util_strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

// samp.dll RVA: 0x00017600
char * Util_itoa(int v, char *s, int r)
{
	int i,neg = 0;
	char *p = s;
	char *q = s;

	if (r < 0 || r > 35) {
		*s = 0;
		return (s);
		}
	if (r == 0) r = 10;
	if (v == 0) {
		*p++ = '0';
		*p = 0;
		return (s);
		}
	if (v < 0) {
		neg = 1;
		v = -v;
		}
	while (v > 0) {
		i = v % r;
		if (i > 9) i += 7;
		*p++ = '0' + i;
		v /= r;
		}
	if (neg) *p++ = '-';
	*p-- = 0;
	q = s;
	while (p > q) {
		i = *q;
		*q++ = *p;
		*p-- = i;
		}
	return (s);
}

// samp.dll RVA: 0x0009d840
void K_EncodeString(char *szInput, char *szOutput)
{
	char b;

	while(*szInput) {
		b = *szInput;
		_asm mov bl, b
		_asm ror bl, 3
		_asm mov b, bl
		*szOutput = b;
		szInput++;
		szOutput++;
	}
	*szOutput = 0;
}

// samp.dll RVA: 0x0009d880
char * K_DecodeString(unsigned char *szInput)
{
	char b;
	char *st = (char *)szInput;
    
	while(*szInput) {
		b = *szInput;
		_asm mov bl, b
		_asm rol bl, 3
		_asm mov b, bl
		*szInput = b;
		szInput++;
	}

	return st;
}
