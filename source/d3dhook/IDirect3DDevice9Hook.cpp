#include "d3dhook/IDirect3DDevice9Hook.h"
#include "d3dhook/texture_binding.h"
#include "game/game.h"
#include "game/hooks_independent.h"
#include "net/vehiclepool.h"
#include <string.h>
// samp.dll RVA: 0x000c5e60
IDirect3DDevice9Hook::IDirect3DDevice9Hook() {}
// samp.dll RVA: 0x0017ec00
D3DMATRIX g_deviceViewMatrix;
// samp.dll RVA: 0x0017ec40
D3DMATRIX g_deviceProjectionMatrix;
// samp.dll RVA: 0x0017ec80
D3DMATRIX g_deviceWorldMatrix;
// samp.dll RVA: 0x00073f60
HRESULT __stdcall IDirect3DDevice9Hook::QueryInterface(REFIID riid, void** ppvObj)
{
    return g_gameD3DDevice->QueryInterface(riid, ppvObj);
}
// samp.dll RVA: 0x00073f70
ULONG __stdcall IDirect3DDevice9Hook::AddRef()
{
    return g_gameD3DDevice->AddRef();
}
// samp.dll RVA: 0x00073f80
ULONG __stdcall IDirect3DDevice9Hook::Release()
{
    return g_gameD3DDevice->Release();
}
// samp.dll RVA: 0x00073f90
HRESULT __stdcall IDirect3DDevice9Hook::TestCooperativeLevel()
{
    return g_gameD3DDevice->TestCooperativeLevel();
}
// samp.dll RVA: 0x00073fa0
UINT __stdcall IDirect3DDevice9Hook::GetAvailableTextureMem()
{
    return g_gameD3DDevice->GetAvailableTextureMem();
}
// samp.dll RVA: 0x00073fb0
HRESULT __stdcall IDirect3DDevice9Hook::EvictManagedResources()
{
    return g_gameD3DDevice->EvictManagedResources();
}
// samp.dll RVA: 0x00073fc0
HRESULT __stdcall IDirect3DDevice9Hook::GetDirect3D(IDirect3D9** ppD3D9)
{
    return g_gameD3DDevice->GetDirect3D(ppD3D9);
}
// samp.dll RVA: 0x00073fd0
HRESULT __stdcall IDirect3DDevice9Hook::GetDeviceCaps(D3DCAPS9* pCaps)
{
    return g_gameD3DDevice->GetDeviceCaps(pCaps);
}
// samp.dll RVA: 0x00073fe0
HRESULT __stdcall IDirect3DDevice9Hook::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
    return g_gameD3DDevice->GetDisplayMode(iSwapChain, pMode);
}
// samp.dll RVA: 0x00073ff0
HRESULT __stdcall IDirect3DDevice9Hook::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
    return g_gameD3DDevice->GetCreationParameters(pParameters);
}
// samp.dll RVA: 0x00074000
HRESULT __stdcall IDirect3DDevice9Hook::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
    return g_gameD3DDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}
// samp.dll RVA: 0x00074010
void __stdcall IDirect3DDevice9Hook::SetCursorPosition(int X, int Y, DWORD Flags)
{
    g_gameD3DDevice->SetCursorPosition(X, Y, Flags);
}
// samp.dll RVA: 0x00074020
BOOL __stdcall IDirect3DDevice9Hook::ShowCursor(BOOL bShow)
{
    return g_gameD3DDevice->ShowCursor(bShow);
}
// samp.dll RVA: 0x00074030
HRESULT __stdcall IDirect3DDevice9Hook::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
    return g_gameD3DDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}
// samp.dll RVA: 0x00074040
HRESULT __stdcall IDirect3DDevice9Hook::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
    return g_gameD3DDevice->GetSwapChain(iSwapChain, pSwapChain);
}
// samp.dll RVA: 0x00074050
UINT __stdcall IDirect3DDevice9Hook::GetNumberOfSwapChains()
{
    return g_gameD3DDevice->GetNumberOfSwapChains();
}
// samp.dll RVA: 0x00074060
HRESULT __stdcall IDirect3DDevice9Hook::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    ClientDeviceObjectsLost();
    HRESULT result = g_gameD3DDevice->Reset(pPresentationParameters);
    if (SUCCEEDED(result))
        ClientDeviceObjectsReset();
    return result;
}
// samp.dll RVA: 0x00074090
HRESULT __stdcall IDirect3DDevice9Hook::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
    return g_gameD3DDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}
