#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include "../DffFormatDll/clump.h"
#include "../DffFormatDll/collision.h"

// Address comments in this unit refer to the original kdff.exe.

// 0x00003eb0
static bool ProcessFile(const char* Dff, const char* Col)
{
    CClump Clump;
    if (Clump.Load(Dff) < 1) {
        printf("[error] Could not load the dff file: %s\n", Dff);
        return false;
    }
    if (Clump.HasCol) {
        printf("[error] File already has a collision: %s\n", Dff);
        return false;
    }
    int Result = Clump.AppendCol(Col);
    if (Result < 1) {
        printf("[error] Could not append col to file: %s error: %d\n", Col, Result);
        return false;
    }
    if (static_cast<int>(Clump.Save(Dff)) < 1) {
        printf("[error] Could not save clump to file: %s\n", Dff);
        return false;
    }
    return true;
}

// 0x000040d0
static bool Append(const char* Dff, const char* Col, const char* Output)
{
    CClump Clump;
    if (Clump.Load(Dff) < 1) printf("[error] Could not load the dff file: %s\n", Dff);
    else if (Clump.HasCol) printf("[error] File already has a collision: %s\n", Dff);
    else {
        int Result = Clump.AppendCol(Col);
        if (Result < 1) printf("[error] Could not append col to file: %s error: %d\n", Col, Result);
        else if (static_cast<int>(Clump.Save(Output)) < 1)
            printf("[error] Could not save clump to file: %s\n", Output);
        else {
            printf("[success] Model file was appended with collision data\n");
            return true;
        }
    }
    return false;
}

// 0x00004200
static bool Info(const char* Dff)
{
    CClump Clump;
    if (Clump.Load(Dff) < 1) {
        printf("[error] Could not load the dff file: %s\n", Dff);
        return false;
    }
    Clump.ReadFrames();
    Clump.ReadGeometry();
    if (Clump.HasCol) printf("[info] File has a collision\n");
    else printf("[info] File doesn't have a collision\n");
    return true;
}

// 0x000042f0
static bool Import(const char* Dff, const char* NightDff, const char* Output)
{
    CClump Source, Night;
    if (Source.Load(Dff) < 1) {
        printf("[error] Could not load the dff(1) file: %s\n", Dff);
        return false;
    }
    if (Night.Load(NightDff) < 1) {
        printf("[error] Could not load the dff(2) file: %s\n", NightDff);
        return false;
    }
    if (!Source.ReadGeometry()) {
        printf("[error] Could read geometry of dff(1): %s\n", Dff);
        return false;
    }
    if (!Night.ReadGeometry()) {
        printf("[error] Could read geometry of dff(2): %s\n", NightDff);
        return false;
    }
    CGeometry* Geometry = static_cast<CGeometry*>(Source.Geometries.Get(0));
    CGeometry* NightGeometry = static_cast<CGeometry*>(Night.Geometries.Get(0));
    if (!Geometry) {
        printf("[error] Could read geometry of dff(1): %s\n", Dff);
        return false;
    }
    if (!NightGeometry) {
        printf("[error] Could read geometry of dff(2): %s\n", NightDff);
        return false;
    }
    if (!Geometry->ImportNightColors(NightGeometry)) {
        printf("[error] failed to copy vertex colors.\n");
        return false;
    }
    if (!Source.RebuildGeometry()) {
        printf("[error] couldn't rebuild the geometry list.\n");
        return false;
    }
    if (static_cast<int>(Source.Save(Output)) < 1) {
        printf("[error] Could not save clump to file: %s\n", Output);
        return false;
    }
    return true;
}

// 0x00004500, 0x00004700, 0x00004900
// The three native generation handlers share this body with an explicit mode.
static bool Generate(const char* Dff, const char* Output, int Mode)
{
    CClump Clump;
    CColModel Col;
    if (Clump.Load(Dff) < 1) printf("[error] Could not load the dff file: %s\n", Dff);
    else if (!Clump.ReadGeometry()) printf("[error] Couldn't read the geometry for: %s\n", Dff);
    else {
        CGeometry* Geometry = static_cast<CGeometry*>(Clump.Geometries.Get(0));
        bool Result;
        if (Mode == 0) Result = Col.GenerateEmpty(Geometry);
        else if (Mode == 1) Result = Col.GenerateBox(Geometry);
        else Result = Col.GenerateFaces(Geometry);
        if (!Result) printf("[error] Couldn't generate the col");
        else if (!Col.Save(Output, "samp")) printf("[error] Could not save col to file: %s\n", Output);
        else return true;
    }
    return false;
}


