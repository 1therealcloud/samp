// Utility procedures derived from the supplied SA-MP 0.2x reference.
// Original copyright (c) 2005 Kye Bitossi.
// Recovered against 0.3.DL-R1 instructions; verification scope is recorded in analysis.

#include "runutil.h"
#include <string.h>

// samp.dll RVA: 0x000172e0
void Util_UrlUnencode(char* enc)
{
    char* write_pos = enc;

    while (*enc)
    {
        if (*enc == '%')
        {
            *write_pos = (*++enc > '/' && *enc < ':') ? ((*enc - ('0')) << 4) : ((*enc - ('7')) << 4);
            *write_pos |= (*++enc > '/' && *enc < ':') ? (*enc - '0') : (*enc - '7');
        }
        else if (*enc == '+')
            *write_pos = ' ';
        else
            *write_pos = *enc;

        write_pos++;
        enc++;
    }
    *write_pos = '\0';
}

// samp.dll RVA: 0x00017360
char Util_toupper(char c)
{
    return ((c > (char)0x60) && (c < (char)0x7b)) ? c - 0x20 : c;
}

// samp.dll RVA: 0x00017440
void Util_strupr(char* text)
{
    while (*text)
    {
        *text = Util_toupper(*text);
        ++text;
    }
}
// samp.dll RVA: 0x00017510
int Util_strnicmp(const char* first, const char* second, size_t count)
{
    if (count == 0)
        return 0;
    do
    {
        if (Util_toupper((unsigned char)*first) != Util_toupper((unsigned char)*second++))
            return (int)Util_toupper((unsigned char)*first) - (int)Util_toupper((unsigned char)*--second);
        if (*first++ == 0)
            break;
    }
    while (--count != 0);
    return 0;
}