// samp.dll RVA: 0x000740a0
HRESULT __stdcall IDirect3DDevice9Hook::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
    return g_gameD3DDevice->GetRasterStatus(iSwapChain, pRasterStatus);
}
// samp.dll RVA: 0x000740b0
HRESULT __stdcall IDirect3DDevice9Hook::SetDialogBoxMode(BOOL bEnableDialogs)
{
    return g_gameD3DDevice->SetDialogBoxMode(bEnableDialogs);
}
// samp.dll RVA: 0x000740c0
void __stdcall IDirect3DDevice9Hook::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
    g_gameD3DDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
}
// samp.dll RVA: 0x000740d0
void __stdcall IDirect3DDevice9Hook::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
    g_gameD3DDevice->GetGammaRamp(iSwapChain, pRamp);
}
// samp.dll RVA: 0x000740e0
HRESULT __stdcall IDirect3DDevice9Hook::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture,
                                                      HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}
// samp.dll RVA: 0x000740f0
HRESULT __stdcall IDirect3DDevice9Hook::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
                                                            IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}
// samp.dll RVA: 0x00074100
HRESULT __stdcall IDirect3DDevice9Hook::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture,
                                                          HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}
// samp.dll RVA: 0x00074110
HRESULT __stdcall IDirect3DDevice9Hook::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}
// samp.dll RVA: 0x00074120
HRESULT __stdcall IDirect3DDevice9Hook::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}
// samp.dll RVA: 0x00074130
HRESULT __stdcall IDirect3DDevice9Hook::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
                                                           IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}
// samp.dll RVA: 0x00074140
HRESULT __stdcall IDirect3DDevice9Hook::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,
                                                                  BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateDepthStencilSurface(Width, Height, Format, D3DMULTISAMPLE_NONE, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}
// samp.dll RVA: 0x00074160
HRESULT __stdcall IDirect3DDevice9Hook::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
    return g_gameD3DDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}
// samp.dll RVA: 0x00074170
HRESULT __stdcall IDirect3DDevice9Hook::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
    return g_gameD3DDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
}
// samp.dll RVA: 0x00074180
HRESULT __stdcall IDirect3DDevice9Hook::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
    return g_gameD3DDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
}
// samp.dll RVA: 0x000741a0
HRESULT __stdcall IDirect3DDevice9Hook::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
    return g_gameD3DDevice->GetFrontBufferData(iSwapChain, pDestSurface);
}
// samp.dll RVA: 0x000741c0
HRESULT __stdcall IDirect3DDevice9Hook::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect,
                                                    D3DTEXTUREFILTERTYPE Filter)
{
    return g_gameD3DDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}
// samp.dll RVA: 0x000741e0
HRESULT __stdcall IDirect3DDevice9Hook::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
    return g_gameD3DDevice->ColorFill(pSurface, pRect, color);
}
// samp.dll RVA: 0x00074200
HRESULT __stdcall IDirect3DDevice9Hook::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
    return g_gameD3DDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}
// samp.dll RVA: 0x00074220
HRESULT __stdcall IDirect3DDevice9Hook::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
    return g_gameD3DDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}
// samp.dll RVA: 0x00074240
HRESULT __stdcall IDirect3DDevice9Hook::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
    return g_gameD3DDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}
// samp.dll RVA: 0x00074260
HRESULT __stdcall IDirect3DDevice9Hook::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
    return g_gameD3DDevice->SetDepthStencilSurface(pNewZStencil);
}
// samp.dll RVA: 0x00074280
HRESULT __stdcall IDirect3DDevice9Hook::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
    return g_gameD3DDevice->GetDepthStencilSurface(ppZStencilSurface);
}
// samp.dll RVA: 0x000742a0
HRESULT __stdcall IDirect3DDevice9Hook::BeginScene()
{
    return g_gameD3DDevice->BeginScene();
}
// samp.dll RVA: 0x000742c0
HRESULT __stdcall IDirect3DDevice9Hook::EndScene()
{
    GameDeviceIntegrityCycle();
    HRESULT result = g_gameD3DDevice->EndScene();
    GameDeviceIntegrityCycle();
    return result;
}
// samp.dll RVA: 0x000742f0
HRESULT __stdcall IDirect3DDevice9Hook::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
    return g_gameD3DDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}