// Private symbols are descriptive; all addresses refer to the original kdff.exe.
#pragma pack(push, 1)
struct CommandLine {
    unsigned char Mode;
    char Dff[261], Other[261], Output[261], GenerationMode[65];
};
#pragma pack(pop)

// 0x00012f20
static CommandLine Options;

// 0x00003a50
static void ReadQuoted(const char* Input, char* Output)
{
    while (*Input != '"') ++Input;
    ++Input;
    while (*Input && *Input != '"') *Output++ = *Input++;
    *Output = 0;
}

// 0x00003a90
static void ReadValue(const char* Input, char* Output)
{
    while (*Input == ' ') ++Input;
    if (*Input == '"') { ReadQuoted(Input, Output); return; }
    while (*Input && *Input != ' ' && *Input != '-' && *Input != '/')
        *Output++ = *Input++;
    *Output = 0;
}

// 0x00003af0
static void ParseCommandLine()
{
    const char* Input = GetCommandLineA();
    memset(&Options, 0, sizeof(Options));
    while (*Input) {
        if (*Input == '-' || *Input == '/') {
            ++Input;
            switch (*Input) {
            case 'a': Options.Mode = 2; break;
            case 'i': Options.Mode = 1; break;
            case 'p': Options.Mode = 6; break;
            case 'v': Options.Mode = 7; break;
            case 'c': case 'n': ReadValue(++Input, Options.Other); break;
            case 'd': ReadValue(++Input, Options.Dff); break;
            case 'o': ReadValue(++Input, Options.Output); break;
            case 'g': Options.Mode = 4; ReadValue(++Input, Options.GenerationMode); break;
            }
        }
        ++Input;
    }
}

// 0x00003bd0
static bool ChangeExtension(const char* FileName, char* Output, const char* Extension)
{
    char Path[264];
    strcpy(Path, FileName);
    char* Dot = strrchr(Path, '.');
    if (!Dot) return false;
    *Dot = 0;
    sprintf(Output, "%s.%s", Path, Extension);
    return true;
}

// 0x00003c60
static void Usage()
{
    printf("[usage] Modes: -i (info) -a (append col) -p (process all) -v (night vertex)\n");
    printf("[usage] Modes: -g [mode] (generate col for dff) -g modes: empty, box, mesh\n");
    printf("[usage] Input: -d [dff file] -c [collision file] (col ver3) -o [output file]\n");
    printf("[usage] Example (append col to dff):  kdff.exe -a -d MyHouse.dff -c MyHouse.col -o MyHouseC.dff\n");
    printf("[usage] Example (show info for dff):  kdff.exe -i -d MyHouse.dff\n");
    printf("[usage] Example (generate empty col): kdff.exe -g empty -d MyHouse.dff -o MyHouse.col\n");
    printf("[usage] Example (import NVC from -n): kdff.exe -v -d MyHouseDT.dff -n MyHouseNT.dff -o MyHouseNVC.dff\n");
    printf("[usage] Example (all in current dir): kdff.exe -p (process *.dff in cwd and attach *.col)\n");
}

// 0x00004010
static void ProcessAll()
{
    char Directory[264], Col[264];
    _getcwd(Directory, 260);
    int Count = 0;
    WIN32_FIND_DATAA Data;
    HANDLE Find = FindFirstFileA("*.dff", &Data);
    if (Find != INVALID_HANDLE_VALUE) {
        do {
            if (!(Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                ChangeExtension(Data.cFileName, Col, "col");
                ProcessFile(Data.cFileName, Col);
                ++Count;
            }
        } while (FindNextFileA(Find, &Data));
        FindClose(Find);
    }
    printf("[info] Processed %d files", Count);
}

// 0x00004b00
int main()
{
    ParseCommandLine();
    if (Options.Mode == 2 && *Options.Dff && *Options.Other && *Options.Output)
        Append(Options.Dff, Options.Other, Options.Output);
    else if (Options.Mode == 6) ProcessAll();
    else if (Options.Mode == 1 && *Options.Dff)
        Info(Options.Dff);
    else if (Options.Mode == 7 && *Options.Dff && *Options.Other && *Options.Output)
        Import(Options.Dff, Options.Other, Options.Output);
    else if (Options.Mode == 4 && *Options.GenerationMode && *Options.Output && *Options.Dff) {
        if (!_stricmp(Options.GenerationMode, "empty"))
            Generate(Options.Dff, Options.Output, 0);
        else if (!_stricmp(Options.GenerationMode, "box"))
            Generate(Options.Dff, Options.Output, 1);
        else if (!_stricmp(Options.GenerationMode, "mesh"))
            Generate(Options.Dff, Options.Output, 2);
        else Usage();
    }
    else Usage();
    return 0;
}
