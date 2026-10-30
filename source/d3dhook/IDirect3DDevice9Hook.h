#ifndef SAMP_DIRECT3D_DEVICE9_HOOK_H
#define SAMP_DIRECT3D_DEVICE9_HOOK_H
#include <d3d9.h>
// Four-byte native proxy; the real device is a shared global.
class IDirect3DDevice9Hook : public IDirect3DDevice9
{
public:
    // 0x000c5e60
    IDirect3DDevice9Hook();
    // 0x00073f60
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj);
    // 0x00073f70
    ULONG __stdcall AddRef();
    // 0x00073f80
    ULONG __stdcall Release();
    // 0x00073f90
    HRESULT __stdcall TestCooperativeLevel();
    // 0x00073fa0
    UINT __stdcall GetAvailableTextureMem();
    // 0x00073fb0
    HRESULT __stdcall EvictManagedResources();
    // 0x00073fc0
    HRESULT __stdcall GetDirect3D(IDirect3D9** ppD3D9);
    // 0x00073fd0
    HRESULT __stdcall GetDeviceCaps(D3DCAPS9* pCaps);
    // 0x00073fe0
    HRESULT __stdcall GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode);
    // 0x00073ff0
    HRESULT __stdcall GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters);
    // 0x00074000
    HRESULT __stdcall SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
    // 0x00074010
    void __stdcall SetCursorPosition(int X, int Y, DWORD Flags);
    // 0x00074020
    BOOL __stdcall ShowCursor(BOOL bShow);
    // 0x00074030
    HRESULT __stdcall CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
    // 0x00074040
    HRESULT __stdcall GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
    // 0x00074050
    UINT __stdcall GetNumberOfSwapChains();
    // 0x00074060
    HRESULT __stdcall Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
    // Implemented in d3dhook/present.cpp; downstream rendering dependencies remain.
    // 0x000751b0
    HRESULT __stdcall Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
    // 0x00074090
    HRESULT __stdcall GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    // 0x000740a0
    HRESULT __stdcall GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus);
    // 0x000740b0
    HRESULT __stdcall SetDialogBoxMode(BOOL bEnableDialogs);
    // 0x000740c0
    void __stdcall SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp);
    // 0x000740d0
    void __stdcall GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp);
    // 0x000740e0
    HRESULT __stdcall CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle);
    // 0x000740f0
    HRESULT __stdcall CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture,
                                          HANDLE* pSharedHandle);
    // 0x00074100
    HRESULT __stdcall CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle);
    // 0x00074110
    HRESULT __stdcall CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
    // 0x00074120
    HRESULT __stdcall CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle);
    // 0x00074130
    HRESULT __stdcall CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable,
                                         IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    // 0x00074140
    HRESULT __stdcall CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard,
                                                IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    // 0x00074160
    HRESULT __stdcall UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint);
    // 0x00074170
    HRESULT __stdcall UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
    // 0x00074180
    HRESULT __stdcall GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface);
    // 0x000741a0
    HRESULT __stdcall GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface);
    // 0x000741c0
    HRESULT __stdcall StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter);
    // 0x000741e0
    HRESULT __stdcall ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color);
    // 0x00074200
    HRESULT __stdcall CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    // 0x00074220
    HRESULT __stdcall SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
    // 0x00074240
    HRESULT __stdcall GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);
    // 0x00074260
    HRESULT __stdcall SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil);
    // 0x00074280
    HRESULT __stdcall GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
    // 0x000742a0
    HRESULT __stdcall BeginScene();
    // 0x000742c0
    HRESULT __stdcall EndScene();
    // 0x000742f0
    HRESULT __stdcall Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    // 0x00074310
    HRESULT __stdcall SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* mat);
    // 0x00074360
    HRESULT __stdcall GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* mat);
    // 0x00074380
    HRESULT __stdcall MultiplyTransform(D3DTRANSFORMSTATETYPE argument0, CONST D3DMATRIX* argument1);
    // 0x000743a0
    HRESULT __stdcall SetViewport(CONST D3DVIEWPORT9* pViewport);
    // 0x000743c0
    HRESULT __stdcall GetViewport(D3DVIEWPORT9* pViewport);
    // 0x000743e0
    HRESULT __stdcall SetMaterial(CONST D3DMATERIAL9* pMaterial);
    // 0x00074400
    HRESULT __stdcall GetMaterial(D3DMATERIAL9* pMaterial);
    // 0x00074420
    HRESULT __stdcall SetLight(DWORD Index, CONST D3DLIGHT9* argument1);
    // 0x00074440
    HRESULT __stdcall GetLight(DWORD Index, D3DLIGHT9* argument1);
    // 0x00074460
    HRESULT __stdcall LightEnable(DWORD Index, BOOL Enable);
    // 0x00074480
    HRESULT __stdcall GetLightEnable(DWORD Index, BOOL* pEnable);
    // 0x000744a0
    HRESULT __stdcall SetClipPlane(DWORD Index, CONST float* pPlane);
    // 0x000744c0
    HRESULT __stdcall GetClipPlane(DWORD Index, float* pPlane);
    // 0x000744e0
    HRESULT __stdcall SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
    // 0x00074500
    HRESULT __stdcall GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue);
    // 0x00074520
    HRESULT __stdcall CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB);
    // 0x00074540
    HRESULT __stdcall BeginStateBlock();
    // 0x00074560
    HRESULT __stdcall EndStateBlock(IDirect3DStateBlock9** ppSB);
    // 0x00074580
    HRESULT __stdcall SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus);
    // 0x000745a0
    HRESULT __stdcall GetClipStatus(D3DCLIPSTATUS9* pClipStatus);
    // 0x000745c0
    HRESULT __stdcall GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture);
    // 0x000745e0
    HRESULT __stdcall SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture);
    // 0x00074720
    HRESULT __stdcall GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
    // 0x00074740
    HRESULT __stdcall SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    // 0x00074760
    HRESULT __stdcall GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue);
    // 0x00074780
    HRESULT __stdcall SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    // 0x000747a0
    HRESULT __stdcall ValidateDevice(DWORD* pNumPasses);
    // 0x000747c0
    HRESULT __stdcall SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries);
    // 0x000747e0
    HRESULT __stdcall GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries);
    // 0x00074800
    HRESULT __stdcall SetCurrentTexturePalette(UINT PaletteNumber);
    // 0x00074820
    HRESULT __stdcall GetCurrentTexturePalette(UINT* PaletteNumber);
    // 0x00074840
    HRESULT __stdcall SetScissorRect(CONST RECT* pRect);
    // 0x00074860
    HRESULT __stdcall GetScissorRect(RECT* pRect);
    // 0x00074880
    HRESULT __stdcall SetSoftwareVertexProcessing(BOOL bSoftware);
    // 0x000748a0
    BOOL __stdcall GetSoftwareVertexProcessing();
    // 0x000748c0
    HRESULT __stdcall SetNPatchMode(float nSegments);
    // 0x000748e0
    float __stdcall GetNPatchMode();
    // 0x00074900
    HRESULT __stdcall DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    // 0x00074920
    HRESULT __stdcall DrawIndexedPrimitive(D3DPRIMITIVETYPE argument0, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
    // 0x00074940
    HRESULT __stdcall DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    // 0x00074960
    HRESULT __stdcall DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData,
                                             D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    // 0x00074980
    HRESULT __stdcall ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl,
                                      DWORD Flags);
    // 0x000749a0
    HRESULT __stdcall CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
    // 0x000749c0
    HRESULT __stdcall SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl);
    // 0x000749e0
    HRESULT __stdcall GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl);
    // 0x00074a00
    HRESULT __stdcall SetFVF(DWORD FVF);
    // 0x00074a20
    HRESULT __stdcall GetFVF(DWORD* pFVF);
    // 0x00074a40
    HRESULT __stdcall CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader);
    // 0x00074a60
    HRESULT __stdcall SetVertexShader(IDirect3DVertexShader9* pShader);
    // 0x00074a80
    HRESULT __stdcall GetVertexShader(IDirect3DVertexShader9** ppShader);
    // 0x00074aa0
    HRESULT __stdcall SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
    // 0x00074ac0
    HRESULT __stdcall GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    // 0x00074ae0
    HRESULT __stdcall SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount);
    // 0x00074b00
    HRESULT __stdcall GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    // 0x00074b20
    HRESULT __stdcall SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount);
    // 0x00074b40
    HRESULT __stdcall GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    // 0x00074b60
    HRESULT __stdcall SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride);
    // 0x00074b80
    HRESULT __stdcall GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride);
    // 0x00074ba0
    HRESULT __stdcall SetStreamSourceFreq(UINT StreamNumber, UINT Divider);
    // 0x00074bc0
    HRESULT __stdcall GetStreamSourceFreq(UINT StreamNumber, UINT* Divider);
    // 0x00074be0
    HRESULT __stdcall SetIndices(IDirect3DIndexBuffer9* pIndexData);
    // 0x00074c00
    HRESULT __stdcall GetIndices(IDirect3DIndexBuffer9** ppIndexData);
    // 0x00074c20
    HRESULT __stdcall CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader);
    // 0x00074c40
    HRESULT __stdcall SetPixelShader(IDirect3DPixelShader9* pShader);
    // 0x00074c60
    HRESULT __stdcall GetPixelShader(IDirect3DPixelShader9** ppShader);
    // 0x00074c80
    HRESULT __stdcall SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
    // 0x00074ca0
    HRESULT __stdcall GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    // 0x00074cc0
    HRESULT __stdcall SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount);
    // 0x00074ce0
    HRESULT __stdcall GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    // 0x00074d00
    HRESULT __stdcall SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount);
    // 0x00074d20
    HRESULT __stdcall GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    // 0x00074d40
    HRESULT __stdcall DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo);
    // 0x00074d60
    HRESULT __stdcall DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo);
    // 0x00074d80
    HRESULT __stdcall DeletePatch(UINT Handle);
    // 0x00074da0
    HRESULT __stdcall CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);
};
typedef char DeviceProxyExtent[(sizeof(IDirect3DDevice9Hook) == 4) ? 1 : -1];
// 0x0017ec00
extern D3DMATRIX g_deviceViewMatrix;
// 0x0017ec40
extern D3DMATRIX g_deviceProjectionMatrix;
// 0x0017ec80
extern D3DMATRIX g_deviceWorldMatrix;
// Implemented in gui.cpp; downstream UI/pool dependencies remain.
// 0x000c5620
void ClientDeviceObjectsLost();
// Implemented in gui.cpp; downstream UI dependencies remain.
// 0x000c5880
void ClientDeviceObjectsReset();
// Implemented in anticheat.cpp.
// 0x0009d6f0
void GameDeviceIntegrityCycle();

// 0x001e0490
extern BOOL g_deviceMaterialOverrideEnabled;
// 0x001e0494
extern BYTE* g_deviceMaterialOverrides;
// 0x0017ebf0
extern DWORD g_deviceMaterialIndex;
// 0x0018f564
extern void* g_devicePlateTexture;
// 0x002aca30
extern BYTE* g_devicePlateManager;
// 0x000a41d0
void* GameGetVehiclePlateTexture();
// 0x000c54f0
void ClientSetRenderState(DWORD state, DWORD value);
#endif