// samp.dll RVA: 0x00074310
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
// samp.dll RVA: 0x00074360
HRESULT __stdcall IDirect3DDevice9Hook::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* mat)
{
    return g_gameD3DDevice->GetTransform(State, mat);
}
// samp.dll RVA: 0x00074380
HRESULT __stdcall IDirect3DDevice9Hook::MultiplyTransform(D3DTRANSFORMSTATETYPE argument0, CONST D3DMATRIX* argument1)
{
    return g_gameD3DDevice->MultiplyTransform(argument0, argument1);
}
// samp.dll RVA: 0x000743a0
HRESULT __stdcall IDirect3DDevice9Hook::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
    return g_gameD3DDevice->SetViewport(pViewport);
}
// samp.dll RVA: 0x000743c0
HRESULT __stdcall IDirect3DDevice9Hook::GetViewport(D3DVIEWPORT9* pViewport)
{
    return g_gameD3DDevice->GetViewport(pViewport);
}
// samp.dll RVA: 0x000743e0
HRESULT __stdcall IDirect3DDevice9Hook::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
    return g_gameD3DDevice->SetMaterial(pMaterial);
}
// samp.dll RVA: 0x00074400
HRESULT __stdcall IDirect3DDevice9Hook::GetMaterial(D3DMATERIAL9* pMaterial)
{
    return g_gameD3DDevice->GetMaterial(pMaterial);
}
// samp.dll RVA: 0x00074420
HRESULT __stdcall IDirect3DDevice9Hook::SetLight(DWORD Index, CONST D3DLIGHT9* argument1)
{
    return g_gameD3DDevice->SetLight(Index, argument1);
}
// samp.dll RVA: 0x00074440
HRESULT __stdcall IDirect3DDevice9Hook::GetLight(DWORD Index, D3DLIGHT9* argument1)
{
    return g_gameD3DDevice->GetLight(Index, argument1);
}
// samp.dll RVA: 0x00074460
HRESULT __stdcall IDirect3DDevice9Hook::LightEnable(DWORD Index, BOOL Enable)
{
    return g_gameD3DDevice->LightEnable(Index, Enable);
}
// samp.dll RVA: 0x00074480
HRESULT __stdcall IDirect3DDevice9Hook::GetLightEnable(DWORD Index, BOOL* pEnable)
{
    return g_gameD3DDevice->GetLightEnable(Index, pEnable);
}
// samp.dll RVA: 0x000744a0
HRESULT __stdcall IDirect3DDevice9Hook::SetClipPlane(DWORD Index, CONST float* pPlane)
{
    return g_gameD3DDevice->SetClipPlane(Index, pPlane);
}
// samp.dll RVA: 0x000744c0
HRESULT __stdcall IDirect3DDevice9Hook::GetClipPlane(DWORD Index, float* pPlane)
{
    return g_gameD3DDevice->GetClipPlane(Index, pPlane);
}
// samp.dll RVA: 0x000744e0
HRESULT __stdcall IDirect3DDevice9Hook::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
    return g_gameD3DDevice->SetRenderState(State, Value);
}
// samp.dll RVA: 0x00074500
HRESULT __stdcall IDirect3DDevice9Hook::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
    return g_gameD3DDevice->GetRenderState(State, pValue);
}
// samp.dll RVA: 0x00074520
HRESULT __stdcall IDirect3DDevice9Hook::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
    return g_gameD3DDevice->CreateStateBlock(Type, ppSB);
}
// samp.dll RVA: 0x00074540
HRESULT __stdcall IDirect3DDevice9Hook::BeginStateBlock()
{
    return g_gameD3DDevice->BeginStateBlock();
}
// samp.dll RVA: 0x00074560
HRESULT __stdcall IDirect3DDevice9Hook::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
    return g_gameD3DDevice->EndStateBlock(ppSB);
}
// samp.dll RVA: 0x00074580
HRESULT __stdcall IDirect3DDevice9Hook::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
    return g_gameD3DDevice->SetClipStatus(pClipStatus);
}
// samp.dll RVA: 0x000745a0
HRESULT __stdcall IDirect3DDevice9Hook::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
    return g_gameD3DDevice->GetClipStatus(pClipStatus);
}
// samp.dll RVA: 0x000745c0
HRESULT __stdcall IDirect3DDevice9Hook::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
    return g_gameD3DDevice->GetTexture(Stage, ppTexture);
}
// samp.dll RVA: 0x000745e0
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
// samp.dll RVA: 0x00074720
HRESULT __stdcall IDirect3DDevice9Hook::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
    return g_gameD3DDevice->GetTextureStageState(Stage, Type, pValue);
}
// samp.dll RVA: 0x00074740
HRESULT __stdcall IDirect3DDevice9Hook::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
    return g_gameD3DDevice->SetTextureStageState(Stage, Type, Value);
}
// samp.dll RVA: 0x00074760
HRESULT __stdcall IDirect3DDevice9Hook::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
    return g_gameD3DDevice->GetSamplerState(Sampler, Type, pValue);
}
// samp.dll RVA: 0x00074780
HRESULT __stdcall IDirect3DDevice9Hook::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
    return g_gameD3DDevice->SetSamplerState(Sampler, Type, Value);
}
// samp.dll RVA: 0x000747a0
HRESULT __stdcall IDirect3DDevice9Hook::ValidateDevice(DWORD* pNumPasses)
{
    return g_gameD3DDevice->ValidateDevice(pNumPasses);
}
// samp.dll RVA: 0x000747c0
HRESULT __stdcall IDirect3DDevice9Hook::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
    return g_gameD3DDevice->SetPaletteEntries(PaletteNumber, pEntries);
}
// samp.dll RVA: 0x000747e0
HRESULT __stdcall IDirect3DDevice9Hook::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
    return g_gameD3DDevice->GetPaletteEntries(PaletteNumber, pEntries);
}
// samp.dll RVA: 0x00074800
HRESULT __stdcall IDirect3DDevice9Hook::SetCurrentTexturePalette(UINT PaletteNumber)
{
    return g_gameD3DDevice->SetCurrentTexturePalette(PaletteNumber);
}
// samp.dll RVA: 0x00074820
HRESULT __stdcall IDirect3DDevice9Hook::GetCurrentTexturePalette(UINT* PaletteNumber)
{
    return g_gameD3DDevice->GetCurrentTexturePalette(PaletteNumber);
}
// samp.dll RVA: 0x00074840
HRESULT __stdcall IDirect3DDevice9Hook::SetScissorRect(CONST RECT* pRect)
{
    return g_gameD3DDevice->SetScissorRect(pRect);
}
// samp.dll RVA: 0x00074860
HRESULT __stdcall IDirect3DDevice9Hook::GetScissorRect(RECT* pRect)
{
    return g_gameD3DDevice->GetScissorRect(pRect);
}
// samp.dll RVA: 0x00074880
HRESULT __stdcall IDirect3DDevice9Hook::SetSoftwareVertexProcessing(BOOL bSoftware)
{
    return g_gameD3DDevice->SetSoftwareVertexProcessing(bSoftware);
}
// samp.dll RVA: 0x000748a0
BOOL __stdcall IDirect3DDevice9Hook::GetSoftwareVertexProcessing()
{
    return g_gameD3DDevice->GetSoftwareVertexProcessing();
}
// samp.dll RVA: 0x000748c0
HRESULT __stdcall IDirect3DDevice9Hook::SetNPatchMode(float nSegments)
{
    return g_gameD3DDevice->SetNPatchMode(nSegments);
}
// samp.dll RVA: 0x000748e0
float __stdcall IDirect3DDevice9Hook::GetNPatchMode()
{
    return g_gameD3DDevice->GetNPatchMode();
}
// samp.dll RVA: 0x00074900
HRESULT __stdcall IDirect3DDevice9Hook::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    return g_gameD3DDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}
// samp.dll RVA: 0x00074920
HRESULT __stdcall IDirect3DDevice9Hook::DrawIndexedPrimitive(D3DPRIMITIVETYPE argument0, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex,
                                                             UINT primCount)
{
    return g_gameD3DDevice->DrawIndexedPrimitive(argument0, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}
// samp.dll RVA: 0x00074940
HRESULT __stdcall IDirect3DDevice9Hook::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    return g_gameD3DDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}
// samp.dll RVA: 0x00074960
HRESULT __stdcall IDirect3DDevice9Hook::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData,
                                                               D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    return g_gameD3DDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData,
                                                   VertexStreamZeroStride);
}
// samp.dll RVA: 0x00074980
HRESULT __stdcall IDirect3DDevice9Hook::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer,
                                                        IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
    return g_gameD3DDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}
