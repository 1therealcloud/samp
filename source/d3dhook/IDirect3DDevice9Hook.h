// IDirect3DDevice9Hook
#ifndef IDIRECT3DDEVICE9HOOK_H
#define IDIRECT3DDEVICE9HOOK_H

#include <d3d9.h>

#include <static_assert.h>

// Four-byte native proxy; the real device is a shared global.
class IDirect3DDevice9Hook : public IDirect3DDevice9
{
public:
    // R5 address to verify
    IDirect3DDevice9Hook();
    // R5 address to verify
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj);
    // R5 address to verify
    ULONG __stdcall AddRef();
    // R5 address to verify
    ULONG __stdcall Release();
    // R5 address to verify
    HRESULT __stdcall TestCooperativeLevel();
    // R5 address to verify
    UINT __stdcall GetAvailableTextureMem();
    // R5 address to verify
    HRESULT __stdcall EvictManagedResources();
    // R5 address to verify
    HRESULT __stdcall GetDirect3D(IDirect3D9** ppD3D9);
    // R5 address to verify
    HRESULT __stdcall GetDeviceCaps(D3DCAPS9* pCaps);
    // R5 address to verify
    HRESULT __stdcall GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode);
    // R5 address to verify
    HRESULT __stdcall GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters);
    // R5 address to verify
    HRESULT __stdcall SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
    // R5 address to verify
    void __stdcall SetCursorPosition(int X, int Y, DWORD Flags);
    // R5 address to verify
    BOOL __stdcall ShowCursor(BOOL bShow);
    // R5 address to verify
    HRESULT __stdcall CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
    // R5 address to verify
    HRESULT __stdcall GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
    // R5 address to verify
    UINT __stdcall GetNumberOfSwapChains();
    // R5 address to verify
    HRESULT __stdcall Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
    // Implemented in d3dhook/present.cpp; downstream rendering dependencies remain.
    // 0x00075730
    HRESULT __stdcall Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
    // R5 address to verify
    HRESULT __stdcall GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    // R5 address to verify
    HRESULT __stdcall GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus);
    // R5 address to verify
    HRESULT __stdcall SetDialogBoxMode(BOOL bEnableDialogs);
    // R5 address to verify
    void __stdcall SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp);
    // R5 address to verify
    void __stdcall GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp);
    // R5 address to verify
    HRESULT __stdcall CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle);
    // R5 address to verify
    HRESULT __stdcall CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture,
                                          HANDLE* pSharedHandle);
    // R5 address to verify
    HRESULT __stdcall CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle);
    // R5 address to verify
    HRESULT __stdcall CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
    // R5 address to verify
    HRESULT __stdcall CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle);
    // R5 address to verify
    HRESULT __stdcall CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
                                         IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    // R5 address to verify
    HRESULT __stdcall CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard,
                                                IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    // R5 address to verify
    HRESULT __stdcall UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint);
    // R5 address to verify
    HRESULT __stdcall UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
    // R5 address to verify
    HRESULT __stdcall GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface);
    // R5 address to verify
    HRESULT __stdcall GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface);
    // R5 address to verify
    HRESULT __stdcall StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter);
    // R5 address to verify
    HRESULT __stdcall ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color);
    // R5 address to verify
    HRESULT __stdcall CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    // R5 address to verify
    HRESULT __stdcall SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
    // R5 address to verify
    HRESULT __stdcall GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);
    // R5 address to verify
    HRESULT __stdcall SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil);
    // R5 address to verify
    HRESULT __stdcall GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
    // R5 address to verify
    HRESULT __stdcall BeginScene();
    // R5 address to verify
    HRESULT __stdcall EndScene();
    // R5 address to verify
    HRESULT __stdcall Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    // R5 address to verify
    HRESULT __stdcall SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* mat);
    // R5 address to verify
    HRESULT __stdcall GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* mat);
    // R5 address to verify
    HRESULT __stdcall MultiplyTransform(D3DTRANSFORMSTATETYPE argument0, CONST D3DMATRIX* argument1);
    // R5 address to verify
    HRESULT __stdcall SetViewport(CONST D3DVIEWPORT9* pViewport);
    // R5 address to verify
    HRESULT __stdcall GetViewport(D3DVIEWPORT9* pViewport);
    // R5 address to verify
    HRESULT __stdcall SetMaterial(CONST D3DMATERIAL9* pMaterial);
    // R5 address to verify
    HRESULT __stdcall GetMaterial(D3DMATERIAL9* pMaterial);
    // R5 address to verify
    HRESULT __stdcall SetLight(DWORD Index, CONST D3DLIGHT9* argument1);
    // R5 address to verify
    HRESULT __stdcall GetLight(DWORD Index, D3DLIGHT9* argument1);
    // R5 address to verify
    HRESULT __stdcall LightEnable(DWORD Index, BOOL Enable);
    // R5 address to verify
    HRESULT __stdcall GetLightEnable(DWORD Index, BOOL* pEnable);
    // R5 address to verify
    HRESULT __stdcall SetClipPlane(DWORD Index, CONST float* pPlane);
    // R5 address to verify
    HRESULT __stdcall GetClipPlane(DWORD Index, float* pPlane);
    // R5 address to verify
    HRESULT __stdcall SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
    // R5 address to verify
    HRESULT __stdcall GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue);
    // R5 address to verify
    HRESULT __stdcall CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB);
    // R5 address to verify
    HRESULT __stdcall BeginStateBlock();
    // R5 address to verify
    HRESULT __stdcall EndStateBlock(IDirect3DStateBlock9** ppSB);
    // R5 address to verify
    HRESULT __stdcall SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus);
    // R5 address to verify
    HRESULT __stdcall GetClipStatus(D3DCLIPSTATUS9* pClipStatus);
    // R5 address to verify
    HRESULT __stdcall GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture);
    // R5 address to verify
    HRESULT __stdcall SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture);
    // R5 address to verify
    HRESULT __stdcall GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
    // R5 address to verify
    HRESULT __stdcall SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    // R5 address to verify
    HRESULT __stdcall GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue);
    // R5 address to verify
    HRESULT __stdcall SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    // R5 address to verify
    HRESULT __stdcall ValidateDevice(DWORD* pNumPasses);
    // R5 address to verify
    HRESULT __stdcall SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries);
    // R5 address to verify
    HRESULT __stdcall GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries);
    // R5 address to verify
    HRESULT __stdcall SetCurrentTexturePalette(UINT PaletteNumber);
    // R5 address to verify
    HRESULT __stdcall GetCurrentTexturePalette(UINT* PaletteNumber);
    // R5 address to verify
    HRESULT __stdcall SetScissorRect(CONST RECT* pRect);
    // R5 address to verify
    HRESULT __stdcall GetScissorRect(RECT* pRect);
    // R5 address to verify
    HRESULT __stdcall SetSoftwareVertexProcessing(BOOL bSoftware);
    // R5 address to verify
    BOOL __stdcall GetSoftwareVertexProcessing();
    // R5 address to verify
    HRESULT __stdcall SetNPatchMode(float nSegments);
    // R5 address to verify
    float __stdcall GetNPatchMode();
    // R5 address to verify
    HRESULT __stdcall DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    // R5 address to verify
    HRESULT __stdcall DrawIndexedPrimitive(D3DPRIMITIVETYPE argument0, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
    // R5 address to verify
    HRESULT __stdcall DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    // R5 address to verify
    HRESULT __stdcall DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData,
                                             D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    // R5 address to verify
    HRESULT __stdcall ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl,
                                      DWORD Flags);
    // R5 address to verify
    HRESULT __stdcall CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
    // R5 address to verify
    HRESULT __stdcall SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl);
    // R5 address to verify
    HRESULT __stdcall GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl);
    // R5 address to verify
    HRESULT __stdcall SetFVF(DWORD FVF);
    // R5 address to verify
    HRESULT __stdcall GetFVF(DWORD* pFVF);
    // R5 address to verify
    HRESULT __stdcall CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader);
    // R5 address to verify
    HRESULT __stdcall SetVertexShader(IDirect3DVertexShader9* pShader);
    // R5 address to verify
    HRESULT __stdcall GetVertexShader(IDirect3DVertexShader9** ppShader);
    // R5 address to verify
    HRESULT __stdcall SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
    // R5 address to verify
    HRESULT __stdcall GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    // R5 address to verify
    HRESULT __stdcall SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount);
    // R5 address to verify
    HRESULT __stdcall GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    // R5 address to verify
    HRESULT __stdcall SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount);
    // R5 address to verify
    HRESULT __stdcall GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    // R5 address to verify
    HRESULT __stdcall SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride);
    // R5 address to verify
    HRESULT __stdcall GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride);
    // R5 address to verify
    HRESULT __stdcall SetStreamSourceFreq(UINT StreamNumber, UINT Divider);
    // R5 address to verify
    HRESULT __stdcall GetStreamSourceFreq(UINT StreamNumber, UINT* Divider);
    // R5 address to verify
    HRESULT __stdcall SetIndices(IDirect3DIndexBuffer9* pIndexData);
    // R5 address to verify
    HRESULT __stdcall GetIndices(IDirect3DIndexBuffer9** ppIndexData);
    // R5 address to verify
    HRESULT __stdcall CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader);
    // R5 address to verify
    HRESULT __stdcall SetPixelShader(IDirect3DPixelShader9* pShader);
    // R5 address to verify
    HRESULT __stdcall GetPixelShader(IDirect3DPixelShader9** ppShader);
    // R5 address to verify
    HRESULT __stdcall SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
    // R5 address to verify
    HRESULT __stdcall GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    // R5 address to verify
    HRESULT __stdcall SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount);
    // R5 address to verify
    HRESULT __stdcall GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    // R5 address to verify
    HRESULT __stdcall SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount);
    // R5 address to verify
    HRESULT __stdcall GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    // R5 address to verify
    HRESULT __stdcall DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo);
    // R5 address to verify
    HRESULT __stdcall DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo);
    // R5 address to verify
    HRESULT __stdcall DeletePatch(UINT Handle);
    // R5 address to verify
    HRESULT __stdcall CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);
};
STATIC_ASSERT(sizeof(IDirect3DDevice9Hook) == 4);

// 0x00140d70
extern D3DMATRIX g_deviceViewMatrix;
// 0x00140db0
extern D3DMATRIX g_deviceProjectionMatrix;
// R5 address to verify
extern D3DMATRIX g_deviceWorldMatrix;
// Implemented in gui/gui.cpp; downstream UI/pool dependencies remain.
// 0x000c3f50
void ClientDeviceObjectsLost();
// Implemented in gui/gui.cpp; downstream UI dependencies remain.
// 0x000c41b0
void ClientDeviceObjectsReset();
// Implemented in anticheat.cpp.
// 0x0009d8b0
void GameDeviceIntegrityCycle();

// 0x001a25f8
extern BOOL g_deviceMaterialOverrideEnabled;
// 0x001a25fc
extern BYTE* g_deviceMaterialOverrides;
// R5 address to verify
extern DWORD g_deviceMaterialIndex;
// R5 address to verify
extern void* g_devicePlateTexture;
// 0x0026eba0
extern BYTE* g_devicePlateManager;
// 0x000a4470
void* GameGetVehiclePlateTexture();
// 0x000c3e20
void ClientSetRenderState(DWORD state, DWORD value);

#endif // IDIRECT3DDEVICE9HOOK_H
