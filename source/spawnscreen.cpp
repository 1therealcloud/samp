#include <new>
#include <string.h>

#include "spawnscreen.h"

#include "archive/ArchiveFS.h"
#include "chatwindow.h"
#include "d3dhook/ID3DXFontHook.h"
#include "game/entity.h"
#include "game/textdraw.h"
#include "gui.h"
#include "main.h"
// 0x0011c74c
const char g_spawnVersionText[] = "SA-MP 0.3.DL-R1";

// 0x000705c0
SpawnScreenVertex::SpawnScreenVertex() {}

// 0x000705d0
SpawnScreenVertex::SpawnScreenVertex(const CVector& position, float w, DWORD color, float tu, float tv)
{
    memcpy(&x, &position, 12);
    memcpy(&rhw, &w, 4);
    this->color = color;
    memcpy(&u, &tu, 4);
    memcpy(&v, &tv, 4);
}

// 0x000709d0
CSpawnScreen::~CSpawnScreen()
{
    char* text = *(char**)(unknown00 + 4);
    if (text)
        ::operator delete(text);
    if (texture || states[1] || states[0])
        OnLostDevice();
    CFonts* ownedFonts = fonts;
    if (ownedFonts != g_deviceFonts && ownedFonts)
    {
        delete ownedFonts;
        fonts = 0;
    }
}

// 0x00070a30
void CSpawnScreen::Draw()
{
    if (!fonts || !*(char**)(unknown00 + 4))
        return;
    FontSize measured;
    fonts->GetTextScreenSize(&measured, *(char**)(unknown00 + 4), 0);
    // GTA SA virtual address: screen height 0x00C17048. Integer arithmetic wraps.
    LONG bottom = (LONG)(*(DWORD*)0xc17048 - 30), top = (LONG)((DWORD)bottom - (DWORD)measured.cy);
    RECT textRect = {20, top, (LONG)(20u + (DWORD)measured.cx), bottom};
    fonts->DrawText(0, *(char**)(unknown00 + 4), textRect, 0xffffffff, TRUE);
    int imageTop = (int)((DWORD)top - 103), imageBottom = (int)((DWORD)imageTop + 73);
    if (texture)
    {
        SpawnScreenVertex vertices[4];
        float topY, bottomY;
        // clang-format off
        __asm {
            fild imageTop
            fsub g_textDrawHalf
            fild imageBottom
            fadd g_textDrawHalf
            fstp bottomY
            fstp topY
        }
        // clang-format on
        for (int i = 0; i < 4; ++i)
        {
            vertices[i].z = 0.1f;
            vertices[i].rhw = 1.0f;
            vertices[i].color = 0xffffffff;
        }
        vertices[0].x = 19.5f;
        vertices[0].y = bottomY;
        vertices[0].u = 0;
        vertices[0].v = 1;
        vertices[1].x = 19.5f;
        vertices[1].y = topY;
        vertices[1].u = 0;
        vertices[1].v = 0;
        vertices[2].x = 205.5f;
        vertices[2].y = bottomY;
        vertices[2].u = 1;
        vertices[2].v = 1;
        vertices[3].x = 205.5f;
        vertices[3].y = topY;
        vertices[3].u = 1;
        vertices[3].v = 0;
        states[0]->Capture();
        states[1]->Apply();
        device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        device->SetFVF(0x144);
        device->SetPixelShader(0);
        device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, 28);
        states[0]->Apply();
    }
    FontSize versionSize;
    FontSize* size = fonts->GetTextScreenSize(&versionSize, g_spawnVersionText, 0);
    LONG left = (LONG)(20u + (DWORD)((int)(185u - (DWORD)size->cx) / 2));
    RECT versionRect = {left, imageBottom, (LONG)((DWORD)left + (DWORD)size->cx), (LONG)((DWORD)imageBottom + (DWORD)size->cy)};
    fonts->DrawText(0, g_spawnVersionText, versionRect, 0xff27abfa, TRUE);
}

// 0x00070630
void CSpawnScreen::SetText(const char* value)
{
    char*& text = *(char**)(unknown00 + 4);
    if (text)
        ::operator delete(text);
    text = 0;
    if (value)
    {
        text = (char*)::operator new(strlen(value) + 1);
        strcpy(text, value);
    }
}

// 0x00070940
void CSpawnScreen::OnLostDevice()
{
    if (texture)
        texture->Release();
    if (states[0])
        states[0]->Release();
    if (states[1])
        states[1]->Release();
    texture = 0;
    states[0] = 0;
    states[1] = 0;
    if (fonts && fonts != g_deviceFonts)
    {
        fonts->OnLostDevice();
        fonts = 0;
    }
}

// 0x00070690
void CSpawnScreen::OnResetDevice()
{
    if (device)
    {
        if (texture)
            texture->Release();
        DWORD index = g_pArchive->GetFileIndex(0xbaddea6b);
        if (index != 0xffffffff)
        {
            BYTE* data = g_pArchive->GetFileData(index);
            DWORD size = g_pArchive->GetFileSize(index);
            D3DXCreateTextureFromFileInMemory(device, data, size, &texture);
        }
        for (int i = 0; i < 2; i++)
        {
            device->BeginStateBlock();
            device->SetTexture(0, texture);
            device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            device->SetRenderState(D3DRS_ALPHAREF, 8);
            device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
            device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
            device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
            device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
            device->SetRenderState(D3DRS_CLIPPING, TRUE);
            device->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
            device->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
            device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
            device->SetRenderState(D3DRS_FOGENABLE, FALSE);
            device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
            device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
            device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
            device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
            device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
            device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
            device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
            device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
            device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
            device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
            device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
            device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
            device->EndStateBlock(&states[i]);
        }
    }
    if (fonts && fonts != g_deviceFonts)
        fonts->OnResetDevice();
}

// 0x00070990
CSpawnScreen::CSpawnScreen(IDirect3DDevice9* value)
{
    device = value;
    *(DWORD*)(unknown00 + 4) = 0;
    texture = 0;
    states[1] = 0;
    states[0] = 0;
    if (device)
        fonts = g_deviceFonts;
    OnResetDevice();
}