// samp.dll RVA: 0x000749a0
HRESULT __stdcall IDirect3DDevice9Hook::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
    return g_gameD3DDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
}
// samp.dll RVA: 0x000749c0
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
    return g_gameD3DDevice->SetVertexDeclaration(pDecl);
}
// samp.dll RVA: 0x000749e0
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
    return g_gameD3DDevice->GetVertexDeclaration(ppDecl);
}
// samp.dll RVA: 0x00074a00
HRESULT __stdcall IDirect3DDevice9Hook::SetFVF(DWORD FVF)
{
    return g_gameD3DDevice->SetFVF(FVF);
}
// samp.dll RVA: 0x00074a20
HRESULT __stdcall IDirect3DDevice9Hook::GetFVF(DWORD* pFVF)
{
    return g_gameD3DDevice->GetFVF(pFVF);
}
// samp.dll RVA: 0x00074a40
HRESULT __stdcall IDirect3DDevice9Hook::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
    return g_gameD3DDevice->CreateVertexShader(pFunction, ppShader);
}
// samp.dll RVA: 0x00074a60
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShader(IDirect3DVertexShader9* pShader)
{
    return g_gameD3DDevice->SetVertexShader(pShader);
}
// samp.dll RVA: 0x00074a80
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
    return g_gameD3DDevice->GetVertexShader(ppShader);
}
// samp.dll RVA: 0x00074aa0
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
    return g_gameD3DDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}
