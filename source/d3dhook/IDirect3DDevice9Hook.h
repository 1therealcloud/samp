#ifndef SAMP_DIRECT3D_DEVICE9_HOOK_H
#define SAMP_DIRECT3D_DEVICE9_HOOK_H
#include <d3d9.h>
// Four-byte native proxy; the real device is a shared global.
class IDirect3DDevice9Hook : public IDirect3DDevice9 {
public:
    // samp.dll RVA: 0x00073f60
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj);
    // samp.dll RVA: 0x00073f70
    ULONG __stdcall AddRef();
    // samp.dll RVA: 0x00073f80
    ULONG __stdcall Release();
    // samp.dll RVA: 0x00073f90
    HRESULT __stdcall TestCooperativeLevel();
    // samp.dll RVA: 0x00073fa0
    UINT __stdcall GetAvailableTextureMem();
    // samp.dll RVA: 0x00073fb0
    HRESULT __stdcall EvictManagedResources();
    // samp.dll RVA: 0x00073fc0
    HRESULT __stdcall GetDirect3D(IDirect3D9** ppD3D9);
    // samp.dll RVA: 0x00073fd0
    HRESULT __stdcall GetDeviceCaps(D3DCAPS9* pCaps);
    // samp.dll RVA: 0x00073fe0
    HRESULT __stdcall GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode);
    // samp.dll RVA: 0x00073ff0
    HRESULT __stdcall GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters);
    // samp.dll RVA: 0x00074000
    HRESULT __stdcall SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
    // samp.dll RVA: 0x00074010
    void __stdcall SetCursorPosition(int X, int Y, DWORD Flags);
    // samp.dll RVA: 0x00074020
    BOOL __stdcall ShowCursor(BOOL bShow);
    // samp.dll RVA: 0x00074030
    HRESULT __stdcall CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
    // samp.dll RVA: 0x00074040
    HRESULT __stdcall GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
    // samp.dll RVA: 0x00074050
    UINT __stdcall GetNumberOfSwapChains();
    // samp.dll RVA: 0x00074060
    HRESULT __stdcall Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
    // Render/UI/RPC body remains declaration-only.
    // samp.dll RVA: 0x000751b0
    HRESULT __stdcall Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
    // samp.dll RVA: 0x00074090
    HRESULT __stdcall GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    // samp.dll RVA: 0x000740a0
    HRESULT __stdcall GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus);
    // samp.dll RVA: 0x000740b0
    HRESULT __stdcall SetDialogBoxMode(BOOL bEnableDialogs);
    // samp.dll RVA: 0x000740c0
    void __stdcall SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp);
    // samp.dll RVA: 0x000740d0
    void __stdcall GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp);
    // samp.dll RVA: 0x000740e0
    HRESULT __stdcall CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle);
    // samp.dll RVA: 0x000740f0
    HRESULT __stdcall CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle);
    // samp.dll RVA: 0x00074100
    HRESULT __stdcall CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle);
    // samp.dll RVA: 0x00074110
    HRESULT __stdcall CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
    // samp.dll RVA: 0x00074120
    HRESULT __stdcall CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle);
    // samp.dll RVA: 0x00074130
    HRESULT __stdcall CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    // samp.dll RVA: 0x00074140
    HRESULT __stdcall CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    // samp.dll RVA: 0x00074160
    HRESULT __stdcall UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint);
    // samp.dll RVA: 0x00074170
    HRESULT __stdcall UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
    // samp.dll RVA: 0x00074180
    HRESULT __stdcall GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface);
    // samp.dll RVA: 0x000741a0
    HRESULT __stdcall GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface);
    // samp.dll RVA: 0x000741c0
    HRESULT __stdcall StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter);
    // samp.dll RVA: 0x000741e0
    HRESULT __stdcall ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color);
    // samp.dll RVA: 0x00074200
    HRESULT __stdcall CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    // samp.dll RVA: 0x00074220
    HRESULT __stdcall SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
    // samp.dll RVA: 0x00074240
    HRESULT __stdcall GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);
    // samp.dll RVA: 0x00074260
    HRESULT __stdcall SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil);
    // samp.dll RVA: 0x00074280
    HRESULT __stdcall GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
    // samp.dll RVA: 0x000742a0
    HRESULT __stdcall BeginScene();
    // samp.dll RVA: 0x000742c0
    HRESULT __stdcall EndScene();
    // samp.dll RVA: 0x000742f0
    HRESULT __stdcall Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    // samp.dll RVA: 0x00074310
    HRESULT __stdcall SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* mat);
    // samp.dll RVA: 0x00074360
    HRESULT __stdcall GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* mat);
    // samp.dll RVA: 0x00074380
    HRESULT __stdcall MultiplyTransform(D3DTRANSFORMSTATETYPE argument0, CONST D3DMATRIX* argument1);
    // samp.dll RVA: 0x000743a0
    HRESULT __stdcall SetViewport(CONST D3DVIEWPORT9* pViewport);
    // samp.dll RVA: 0x000743c0
    HRESULT __stdcall GetViewport(D3DVIEWPORT9* pViewport);
    // samp.dll RVA: 0x000743e0
    HRESULT __stdcall SetMaterial(CONST D3DMATERIAL9* pMaterial);
    // samp.dll RVA: 0x00074400
    HRESULT __stdcall GetMaterial(D3DMATERIAL9* pMaterial);
    // samp.dll RVA: 0x00074420
    HRESULT __stdcall SetLight(DWORD Index, CONST D3DLIGHT9* argument1);
    // samp.dll RVA: 0x00074440
    HRESULT __stdcall GetLight(DWORD Index, D3DLIGHT9* argument1);
    // samp.dll RVA: 0x00074460
    HRESULT __stdcall LightEnable(DWORD Index, BOOL Enable);
    // samp.dll RVA: 0x00074480
    HRESULT __stdcall GetLightEnable(DWORD Index, BOOL* pEnable);
    // samp.dll RVA: 0x000744a0
    HRESULT __stdcall SetClipPlane(DWORD Index, CONST float* pPlane);
    // samp.dll RVA: 0x000744c0
    HRESULT __stdcall GetClipPlane(DWORD Index, float* pPlane);
    // samp.dll RVA: 0x000744e0
    HRESULT __stdcall SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
    // samp.dll RVA: 0x00074500
    HRESULT __stdcall GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue);
    // samp.dll RVA: 0x00074520
    HRESULT __stdcall CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB);
    // samp.dll RVA: 0x00074540
    HRESULT __stdcall BeginStateBlock();
    // samp.dll RVA: 0x00074560
    HRESULT __stdcall EndStateBlock(IDirect3DStateBlock9** ppSB);
    // samp.dll RVA: 0x00074580
    HRESULT __stdcall SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus);
    // samp.dll RVA: 0x000745a0
    HRESULT __stdcall GetClipStatus(D3DCLIPSTATUS9* pClipStatus);
    // samp.dll RVA: 0x000745c0
    HRESULT __stdcall GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture);
    // samp.dll RVA: 0x000745e0
    HRESULT __stdcall SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture);
    // samp.dll RVA: 0x00074720
    HRESULT __stdcall GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
    // samp.dll RVA: 0x00074740
    HRESULT __stdcall SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    // samp.dll RVA: 0x00074760
    HRESULT __stdcall GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue);
    // samp.dll RVA: 0x00074780
    HRESULT __stdcall SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    // samp.dll RVA: 0x000747a0
    HRESULT __stdcall ValidateDevice(DWORD* pNumPasses);
    // samp.dll RVA: 0x000747c0
    HRESULT __stdcall SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries);
    // samp.dll RVA: 0x000747e0
    HRESULT __stdcall GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries);
    // samp.dll RVA: 0x00074800
    HRESULT __stdcall SetCurrentTexturePalette(UINT PaletteNumber);
    // samp.dll RVA: 0x00074820
    HRESULT __stdcall GetCurrentTexturePalette(UINT *PaletteNumber);
    // samp.dll RVA: 0x00074840
    HRESULT __stdcall SetScissorRect(CONST RECT* pRect);
    // samp.dll RVA: 0x00074860
    HRESULT __stdcall GetScissorRect(RECT* pRect);
    // samp.dll RVA: 0x00074880
    HRESULT __stdcall SetSoftwareVertexProcessing(BOOL bSoftware);
    // samp.dll RVA: 0x000748a0
    BOOL __stdcall GetSoftwareVertexProcessing();
    // samp.dll RVA: 0x000748c0
    HRESULT __stdcall SetNPatchMode(float nSegments);
    // samp.dll RVA: 0x000748e0
    float __stdcall GetNPatchMode();
    // samp.dll RVA: 0x00074900
    HRESULT __stdcall DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    // samp.dll RVA: 0x00074920
    HRESULT __stdcall DrawIndexedPrimitive(D3DPRIMITIVETYPE argument0, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
    // samp.dll RVA: 0x00074940
    HRESULT __stdcall DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    // samp.dll RVA: 0x00074960
    HRESULT __stdcall DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    // samp.dll RVA: 0x00074980
    HRESULT __stdcall ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags);
    // samp.dll RVA: 0x000749a0
    HRESULT __stdcall CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
    // samp.dll RVA: 0x000749c0
    HRESULT __stdcall SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl);
    // samp.dll RVA: 0x000749e0
    HRESULT __stdcall GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl);
    // samp.dll RVA: 0x00074a00
    HRESULT __stdcall SetFVF(DWORD FVF);
    // samp.dll RVA: 0x00074a20
    HRESULT __stdcall GetFVF(DWORD* pFVF);
    // samp.dll RVA: 0x00074a40
    HRESULT __stdcall CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader);
    // samp.dll RVA: 0x00074a60
    HRESULT __stdcall SetVertexShader(IDirect3DVertexShader9* pShader);
    // samp.dll RVA: 0x00074a80
    HRESULT __stdcall GetVertexShader(IDirect3DVertexShader9** ppShader);
    // samp.dll RVA: 0x00074aa0
    HRESULT __stdcall SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
    // samp.dll RVA: 0x00074ac0
    HRESULT __stdcall GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    // samp.dll RVA: 0x00074ae0
    HRESULT __stdcall SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount);
    // samp.dll RVA: 0x00074b00
    HRESULT __stdcall GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    // samp.dll RVA: 0x00074b20
    HRESULT __stdcall SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount);
    // samp.dll RVA: 0x00074b40
    HRESULT __stdcall GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    // samp.dll RVA: 0x00074b60
    HRESULT __stdcall SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride);
    // samp.dll RVA: 0x00074b80
    HRESULT __stdcall GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride);
    // samp.dll RVA: 0x00074ba0
    HRESULT __stdcall SetStreamSourceFreq(UINT StreamNumber, UINT Divider);
    // samp.dll RVA: 0x00074bc0
    HRESULT __stdcall GetStreamSourceFreq(UINT StreamNumber, UINT* Divider);
    // samp.dll RVA: 0x00074be0
    HRESULT __stdcall SetIndices(IDirect3DIndexBuffer9* pIndexData);
    // samp.dll RVA: 0x00074c00
    HRESULT __stdcall GetIndices(IDirect3DIndexBuffer9** ppIndexData);
    // samp.dll RVA: 0x00074c20
    HRESULT __stdcall CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader);
    // samp.dll RVA: 0x00074c40
    HRESULT __stdcall SetPixelShader(IDirect3DPixelShader9* pShader);
    // samp.dll RVA: 0x00074c60
    HRESULT __stdcall GetPixelShader(IDirect3DPixelShader9** ppShader);
    // samp.dll RVA: 0x00074c80
    HRESULT __stdcall SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
    // samp.dll RVA: 0x00074ca0
    HRESULT __stdcall GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    // samp.dll RVA: 0x00074cc0
    HRESULT __stdcall SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount);
    // samp.dll RVA: 0x00074ce0
    HRESULT __stdcall GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    // samp.dll RVA: 0x00074d00
    HRESULT __stdcall SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount);
    // samp.dll RVA: 0x00074d20
    HRESULT __stdcall GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    // samp.dll RVA: 0x00074d40
    HRESULT __stdcall DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo);
    // samp.dll RVA: 0x00074d60
    HRESULT __stdcall DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo);
    // samp.dll RVA: 0x00074d80
    HRESULT __stdcall DeletePatch(UINT Handle);
    // samp.dll RVA: 0x00074da0
    HRESULT __stdcall CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);
};
typedef char DeviceProxyExtent[(sizeof(IDirect3DDevice9Hook)==4)?1:-1];
// samp.dll RVA: 0x0017ec00
extern D3DMATRIX g_deviceViewMatrix;
// samp.dll RVA: 0x0017ec40
extern D3DMATRIX g_deviceProjectionMatrix;
// samp.dll RVA: 0x0017ec80
extern D3DMATRIX g_deviceWorldMatrix;
// Implemented in gui/device_lifecycle.cpp; downstream UI/pool dependencies remain.
// samp.dll RVA: 0x000c5620
void ClientDeviceObjectsLost();
// Implemented in gui/device_lifecycle.cpp; downstream UI dependencies remain.
// samp.dll RVA: 0x000c5880
void ClientDeviceObjectsReset();
// Reviewed dependency; body is not recovered in this profile.
// samp.dll RVA: 0x0009d6f0
void GameDeviceIntegrityCycle();
#endif