// samp.dll RVA: 0x00017470
int Util_wildcmp(char* wild, char* string)
{
    char *cp, *mp;

    while ((*string) && (*wild != '*'))
    {
        if ((*wild != *string) && (*wild != '?'))
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
            cp = string + 1;
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
char* Util_strrev(char* str)
{
    char *p1, *p2;

    if (!str || !*str)
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
char* Util_itoa(int v, char* s, int r)
{
    int i, neg = 0;
    char* p = s;
    char* q = s;

    if (r < 0 || r > 35)
    {
        *s = 0;
        return (s);
    }
    if (r == 0)
        r = 10;
    if (v == 0)
    {
        *p++ = '0';
        *p = 0;
        return (s);
    }
    if (v < 0)
    {
        neg = 1;
        v = -v;
    }
    while (v > 0)
    {
        i = v % r;
        if (i > 9)
            i += 7;
        *p++ = '0' + i;
        v /= r;
    }
    if (neg)
        *p++ = '-';
    *p-- = 0;
    q = s;
    while (p > q)
    {
        i = *q;
        *q++ = *p;
        *p-- = i;
    }
    return (s);
}

// samp.dll RVA: 0x0009d840
void K_EncodeString(char* szInput, char* szOutput)
{
    char b;

    while (*szInput)
    {
        b = *szInput;
        // clang-format off
		_asm mov bl, b
		_asm ror bl, 3
		_asm mov b, bl
                // clang-format on
                *szOutput = b;
        szInput++;
        szOutput++;
    }
    *szOutput = 0;
}

// samp.dll RVA: 0x0009d880
char* K_DecodeString(unsigned char* szInput)
{
    char b;
    char* st = (char*)szInput;

    while (*szInput)
    {
        b = *szInput;
        // clang-format off
		_asm mov bl, b
		_asm rol bl, 3
		_asm mov b, bl
                // clang-format on
                *szInput = b;
        szInput++;
    }

    return st;
}

// samp.dll RVA: 0x00118230
const char g_base64Alphabet[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
// samp.dll RVA: 0x0013c4b4
const char* g_base64Encoding = g_base64Alphabet;

// samp.dll RVA: 0x00017380
char* Util_stristr(const char* String, const char* Pattern)
{
    char *pptri, *sptri, *starti;

    for (starti = (char*)String; *starti != '\0'; starti++)
    {
        /* find start of pattern in string */
        for (; ((*starti != '\0') && (Util_toupper(*starti) != Util_toupper(*Pattern))); starti++)
            ;

        pptri = (char*)Pattern;
        sptri = (char*)starti;

        while (Util_toupper(*sptri) == Util_toupper(*pptri))
        {
            sptri++;
            pptri++;

            /* if end of pattern then pattern was found */
            if ('\0' == *pptri)
                return (starti);
        }
    }

    return (0);
}

// samp.dll RVA: 0x000176a0
void Util_Base64Encode(char* cpInput, char* cpOutput)
{
    int nIdx[4];
    while ('\0' != *cpInput)
    {
        nIdx[0] = ((*cpInput) & 0xFC) >> 2;
        nIdx[1] = ((*cpInput) & 0x03) << 4;
        cpInput++;
        if ('\0' != *cpInput)
        {
            nIdx[1] |= ((*cpInput) & 0xF0) >> 4;
            nIdx[2] = ((*cpInput) & 0x0F) << 2;
            cpInput++;
            if ('\0' != (*cpInput))
            {
                nIdx[2] |= ((*cpInput) & 0xC0) >> 6;
                nIdx[3] = (*cpInput) & 0x3F;
                cpInput++;
            }
            else
                nIdx[3] = 64;
        }
        else
        {
            nIdx[2] = 64;
            nIdx[3] = 64;
        }

        *(cpOutput + 0) = *(g_base64Encoding + nIdx[0]);
        *(cpOutput + 1) = *(g_base64Encoding + nIdx[1]);
        *(cpOutput + 2) = *(g_base64Encoding + nIdx[2]);
        *(cpOutput + 3) = *(g_base64Encoding + nIdx[3]);
        cpOutput += 4;
    }

    *cpOutput = '\0';

    return;
}

// samp.dll RVA: 0x000b55e0
bool Util_IsDecimal(const char* text)
{
    while (*text)
    {
        if (*text < '0' || *text > '9')
            return false;
        ++text;
    }
    return true;
}
// samp.dll RVA: 0x000b5920
void Util_SanitizeAscii(char* text)
{
    while (*text)
    {
        unsigned char value = (unsigned char)*text;
        if (value < 0x20 || value > 0x7f)
            *text = ' ';
        ++text;
    }
}

#include <windows.h>
// Explicit ANSI x86 Toolhelp layout, independent of production UNICODE defines.
// Observed native entry size548, module-name offset32 and base/size offsets20/24.
struct NativeModuleEntry32A
{
    DWORD size, moduleId, processId, globalReferences, processReferences;
    DWORD baseAddress, baseSize, moduleHandle;
    char moduleName[256];
    char executablePath[260];
};
// samp.dll RVA: 0x0018dc3c
unsigned long g_clientD3D9BaseEncoded;
// samp.dll RVA: 0x0018dc40
unsigned long g_clientD3D9ImageSize;
// samp.dll RVA: 0x0009d8c0
unsigned long Util_RefreshD3D9ModuleInfo()
{
    unsigned char d3d9[] = {0x8c, 0x66, 0x8c, 0x27, 0xc5, 0x8c, 0x8d, 0x8d, 0x0};
    char* d3d9Name = K_DecodeString(d3d9);
    unsigned char kernel[] = {0x6d, 0xac, 0x4e, 0xcd, 0xac, 0x8d, 0x66, 0x46, 0xc5, 0x8c, 0x8d, 0x8d, 0x0};
    char* kernelName = K_DecodeString(kernel);
    unsigned char snapshot[] = {0x68, 0x4e, 0xac, 0x2c, 0x8e, 0xac, 0x8a, 0xed, 0xed, 0x8d, 0xd, 0xac, 0x8d, 0xe, 0x66, 0x46, 0x6a, 0xcd, 0x2c, 0xe, 0x6e, 0xd, 0xed, 0x8e, 0x0};
    unsigned char first[] = {0xa9, 0xed, 0x8c, 0xae, 0x8d, 0xac, 0x66, 0x46, 0xc8, 0x2d, 0x4e, 0x6e, 0x8e, 0x0};
    unsigned char next[] = {0xa9, 0xed, 0x8c, 0xae, 0x8d, 0xac, 0x66, 0x46, 0xc9, 0xac, 0xf, 0x8e, 0x0};
    HMODULE kernelModule = GetModuleHandleA(kernelName);
    char* snapshotName = K_DecodeString(snapshot);
    FARPROC(WINAPI * resolve)(HMODULE, LPCSTR) = GetProcAddress;
    typedef HANDLE(WINAPI * SnapshotFunction)(DWORD, DWORD);
    typedef BOOL(WINAPI * ModuleFunction)(HANDLE, NativeModuleEntry32A*);
    SnapshotFunction createSnapshot = (SnapshotFunction)resolve(kernelModule, snapshotName);
    ModuleFunction firstModule = (ModuleFunction)resolve(kernelModule, K_DecodeString(first));
    ModuleFunction nextModule = (ModuleFunction)resolve(kernelModule, K_DecodeString(next));
    HANDLE handle = createSnapshot(8, GetCurrentProcessId());
    if (handle == INVALID_HANDLE_VALUE)
        return 0;
    NativeModuleEntry32A entry;
    entry.size = 0x224;
    if (!firstModule(handle, &entry))
    {
        CloseHandle(handle);
        return 0;
    }
    unsigned long matches = 0;
    char moduleName[MAX_PATH];
    do
    {
        // Native keeps this unbounded intermediate copy before its exact strcmp.
        strcpy(moduleName, entry.moduleName);
        if (!strcmp(d3d9Name, moduleName))
        {
            g_clientD3D9BaseEncoded = entry.baseAddress ^ 0xacacacac;
            g_clientD3D9ImageSize = entry.baseSize;
            ++matches;
        }
    }
    while (nextModule(handle, &entry));
    CloseHandle(handle);
    return matches;
}