// samp.dll RVA: 0x00074ac0
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
    return g_gameD3DDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}
// samp.dll RVA: 0x00074ae0
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
    return g_gameD3DDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}
// samp.dll RVA: 0x00074b00
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
    return g_gameD3DDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}
// samp.dll RVA: 0x00074b20
HRESULT __stdcall IDirect3DDevice9Hook::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
    return g_gameD3DDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}
// samp.dll RVA: 0x00074b40
HRESULT __stdcall IDirect3DDevice9Hook::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
    return g_gameD3DDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}
// samp.dll RVA: 0x00074b60
HRESULT __stdcall IDirect3DDevice9Hook::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
    return g_gameD3DDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}
// samp.dll RVA: 0x00074b80
HRESULT __stdcall IDirect3DDevice9Hook::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride)
{
    return g_gameD3DDevice->GetStreamSource(StreamNumber, ppStreamData, OffsetInBytes, pStride);
}
// samp.dll RVA: 0x00074ba0
HRESULT __stdcall IDirect3DDevice9Hook::SetStreamSourceFreq(UINT StreamNumber, UINT Divider)
{
    return g_gameD3DDevice->SetStreamSourceFreq(StreamNumber, Divider);
}
// samp.dll RVA: 0x00074bc0
HRESULT __stdcall IDirect3DDevice9Hook::GetStreamSourceFreq(UINT StreamNumber, UINT* Divider)
{
    return g_gameD3DDevice->GetStreamSourceFreq(StreamNumber, Divider);
}
// samp.dll RVA: 0x00074be0
HRESULT __stdcall IDirect3DDevice9Hook::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
    return g_gameD3DDevice->SetIndices(pIndexData);
}
// samp.dll RVA: 0x00074c00
HRESULT __stdcall IDirect3DDevice9Hook::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
    return g_gameD3DDevice->GetIndices(ppIndexData);
}
// samp.dll RVA: 0x00074c20
HRESULT __stdcall IDirect3DDevice9Hook::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
    return g_gameD3DDevice->CreatePixelShader(pFunction, ppShader);
}
// samp.dll RVA: 0x00074c40
HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShader(IDirect3DPixelShader9* pShader)
{
    return g_gameD3DDevice->SetPixelShader(pShader);
}
// samp.dll RVA: 0x00074c60
HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
    return g_gameD3DDevice->GetPixelShader(ppShader);
}
// samp.dll RVA: 0x00074c80
HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
    return g_gameD3DDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}
