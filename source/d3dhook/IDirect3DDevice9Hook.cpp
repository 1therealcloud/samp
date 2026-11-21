// IDirect3DDevice9Hook.cpp

#include "d3dhook/IDirect3DDevice9Hook.h"

#include "anticheat.h"
#include "game/game.h"
#include "game/hooks.h"
#include "game/playerped.h"
#include "game/vehicle.h"
#include "gui/chatwindow.h"
#include "gui/cmdwindow.h"
#include "gui/gui.h"
#include "main.h"
#include "net/vehiclepool.h"

#include <BitStream.h>
#include <DXUT.h>
#include <DXUTgui.h>
#include <RakClientInterface.h>

#include <stdio.h>
#include <string.h>

// 0x00140d70
D3DMATRIX g_deviceViewMatrix;
// 0x00140db0
D3DMATRIX g_deviceProjectionMatrix;
// R5 address to verify
D3DMATRIX g_deviceWorldMatrix;
// 0x001a25f8
BOOL g_deviceMaterialOverrideEnabled;
// 0x001a25fc
BYTE* g_deviceMaterialOverrides;
// R5 address to verify
DWORD g_deviceMaterialIndex;
// R5 address to verify
void* g_devicePlateTexture;
// 0x0026eba0
BYTE* g_devicePlateManager;
// 0x0026eb8c
CAudioStream* g_renderAudioStream;
// R5 address to verify
CChatBubble* g_renderChatBubble;
// 0x0026eb50
CDialog* g_renderDialog;
// R5 address to verify
CRenderPanel* g_renderPanel;
// 0x0026eb58
CDownloadWindow* g_renderDownloadWindow;
// 0x0026eb64
CObjectSelection* g_renderObjectSelection;
// 0x0026eb68
CTextDrawSelection* g_renderTextDrawSelection;
// R5 address to verify
CNetStats* g_renderNetStats;
// R5 address to verify
CSrvNetStats* g_renderServerNetStats;
// R5 address to verify
CHelpDialog* g_renderHelp;
// R5 address to verify
CDXUTDialog* g_renderFirstDialog;
// R5 address to verify
CDXUTDialog* g_renderSecondDialog;
// 0x0012de64
DWORD g_renderScreenshotRequested;
// R5 address to verify
BYTE g_renderShowVehicleInfo;
// R5 address to verify
GroundHeightFunction g_renderGroundHeight = (GroundHeightFunction)0x569660; // GTA SA virtual address.
// 0x000e9b9c
int g_renderMapClickRpc = 119;

// R5 address to verify
IDirect3DDevice9Hook::IDirect3DDevice9Hook() {}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::QueryInterface(REFIID riid, void** ppvObj)
{
    return g_gameD3DDevice->QueryInterface(riid, ppvObj);
}

// R5 address to verify
ULONG __stdcall IDirect3DDevice9Hook::AddRef()
{
    return g_gameD3DDevice->AddRef();
}

