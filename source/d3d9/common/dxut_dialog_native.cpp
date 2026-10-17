// Isolated hybrid DXUT candidate; ANSI body verified against RVA 0x0008cae0.
#include "dxstdafx.h"

// samp.dll RVA: 0x0008cae0
HRESULT CDXUTDialog::DrawTextA( const char * strText, CDXUTElement* pElement, RECT* prcDest, bool bShadow, int nCount )
{
    HRESULT hr = S_OK;

    // No need to draw fully transparent layers
    if( pElement->FontColor.Current.a == 0 )
        return S_OK;

    RECT rcScreen = *prcDest;
    OffsetRect( &rcScreen, m_x, m_y );

    // If caption is enabled, offset the Y position by its height.
    if( m_bCaption )
        OffsetRect( &rcScreen, 0, m_nCaptionHeight );

    //debug
    //DrawRect( &rcScreen, D3DCOLOR_ARGB(100, 255, 0, 0) );

    D3DXMATRIXA16 matTransform;
    D3DXMatrixIdentity( &matTransform );
    m_pManager->m_pSprite->SetTransform( &matTransform );

    DXUTFontNode* pFontNode = GetFont( pElement->iFont );
    
    if( bShadow )
    {
        RECT rcShadow = rcScreen;
        OffsetRect( &rcShadow, 1, 1 );
		if(pFontNode->pFont) {
			hr = pFontNode->pFont->DrawTextA( m_pManager->m_pSprite, strText, nCount, &rcShadow, pElement->dwTextFormat, D3DCOLOR_ARGB(DWORD(pElement->FontColor.Current.a * 255), 0, 0, 0) );
		} else {
			m_pManager->CreateFont(pElement->iFont);
		}

		if( FAILED(hr) )
            return hr;
    }

	if(pFontNode->pFont) {
		hr = pFontNode->pFont->DrawTextA( m_pManager->m_pSprite, strText, nCount, &rcScreen, pElement->dwTextFormat, pElement->FontColor.Current );
	} else {
		m_pManager->CreateFont(pElement->iFont);
	}

	if( FAILED(hr) )
        return hr;

    return S_OK;
}