// samp.dll RVA: 0x00074ca0
HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
    return g_gameD3DDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}
// samp.dll RVA: 0x00074cc0
HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
    return g_gameD3DDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}
// samp.dll RVA: 0x00074ce0
HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
    return g_gameD3DDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}
// samp.dll RVA: 0x00074d00
HRESULT __stdcall IDirect3DDevice9Hook::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
    return g_gameD3DDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}
// samp.dll RVA: 0x00074d20
HRESULT __stdcall IDirect3DDevice9Hook::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
    return g_gameD3DDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}
// samp.dll RVA: 0x00074d40
HRESULT __stdcall IDirect3DDevice9Hook::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
    return g_gameD3DDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}
// samp.dll RVA: 0x00074d60
HRESULT __stdcall IDirect3DDevice9Hook::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
    return g_gameD3DDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}
// samp.dll RVA: 0x00074d80
HRESULT __stdcall IDirect3DDevice9Hook::DeletePatch(UINT Handle)
{
    return g_gameD3DDevice->DeletePatch(Handle);
}
// samp.dll RVA: 0x00074da0
HRESULT __stdcall IDirect3DDevice9Hook::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
    return g_gameD3DDevice->CreateQuery(Type, ppQuery);
}

// samp.dll RVA: 0x001e0490
BOOL g_deviceMaterialOverrideEnabled;
// samp.dll RVA: 0x001e0494
BYTE* g_deviceMaterialOverrides;
// samp.dll RVA: 0x0017ebf0
DWORD g_deviceMaterialIndex;
// samp.dll RVA: 0x0018f564
void* g_devicePlateTexture;
// samp.dll RVA: 0x002aca30
BYTE* g_devicePlateManager;
// samp.dll RVA: 0x000a41d0
void* GameGetVehiclePlateTexture()
{
    if (g_hookCachedBuffer)
    {
        g_devicePlateTexture = *(void**)((BYTE*)g_hookCachedBuffer + 0x34);
        return *(void**)((BYTE*)g_hookCachedBuffer + 0x34);
    }
    return 0;
}
// samp.dll RVA: 0x000c54f0
void ClientSetRenderState(DWORD state, DWORD value)
{
    // GTA SA virtual address: RenderWare engine pointer, outside samp.dll.
    ((void(__cdecl*)(DWORD, DWORD))(*(DWORD**)(*(DWORD*)0xc97b24 + 0x20)))(state, value);
}

#include "BitStream.h"
#include "RakClientInterface.h"
#include "chatwindow.h"
#include "cmdwindow.h"
#include "game/device_integrity.h"
#include "game/playerped.h"
#include "game/vehicle.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <stdio.h>
// samp.dll RVA: 0x002aca1c
CAudioStream* g_renderAudioStream;
// samp.dll RVA: 0x002aca08
CChatBubble* g_renderChatBubble;
// samp.dll RVA: 0x002ac9e0
CDialog* g_renderDialog;
// samp.dll RVA: 0x002ac9e4
CRenderPanel* g_renderPanel;
// samp.dll RVA: 0x002ac9e8
CDownloadWindow* g_renderDownloadWindow;
// samp.dll RVA: 0x002ac9f4
CObjectSelection* g_renderObjectSelection;
// samp.dll RVA: 0x002ac9f8
CTextDrawSelection* g_renderTextDrawSelection;
// samp.dll RVA: 0x002ac9fc
CNetStats* g_renderNetStats;
// samp.dll RVA: 0x002aca00
CSrvNetStats* g_renderServerNetStats;
// samp.dll RVA: 0x002aca04
CHelpDialog* g_renderHelp;
// samp.dll RVA: 0x002acab4
CDXUTDialog* g_renderFirstDialog;
// samp.dll RVA: 0x002acabc
CDXUTDialog* g_renderSecondDialog;
// samp.dll RVA: 0x0016be7c
DWORD g_renderScreenshotRequested;
// samp.dll RVA: 0x002aca48
BYTE g_renderShowVehicleInfo;
// samp.dll RVA: 0x00140bdc
GroundHeightFunction g_renderGroundHeight = (GroundHeightFunction)0x569660; // GTA SA virtual address.
// samp.dll RVA: 0x0011bbd4
int g_renderMapClickRpc = 119;
// samp.dll RVA: 0x000751b0
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
