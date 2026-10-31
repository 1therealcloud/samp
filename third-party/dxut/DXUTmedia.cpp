#include "dxstdafx.h"
// 0x00181ac8
char g_dxutMediaSearchPath[MAX_PATH];
// 0x00093290
char* InputDxutMediaSearchPath()
{
    return g_dxutMediaSearchPath;
}

//--------------------------------------------------------------------------------------
// 0x000972b0
HRESULT DXUTFindDXSDKMediaFileCch(char* strDestPath, int cchDest, const char* strFilename)
{
    bool bFound;
    char strSearchFor[MAX_PATH];

    if (NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10)
        return E_INVALIDARG;

    // Get the exe name, and exe path
    char strExePath[MAX_PATH] = {0};
    char strExeName[MAX_PATH] = {0};
    char* strLastSlash = NULL;
    GetModuleFileNameA(NULL, strExePath, MAX_PATH);
    strExePath[MAX_PATH - 1] = 0;
    strLastSlash = strrchr(strExePath, TEXT('\\'));
    if (strLastSlash)
    {
        StringCchCopyA(strExeName, MAX_PATH, &strLastSlash[1]);

        // Chop the exe name from the exe path
        *strLastSlash = 0;

        // Chop the .exe from the exe name
        strLastSlash = strrchr(strExeName, TEXT('.'));
        if (strLastSlash)
            *strLastSlash = 0;
    }

    // Typical directories:
    //      .\
    //      ..\
    //      ..\..\
    //      %EXE_DIR%\
    //      %EXE_DIR%\..\
    //      %EXE_DIR%\..\..\
    //      %EXE_DIR%\..\%EXE_NAME%
    //      %EXE_DIR%\..\..\%EXE_NAME%

    // Typical directory search
    bFound = DXUTFindMediaSearchTypicalDirs(strDestPath, cchDest, strFilename, strExePath, strExeName);
    if (bFound)
        return S_OK;

    // Typical directory search again, but also look in a subdir called "\media\"
    StringCchPrintfA(strSearchFor, MAX_PATH, "media\\%s", strFilename);
    bFound = DXUTFindMediaSearchTypicalDirs(strDestPath, cchDest, strSearchFor, strExePath, strExeName);
    if (bFound)
        return S_OK;

    char strLeafName[MAX_PATH] = {0};

    // Search all parent directories starting at .\ and using strFilename as the leaf name
    StringCchCopyA(strLeafName, MAX_PATH, strFilename);
    bFound = DXUTFindMediaSearchParentDirs(strDestPath, cchDest, ".", strLeafName);
    if (bFound)
        return S_OK;

    // Search all parent directories starting at the exe's dir and using strFilename as the leaf name
    bFound = DXUTFindMediaSearchParentDirs(strDestPath, cchDest, strExePath, strLeafName);
    if (bFound)
        return S_OK;

    // Search all parent directories starting at .\ and using "media\strFilename" as the leaf name
    StringCchPrintfA(strLeafName, MAX_PATH, "media\\%s", strFilename);
    bFound = DXUTFindMediaSearchParentDirs(strDestPath, cchDest, ".", strLeafName);
    if (bFound)
        return S_OK;

    // Search all parent directories starting at the exe's dir and using "media\strFilename" as the leaf name
    bFound = DXUTFindMediaSearchParentDirs(strDestPath, cchDest, strExePath, strLeafName);
    if (bFound)
        return S_OK;

    // On failure, return the file as the path but also return an error code
    StringCchCopyA(strDestPath, cchDest, strFilename);

    return DXUTERR_MEDIANOTFOUND;
}

//--------------------------------------------------------------------------------------
// Search a set of typical directories
//--------------------------------------------------------------------------------------
// 0x000961c0
bool DXUTFindMediaSearchTypicalDirs(char* strSearchPath, int cchSearch, const char* strLeaf, char* strExePath, char* strExeName)
{
    // Typical directories:
    //      .\
    //      ..\
    //      ..\..\
    //      %EXE_DIR%\
    //      %EXE_DIR%\..\
    //      %EXE_DIR%\..\..\
    //      %EXE_DIR%\..\%EXE_NAME%
    //      %EXE_DIR%\..\..\%EXE_NAME%
    //      DXSDK media path

    // Search in .\  
    StringCchCopyA( strSearchPath, cchSearch, strLeaf );
    if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
        return true;

    // Search in ..\  
    StringCchPrintfA( strSearchPath, cchSearch, "..\\%s", strLeaf );
    if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
        return true;

    // Search in ..\..\ 
    StringCchPrintfA( strSearchPath, cchSearch, "..\\..\\%s", strLeaf );
    if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
        return true;

    // Search in ..\..\ 
    StringCchPrintfA( strSearchPath, cchSearch, "..\\..\\%s", strLeaf );
    if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
        return true;

    // Search in the %EXE_DIR%\ 
    StringCchPrintfA( strSearchPath, cchSearch, "%s\\%s", strExePath, strLeaf );
    if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
        return true;

    // Search in the %EXE_DIR%\..\ 
    StringCchPrintfA( strSearchPath, cchSearch, "%s\\..\\%s", strExePath, strLeaf );
    if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
        return true;

    // Search in the %EXE_DIR%\..\..\ 
    StringCchPrintfA( strSearchPath, cchSearch, "%s\\..\\..\\%s", strExePath, strLeaf );
    if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
        return true;

    // Search in "%EXE_DIR%\..\%EXE_NAME%\".  This matches the DirectX SDK layout
    StringCchPrintfA(strSearchPath, cchSearch, "%s\\..\\%s\\%s", strExePath, strExeName, strLeaf);
    if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
        return true;

    // Search in "%EXE_DIR%\..\..\%EXE_NAME%\".  This matches the DirectX SDK layout
    StringCchPrintfA(strSearchPath, cchSearch, "%s\\..\\..\\%s\\%s", strExePath, strExeName, strLeaf);
    if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
        return true;

    // Search in media search dir
    char* s_strSearchPath = InputDxutMediaSearchPath();
    if (s_strSearchPath[0] != 0)
    {
        StringCchPrintfA(strSearchPath, cchSearch, "%s%s", s_strSearchPath, strLeaf);
        if (GetFileAttributesA(strSearchPath) != 0xFFFFFFFF)
            return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------
// Search parent directories starting at strStartAt, and appending strLeafName
// at each parent directory.  It stops at the root directory.
//--------------------------------------------------------------------------------------
// 0x00096320
bool DXUTFindMediaSearchParentDirs(char* strSearchPath, int cchSearch, char* strStartAt, char* strLeafName)
{
    char strFullPath[MAX_PATH] = {0};
    char strFullFileName[MAX_PATH] = {0};
    char strSearch[MAX_PATH] = {0};
    char* strFilePart = NULL;

    GetFullPathNameA(strStartAt, MAX_PATH, strFullPath, &strFilePart);
    if (strFilePart == NULL)
        return false;

    while (strFilePart != NULL && *strFilePart != '\0')
    {
        StringCchPrintfA(strFullFileName, MAX_PATH, "%s\\%s", strFullPath, strLeafName);
        if (GetFileAttributesA(strFullFileName) != 0xFFFFFFFF)
        {
            StringCchCopyA(strSearchPath, cchSearch, strFullFileName);
            return true;
        }

        StringCchPrintfA(strSearch, MAX_PATH, "%s\\..", strFullPath);
        GetFullPathNameA(strSearch, MAX_PATH, strFullPath, &strFilePart);
    }

    return false;
}

//--------------------------------------------------------------------------------------