// R5 address to verify
ULONG __stdcall IDirect3DDevice9Hook::Release()
{
    return g_gameD3DDevice->Release();
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::TestCooperativeLevel()
{
    return g_gameD3DDevice->TestCooperativeLevel();
}

// R5 address to verify
UINT __stdcall IDirect3DDevice9Hook::GetAvailableTextureMem()
{
    return g_gameD3DDevice->GetAvailableTextureMem();
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::EvictManagedResources()
{
    return g_gameD3DDevice->EvictManagedResources();
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetDirect3D(IDirect3D9** ppD3D9)
{
    return g_gameD3DDevice->GetDirect3D(ppD3D9);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetDeviceCaps(D3DCAPS9* pCaps)
{
    return g_gameD3DDevice->GetDeviceCaps(pCaps);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
    return g_gameD3DDevice->GetDisplayMode(iSwapChain, pMode);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
    return g_gameD3DDevice->GetCreationParameters(pParameters);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
    return g_gameD3DDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

// R5 address to verify
void __stdcall IDirect3DDevice9Hook::SetCursorPosition(int X, int Y, DWORD Flags)
{
    g_gameD3DDevice->SetCursorPosition(X, Y, Flags);
}

// R5 address to verify
BOOL __stdcall IDirect3DDevice9Hook::ShowCursor(BOOL bShow)
{
    return g_gameD3DDevice->ShowCursor(bShow);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
    return g_gameD3DDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
    return g_gameD3DDevice->GetSwapChain(iSwapChain, pSwapChain);
}

// R5 address to verify
UINT __stdcall IDirect3DDevice9Hook::GetNumberOfSwapChains()
{
    return g_gameD3DDevice->GetNumberOfSwapChains();
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    ClientDeviceObjectsLost();
    HRESULT result = g_gameD3DDevice->Reset(pPresentationParameters);
    if (SUCCEEDED(result))
        ClientDeviceObjectsReset();
    return result;
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
    return g_gameD3DDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
    return g_gameD3DDevice->GetRasterStatus(iSwapChain, pRasterStatus);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetDialogBoxMode(BOOL bEnableDialogs)
{
    return g_gameD3DDevice->SetDialogBoxMode(bEnableDialogs);
}

// R5 address to verify
void __stdcall IDirect3DDevice9Hook::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
    g_gameD3DDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
}

// R5 address to verify
void __stdcall IDirect3DDevice9Hook::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
    g_gameD3DDevice->GetGammaRamp(iSwapChain, pRamp);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture,
                                                      HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
                                                            IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture,
                                                          HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
                                                           IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,
                                                                  BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateDepthStencilSurface(Width, Height, Format, D3DMULTISAMPLE_NONE, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
    return g_gameD3DDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
    return g_gameD3DDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
    return g_gameD3DDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
    return g_gameD3DDevice->GetFrontBufferData(iSwapChain, pDestSurface);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect,
                                                    D3DTEXTUREFILTERTYPE Filter)
{
    return g_gameD3DDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
    return g_gameD3DDevice->ColorFill(pSurface, pRect, color);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
    return g_gameD3DDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
    return g_gameD3DDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
    return g_gameD3DDevice->SetDepthStencilSurface(pNewZStencil);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
    return g_gameD3DDevice->GetDepthStencilSurface(ppZStencilSurface);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::BeginScene()
{
    return g_gameD3DDevice->BeginScene();
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::EndScene()
{
    GameDeviceIntegrityCycle();
    HRESULT result = g_gameD3DDevice->EndScene();
    GameDeviceIntegrityCycle();
    return result;
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
    return g_gameD3DDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* mat)
{
    if (State == D3DTS_VIEW)
        memcpy(&g_deviceViewMatrix, mat, 64);
    else if (State == D3DTS_PROJECTION)
        memcpy(&g_deviceProjectionMatrix, mat, 64);
    else if (State == D3DTS_WORLD)
        memcpy(&g_deviceWorldMatrix, mat, 64);
    return g_gameD3DDevice->SetTransform(State, mat);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* mat)
{
    return g_gameD3DDevice->GetTransform(State, mat);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::MultiplyTransform(D3DTRANSFORMSTATETYPE argument0, CONST D3DMATRIX* argument1)
{
    return g_gameD3DDevice->MultiplyTransform(argument0, argument1);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
    return g_gameD3DDevice->SetViewport(pViewport);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetViewport(D3DVIEWPORT9* pViewport)
{
    return g_gameD3DDevice->GetViewport(pViewport);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
    return g_gameD3DDevice->SetMaterial(pMaterial);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetMaterial(D3DMATERIAL9* pMaterial)
{
    return g_gameD3DDevice->GetMaterial(pMaterial);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetLight(DWORD Index, CONST D3DLIGHT9* argument1)
{
    return g_gameD3DDevice->SetLight(Index, argument1);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetLight(DWORD Index, D3DLIGHT9* argument1)
{
    return g_gameD3DDevice->GetLight(Index, argument1);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::LightEnable(DWORD Index, BOOL Enable)
{
    return g_gameD3DDevice->LightEnable(Index, Enable);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetLightEnable(DWORD Index, BOOL* pEnable)
{
    return g_gameD3DDevice->GetLightEnable(Index, pEnable);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetClipPlane(DWORD Index, CONST float* pPlane)
{
    return g_gameD3DDevice->SetClipPlane(Index, pPlane);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetClipPlane(DWORD Index, float* pPlane)
{
    return g_gameD3DDevice->GetClipPlane(Index, pPlane);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
    return g_gameD3DDevice->SetRenderState(State, Value);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
    return g_gameD3DDevice->GetRenderState(State, pValue);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
    return g_gameD3DDevice->CreateStateBlock(Type, ppSB);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::BeginStateBlock()
{
    return g_gameD3DDevice->BeginStateBlock();
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
    return g_gameD3DDevice->EndStateBlock(ppSB);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
    return g_gameD3DDevice->SetClipStatus(pClipStatus);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
    return g_gameD3DDevice->GetClipStatus(pClipStatus);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
    return g_gameD3DDevice->GetTexture(Stage, ppTexture);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
    if (g_deviceMaterialOverrideEnabled && g_deviceMaterialOverrides)
    {
        for (g_deviceMaterialIndex = 0; g_deviceMaterialIndex != 16; ++g_deviceMaterialIndex)
        {
            DWORD index = g_deviceMaterialIndex;
            if (*(IDirect3DBaseTexture9**)(g_deviceMaterialOverrides + 0x110b + index * 4) == pTexture && *(DWORD*)(g_deviceMaterialOverrides + 0x114b + index * 4))
                return g_gameD3DDevice->SetTexture(Stage, *(IDirect3DBaseTexture9**)(g_deviceMaterialOverrides + 0x114b + index * 4));
        }
    }
    void* plate = GameGetVehiclePlateTexture();
    if (plate && plate == pTexture && g_pNetGame)
    {
        CVehiclePool* pool = *(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
        if (pool)
        {
            BYTE* vehicle = (BYTE*)pool->Get(pool->Find((void*)g_hookSavedEsi));
            // Additional observed texture field; the full vehicle layout is open.
            if (vehicle && *(DWORD*)(vehicle + 0x8f))
            {
                ClientSetRenderState(9, 2);
                return g_gameD3DDevice->SetTexture(Stage, *(IDirect3DBaseTexture9**)(vehicle + 0x8f));
            }
            if (*(DWORD*)(g_devicePlateManager + 0x20))
            {
                ClientSetRenderState(9, 2);
                return g_gameD3DDevice->SetTexture(Stage, *(IDirect3DBaseTexture9**)(g_devicePlateManager + 0x20));
            }
        }
    }
    return g_gameD3DDevice->SetTexture(Stage, pTexture);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
    return g_gameD3DDevice->GetTextureStageState(Stage, Type, pValue);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
    return g_gameD3DDevice->SetTextureStageState(Stage, Type, Value);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
    return g_gameD3DDevice->GetSamplerState(Sampler, Type, pValue);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
    return g_gameD3DDevice->SetSamplerState(Sampler, Type, Value);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::ValidateDevice(DWORD* pNumPasses)
{
    return g_gameD3DDevice->ValidateDevice(pNumPasses);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
    return g_gameD3DDevice->SetPaletteEntries(PaletteNumber, pEntries);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
    return g_gameD3DDevice->GetPaletteEntries(PaletteNumber, pEntries);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetCurrentTexturePalette(UINT PaletteNumber)
{
    return g_gameD3DDevice->SetCurrentTexturePalette(PaletteNumber);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetCurrentTexturePalette(UINT* PaletteNumber)
{
    return g_gameD3DDevice->GetCurrentTexturePalette(PaletteNumber);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetScissorRect(CONST RECT* pRect)
{
    return g_gameD3DDevice->SetScissorRect(pRect);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetScissorRect(RECT* pRect)
{
    return g_gameD3DDevice->GetScissorRect(pRect);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetSoftwareVertexProcessing(BOOL bSoftware)
{
    return g_gameD3DDevice->SetSoftwareVertexProcessing(bSoftware);
}

// R5 address to verify
BOOL __stdcall IDirect3DDevice9Hook::GetSoftwareVertexProcessing()
{
    return g_gameD3DDevice->GetSoftwareVertexProcessing();
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetNPatchMode(float nSegments)
{
    return g_gameD3DDevice->SetNPatchMode(nSegments);
}

// R5 address to verify
float __stdcall IDirect3DDevice9Hook::GetNPatchMode()
{
    return g_gameD3DDevice->GetNPatchMode();
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    return g_gameD3DDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::DrawIndexedPrimitive(D3DPRIMITIVETYPE argument0, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex,
                                                             UINT primCount)
{
    return g_gameD3DDevice->DrawIndexedPrimitive(argument0, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    return g_gameD3DDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData,
                                                               D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    return g_gameD3DDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData,
                                                   VertexStreamZeroStride);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer,
                                                        IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
    return g_gameD3DDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
    return g_gameD3DDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
    return g_gameD3DDevice->SetVertexDeclaration(pDecl);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
    return g_gameD3DDevice->GetVertexDeclaration(ppDecl);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetFVF(DWORD FVF)
{
    return g_gameD3DDevice->SetFVF(FVF);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetFVF(DWORD* pFVF)
{
    return g_gameD3DDevice->GetFVF(pFVF);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
    return g_gameD3DDevice->CreateVertexShader(pFunction, ppShader);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShader(IDirect3DVertexShader9* pShader)
{
    return g_gameD3DDevice->SetVertexShader(pShader);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
    return g_gameD3DDevice->GetVertexShader(ppShader);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
    return g_gameD3DDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
    return g_gameD3DDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
    return g_gameD3DDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
    return g_gameD3DDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
    return g_gameD3DDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
    return g_gameD3DDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
    return g_gameD3DDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride)
{
    return g_gameD3DDevice->GetStreamSource(StreamNumber, ppStreamData, OffsetInBytes, pStride);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetStreamSourceFreq(UINT StreamNumber, UINT Divider)
{
    return g_gameD3DDevice->SetStreamSourceFreq(StreamNumber, Divider);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetStreamSourceFreq(UINT StreamNumber, UINT* Divider)
{
    return g_gameD3DDevice->GetStreamSourceFreq(StreamNumber, Divider);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
    return g_gameD3DDevice->SetIndices(pIndexData);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
    return g_gameD3DDevice->GetIndices(ppIndexData);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
    return g_gameD3DDevice->CreatePixelShader(pFunction, ppShader);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShader(IDirect3DPixelShader9* pShader)
{
    return g_gameD3DDevice->SetPixelShader(pShader);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
    return g_gameD3DDevice->GetPixelShader(ppShader);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
    return g_gameD3DDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
    return g_gameD3DDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
    return g_gameD3DDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
    return g_gameD3DDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
    return g_gameD3DDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
    return g_gameD3DDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
    return g_gameD3DDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
    return g_gameD3DDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::DeletePatch(UINT Handle)
{
    return g_gameD3DDevice->DeletePatch(Handle);
}

// R5 address to verify
HRESULT __stdcall IDirect3DDevice9Hook::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
    return g_gameD3DDevice->CreateQuery(Type, ppQuery);
}

// 0x000a4470
void* GameGetVehiclePlateTexture()
{
    if (g_hookCachedBuffer)
    {
        g_devicePlateTexture = *(void**)((BYTE*)g_hookCachedBuffer + 0x34);
        return g_devicePlateTexture;
    }
    return 0;
}

// 0x000c3e20
void ClientSetRenderState(DWORD state, DWORD value)
{
    // GTA SA virtual address: RenderWare engine pointer, outside samp.dll.
    ((void(__cdecl*)(DWORD, DWORD))(*(DWORD**)(*(DWORD*)0xc97b24 + 0x20)))(state, value);
}

// 0x00075730
HRESULT __stdcall IDirect3DDevice9Hook::Present(CONST RECT* source, CONST RECT* dest, HWND window, CONST RGNDATA* dirty)
{
    if (g_renderScreenshotRequested)
        ClientCaptureScreenshot();
    ClientProcessMenuTransition();
    if (g_pGame->IsMenuVisible())
    {
        if (g_pGame->IsMenuVisible() && g_pNetGame && *(DWORD*)0xba6774 && (GetAsyncKeyState(VK_RBUTTON) & 1) && !*(BYTE*)0xba8298)
        {
            // Native leaves both coordinates uninitialized if no marker matches.
            // No deterministic behavior is claimed for that invalid-marker path.
            float x, y;
            for (int i = 0; i < 175; i++)
            {
                BYTE* marker = (BYTE*)(0xba873d + i * 40);
                if (*(WORD*)marker == 0x1203)
                {
                    memcpy(&x, marker - 0x1d, 4);
                    memcpy(&y, marker - 0x19, 4);
                }
            }
            RakNet::BitStream stream;
            stream.Write(x);
            stream.Write(y);
            float z = g_renderGroundHeight(x, y) + 2.0f;
            stream.Write(z);
            (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_renderMapClickRpc, &stream, HIGH_PRIORITY, RELIABLE, 0, false);
        }
        goto present;
    }
    if (g_pNetGame)
    {
        BYTE* vehicles = *(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
        ClientDrawPlayerTags();
        ClientDrawLabels();
        if (g_renderAudioStream)
            g_renderAudioStream->DrawInfo();
        if (g_renderShowVehicleInfo)
        {
            if (g_deviceLabel)
                g_deviceLabel->Begin();
            for (int i = 0; i < 2000; i++)
            {
                if (*(DWORD*)(vehicles + 0x3074 + i * 4) != 1)
                    continue;
                CVehicle* vehicle = *(CVehicle**)(vehicles + 0x1134 + i * 4);
                if (!vehicle)
                    continue;
                CEntity* entity = (CEntity*)vehicle;
                if (entity->GetDistanceToLocalPlayer() <= 20.0f)
                {
                    CMatrix matrix;
                    entity->GetMatrix(&matrix);
                    CVector position = matrix.position;
                    CVector* sync = (CVector*)(vehicles + 0x11ad4 + i * 12);
                    // Native promotes these six values before the following getters.
                    double cx = position.x, cy = position.y, cz = position.z, sx = sync->x, sy = sync->y, sz = sync->z;
                    unsigned seats = vehicle->GetMaxPassengers();
                    double distance = entity->GetDistanceToLocalPlayer();
                    int preloadModel = entity->GetModelIndex();
                    unsigned preloaded = *((BYTE*)g_pGame + preloadModel - 0x122);
                    double health = vehicle->GetHealth();
                    int subtype = vehicle->GetSubtype();
                    int model = entity->GetModelIndex();
                    char text[256];
                    sprintf(text, "[id: %d, type: %d subtype: %d Health: %.1f preloaded: %u]\nDistance: %.2fm\nPassengerSeats: %u\ncPos: %.3f,%.3f,%.3f\nsPos: %.3f,%.3f,%.3f", i,
                            model, subtype, health, preloaded, distance, seats, cx, cy, cz, sx, sy, sz);
                    g_deviceLabel->Draw(&position, text, 0xff358bd4, TRUE, FALSE);
                }
            }
            if (g_deviceLabel)
                g_deviceLabel->End();
        }
        if (g_pNetGame)
        {
            if (*(DWORD*)g_deviceScoreboard)
            {
                g_pGame->EnableHUD(FALSE);
                g_deviceScoreboard->Draw();
                goto present;
            }
            SHORT key = GetAsyncKeyState(VK_F5);
            if (key && *(DWORD*)((BYTE*)g_pNetGame + 0x3cd) == 5)
            {
                g_pGame->EnableHUD(FALSE);
                if (g_renderNetStats)
                    g_renderNetStats->Draw();
                goto present;
            }
            if (g_pNetGame)
            {
                key = GetAsyncKeyState(VK_F10);
                if (key)
                {
                    g_pGame->EnableHUD(FALSE);
                    if (g_renderServerNetStats)
                        g_renderServerNetStats->Draw();
                    goto present;
                }
                if (g_pNetGame && GetAsyncKeyState(VK_F1) && g_renderHelp)
                    g_renderHelp->Show();
            }
        }
    }
    g_pGame->EnableHUD(g_pChat->m_nMode != 0);
    if (g_pGame && g_pGame->GetPlayerPed() && (int)g_pGame->GetPlayerPed()->GetDrunkLevel() > 5000)
        g_pGame->EnableHUD(FALSE);
    if (g_renderChatBubble)
        g_renderChatBubble->Draw();
    if (g_pNetGame && *(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x1c))
        (*(CLabelPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x1c))->Draw();
    if (g_pChat)
        g_pChat->Draw();
    if (g_pInput)
        g_pInput->EnableCursor();
    if (g_deviceDeathWindow)
        ((CDeathWindow*)g_deviceDeathWindow)->Draw();
    if (g_pNetGame)
    {
        if (g_renderDialog)
            g_renderDialog->Draw();
        if (g_pNetGame)
        {
            if (g_renderPanel)
                g_renderPanel->Draw();
            if (g_pNetGame && g_renderDownloadWindow)
                g_renderDownloadWindow->Draw();
        }
    }
    if (g_renderFirstDialog)
        g_renderFirstDialog->OnRender(10.0f);
    if (g_renderSecondDialog)
        g_renderSecondDialog->OnRender(10.0f);
    if (g_deviceObjectEdit)
        ((CObjectEdit*)g_deviceObjectEdit)->Render();
    if (g_renderObjectSelection)
        g_renderObjectSelection->Draw();
    if (g_renderTextDrawSelection)
        g_renderTextDrawSelection->Process();
present:
    if (g_clientSettings.debug)
        GameDebugDraw();
    GameDeviceIntegrityCycle();
    HRESULT result = g_gameD3DDevice->Present(source, dest, window, dirty);
    GameDeviceIntegrityCycle();
    return result;
}
