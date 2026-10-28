// Native ANSI texture closure; isolated hybrid DXUT candidate.
#include "dxstdafx.h"
#include "dxut/dxut_media_native.h"
// 0x000897d0
int CDXUTDialogResourceManager::AddTexture(const char* strFilename)
{
    // See if this texture already exists
    for (int i = 0; i < m_TextureCache.GetSize(); i++)
    {
        DXUTTextureNode* pTextureNode = m_TextureCache.GetAt(i);
        if (0 == _strnicmp(pTextureNode->strFilename, strFilename, MAX_PATH - 1))
        {
            return i;
        }
    }

    // Add a new texture and try to create it
    DXUTTextureNode* pNewTextureNode = new DXUTTextureNode();
    if (pNewTextureNode == NULL)
        return -1;

    ZeroMemory(pNewTextureNode, sizeof(DXUTTextureNode));
    StringCchCopyA(pNewTextureNode->strFilename, MAX_PATH, strFilename);
    m_TextureCache.Add(pNewTextureNode);

    int iTexture = m_TextureCache.GetSize() - 1;

    // If a device is available, try to create immediately
    if (m_pd3dDevice)
        CreateTexture(iTexture);

    return iTexture;
}

// 0x00089890
HRESULT CDXUTDialog::SetTexture(UINT index, const char* strFilename)
{
    // If this assert triggers, you need to call CDXUTDialog::Init() first.  This change
    // was made so that the DXUT's GUI could become seperate and optional from DXUT's core.  The
    // creation and interfacing with CDXUTDialogResourceManager is now the responsibility
    // of the application if it wishes to use DXUT's GUI.
    assert(m_pManager != NULL && "To fix call CDXUTDialog::Init() first.  See comments for details.");

    // Make sure the list is at least as large as the index being set
    for (UINT i = m_Textures.GetSize(); i <= index; i++)
    {
        m_Textures.Add(-1);
    }

    int iTexture = m_pManager->AddTexture(strFilename);

    m_Textures.SetAt(index, iTexture);
    return S_OK;
}

// 0x000874b0
HRESULT CDXUTDialogResourceManager::CreateTexture(UINT iTexture)
{
    HRESULT hr = S_OK;

    DXUTTextureNode* pTextureNode = m_TextureCache.GetAt(iTexture);

    //_ASSERT(pTextureNode->strFilename[0] == 0);

    // Make sure there's a texture to create
    if (pTextureNode->strFilename[0] == 0)
    {
        return S_OK;
    }

    // Find the texture on the hard drive
    char strPath[MAX_PATH];
    hr = DXUTFindDXSDKMediaFileCch(strPath, MAX_PATH, pTextureNode->strFilename);
    if (FAILED(hr))
    {
        return hr;
    }

    // Create texture
    D3DXIMAGE_INFO info;
    hr = D3DXCreateTextureFromFileExA(m_pd3dDevice, strPath, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info,
                                      NULL, &pTextureNode->pTexture);
    if (FAILED(hr))
    {
        return hr;
    }

    // Store dimensions
    pTextureNode->dwWidth = info.Width;
    pTextureNode->dwHeight = info.Height;

    return S_OK;
}
