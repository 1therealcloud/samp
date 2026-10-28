#include "gui/input_native.h"
#include <DXUT.h>
#include <DXUTgui.h>
// Candidate-only replacements; declarations are supplied by the selected DXUT header.
// 0x0008c860
HRESULT CDXUTDialogResourceManager::OnCreateDevice(IDirect3DDevice9* device)
{
    m_pd3dDevice = device;
    HRESULT result;
    for (int i = 0; i < m_FontCache.GetSize(); ++i)
    {
        result = CreateFont(i);
        if (FAILED(result))
            return result;
    }
    for (int j = 0; j < m_TextureCache.GetSize(); ++j)
    {
        result = CreateTexture(j);
        if (FAILED(result))
            return result;
    }
    result = D3DXCreateSprite(device, &m_pSprite);
    if (FAILED(result))
        return result;
    InputResetIme();
    return S_OK;
}
// 0x00086a00
HRESULT CDXUTDialogResourceManager::OnResetDevice()
{
    for (int i = 0; i < m_FontCache.GetSize(); ++i)
    {
        DXUTFontNode* font = m_FontCache.GetAt(i);
        if (font->pFont)
            font->pFont->OnResetDevice();
    }
    if (m_pSprite)
        m_pSprite->OnResetDevice();
    m_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlock);
    return S_OK;
}
