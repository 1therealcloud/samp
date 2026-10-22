// Isolated hybrid DXUT candidate only; see build_dxut_hybrid_v68.cjs.
#include "dxstdafx.h"
#include "gui/font_wrapper.h"
#include <stdio.h>

// samp.dll RVA: 0x0008c8f0
int CDXUTDialogResourceManager::AddFont(const char *face, LONG height, LONG weight) {
    for (int i = 0; i < m_FontCache.GetSize(); ++i) {
        DXUTFontNode *node = m_FontCache.GetAt(i);
        if (!_strnicmp(node->strFace, face, MAX_PATH - 1) &&
            node->nHeight == height && node->nWeight == weight) return i;
    }
    DXUTFontNode *node = new DXUTFontNode;
    if (!node) return -1;
    ZeroMemory(node, sizeof(DXUTFontNode));
    StringCchCopyA(node->strFace, MAX_PATH, face);
    node->nHeight = height; node->nWeight = weight;
    m_FontCache.Add(node);
    int index = m_FontCache.GetSize() - 1;
    if (m_pd3dDevice) CreateFont(index);
    return index;
}

// samp.dll RVA: 0x0008ca50
HRESULT CDXUTDialog::SetFont(UINT index, const char *face, LONG height, LONG weight) {
    for (UINT i = m_Fonts.GetSize(); i <= index; ++i) m_Fonts.Add(-1);
    int font = m_pManager->AddFont(face, height, weight);
    m_Fonts.SetAt(index, font);
    return S_OK;
}

// samp.dll RVA: 0x0008c170
HRESULT CDXUTDialogResourceManager::CreateFont(UINT index) {
    DXUTFontNode *node = m_FontCache.GetAt(index);
    if (node->pFont) { node->pFont->Release(); node->pFont = 0; }
    D3DXCreateFontA(m_pd3dDevice, node->nHeight, 0, node->nWeight, 1, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        node->strFace, &node->pFont);
    CFontWrapper *wrapper = new CFontWrapper;
    wrapper->font = node->pFont;
    node->pFont = wrapper;
    char debug[260];
    sprintf(debug, "CDXUTDialogResourceManager::CreateFont(pFont=0x%X,pD3DDevice=0x%X)", wrapper, m_pd3dDevice);
    return S_OK;
}
