#include "playertags.h"
#include "chatbubble.h"
#include "d3dhook/IDirect3DDevice9Hook.h"
#include "deathwindow.h"
#include "fontrender.h"
#include "game/aimstuff.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/util.h"
#include "game/vehicle.h"
#include "gui/device_lifecycle.h"
#include "gui/device_objects.h"
#include "main.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include <new>
#include <stdio.h>
#include <string.h>
// samp.dll RVA: 0x0017e988
CMatrix g_nametagVehicleMatrix;
// samp.dll RVA: 0x0017eb08
char g_nametagName[128];
// samp.dll RVA: 0x0017eb88
CMatrix g_nametagLocalMatrix;
// samp.dll RVA: 0x0017ebcc
CVector g_nametagBone;
// samp.dll RVA: 0x0017ebf4
CVector g_nametagDrawPosition;
// samp.dll RVA: 0x0011c7a4
const Scripting::OpcodeInfo g_nametagLineOfSightCommand = {0x6bd, "ffffffiiiii"};
// samp.dll RVA: 0x0011c7b8
const char g_nametagNameFormat[8] = "%s (%d)";
// samp.dll RVA: 0x00140878
PlayerTagVertex g_playerTagVertices[12] = {{0, 0, 0, 1, 0xff000000}, {0, 25, 0, 1, 0xff000000}, {100, 25, 0, 1, 0xff000000}, {100, 0, 0, 1, 0xff000000},
                                           {2, 2, 0, 1, 0xff4b0b14}, {2, 18, 0, 1, 0xff4b0b14}, {96, 18, 0, 1, 0xff4b0b14},  {96, 2, 0, 1, 0xff4b0b14},
                                           {2, 2, 0, 1, 0xffb92228}, {2, 18, 0, 1, 0xffb92228}, {96, 18, 0, 1, 0xffb92228},  {96, 2, 0, 1, 0xffb92228}};
// samp.dll RVA: 0x0011c674
const float g_playerTagFillScale = 0.38461539149284363f;
// samp.dll RVA: 0x0011c678
const float g_playerTagInnerBottom = 23.0f;
// samp.dll RVA: 0x0011c67c
const float g_playerTagHalfWidth = 19.0f;
// samp.dll RVA: 0x0011c680
const float g_playerTagOuterRight = 21.0f;
// samp.dll RVA: 0x0011c684
const float g_playerTagOuterBottom = 24.0f;
// samp.dll RVA: 0x0011c688
const float g_playerTagOuterTop = 18.0f;
// samp.dll RVA: 0x00117874
const float g_playerTagOuterLeft = 20.0f;
// samp.dll RVA: 0x0011797c
const float g_playerTagArmourOffset = 8.0f;
// samp.dll RVA: 0x0011c670
const float g_playerTagDistanceHeight = 0.0475f;
// samp.dll RVA: 0x00118388
const char g_playerTagStatusBack[2] = "C";
// samp.dll RVA: 0x00118370
const char g_playerTagStatusFront[2] = "E";
// samp.dll RVA: 0x0006d090
void CPlayerTags::OnLostDevice()
{
    if (state)
    {
        state->Release();
        state = 0;
    }
    if (sprite)
        sprite->OnLostDevice();
}
// samp.dll RVA: 0x0006d0c0
void CPlayerTags::OnResetDevice()
{
    if (sprite)
        sprite->OnResetDevice();
    if (!state)
        device->CreateStateBlock(D3DSBT_ALL, &state);
}

// samp.dll RVA: 0x0006c730
CPlayerTags::CPlayerTags(IDirect3DDevice9* value)
{
    device = value;
    state = 0;
    sprite = 0;
    D3DXCreateSprite(device, &sprite);
}

// samp.dll RVA: 0x0006c760
CPlayerTags::~CPlayerTags()
{
    if (state)
        state->Release();
    // Original uses scalar delete storage release, not COM Release.
    if (sprite)
    {
        ::operator delete(sprite);
        sprite = 0;
    }
}
// samp.dll RVA: 0x0006c790
void CPlayerTags::EndHealthBar()
{
    if (sprite)
        sprite->End();
    if (state)
        state->Apply();
}
// samp.dll RVA: 0x0006c7c0
void CPlayerTags::BeginLabel()
{
    if (sprite)
        sprite->Begin(0x10);
}
// samp.dll RVA: 0x0006c7d0
void CPlayerTags::EndLabel()
{
    if (sprite)
        sprite->End();
}
// samp.dll RVA: 0x0006d0f0
void CPlayerTags::BeginHealthBar()
{
    if (!state)
        OnResetDevice();
    if (state)
        state->Capture();
    if (sprite)
        sprite->Begin(1);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    device->SetVertexShader(0);
    device->SetPixelShader(0);
    device->SetRenderState(D3DRS_ZENABLE, FALSE);
}

// samp.dll RVA: 0x0006cae0
void CPlayerTags::DrawHealthBar(const CVector* position, float health, float armour, float distance)
{
    CVector point;
    memcpy(&point, position, 12);
    float* height = &point.z;
    // clang-format off
    __asm {
        fld distance
        fmul g_playerTagDistanceHeight
        mov eax,height
        fadd dword ptr [eax]
        fadd g_chatBubbleHeightLift
        fstp dword ptr [eax]
    }
    // clang-format on
    D3DVIEWPORT9 viewport;
    device->GetViewport(&viewport);
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    D3DXVECTOR3 projected;
    D3DXVec3Project(&projected, (D3DXVECTOR3*)&point, &viewport, (D3DXMATRIX*)&g_deviceProjectionMatrix, (D3DXMATRIX*)&g_deviceViewMatrix, &identity);
    BYTE visible, acceptedHealth;
    float* depth = &projected.z;
    // clang-format off
    __asm {
        mov eax,depth
        fld dword ptr [eax]
        fcomp g_quaternionMatrixOne
        fnstsw ax
        test ah,41h
        setnz visible
    }
    // clang-format on
    if (!visible) return;
    // clang-format off
    __asm {
        fld health
        fcomp g_objectScaleLowerBound
        fnstsw ax
        test ah,5
        setp acceptedHealth
    }
    // clang-format on
    // Both gates accept unordered operands; only ordered negative health is rejected.
    if (!acceptedHealth) return;
    float x = (float)(int)projected.x, y = (float)(int)projected.y;
    PlayerTagVertex *border = g_playerTagVertices, *background = border + 4, *fill = border + 8;
    for (int i = 0; i < 4; i++)
    {
        fill[i].color = 0xffb92228;
        background[i].color = 0xff4b0b14;
    }
    border[0].x = border[1].x = x - g_playerTagOuterLeft;
    border[0].y = border[3].y = y + g_playerTagOuterTop;
    border[1].y = border[2].y = y + g_playerTagOuterBottom;
    border[2].x = border[3].x = x + g_playerTagOuterRight;
    background[0].x = background[1].x = fill[0].x = fill[1].x = x - g_playerTagHalfWidth;
    background[0].y = background[3].y = fill[0].y = fill[3].y = y + g_playerTagHalfWidth;
    background[1].y = background[2].y = fill[1].y = fill[2].y = y + g_playerTagInnerBottom;
    background[2].x = background[3].x = x + g_playerTagHalfWidth;
    if (health > g_vehicleTrainSpeedMaximum)
        health = g_vehicleTrainSpeedMaximum;
    float right;
    // clang-format off
    __asm {
        fld health
        fmul g_playerTagFillScale
        fsub g_playerTagHalfWidth
        fadd x
        fstp right
    }
    // clang-format on
    fill[2].x = fill[3].x = right;
    if (armour > g_objectScaleLowerBound)
    {
        for (i = 0; i < 4; i++)
        {
            border[i].y += g_playerTagArmourOffset;
            background[i].y = fill[i].y + g_playerTagArmourOffset;
            fill[i].y += g_playerTagArmourOffset;
        }
    }
    device->SetFVF(0x44);
    device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, border, sizeof(PlayerTagVertex));
    device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, background, sizeof(PlayerTagVertex));
    device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, fill, sizeof(PlayerTagVertex));
    if (armour > g_objectScaleLowerBound)
    {
        for (i = 0; i < 4; i++)
        {
            fill[i].color = 0xffc8c8c8;
            background[i].color = 0xff282828;
        }
        for (i = 0; i < 4; i++)
        {
            border[i].y -= g_playerTagArmourOffset;
            background[i].y -= g_playerTagArmourOffset;
            fill[i].y -= g_playerTagArmourOffset;
        }
        if (armour > g_vehicleTrainSpeedMaximum)
            armour = g_vehicleTrainSpeedMaximum;
        // clang-format off
        __asm {
            fld armour
            fmul g_playerTagFillScale
            fsub g_playerTagHalfWidth
            fadd x
            fstp right
        }
        // clang-format on
        fill[2].x = fill[3].x = right;
        device->SetFVF(0x44);
        device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, border, sizeof(PlayerTagVertex));
        device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, background, sizeof(PlayerTagVertex));
        device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, fill, sizeof(PlayerTagVertex));
    }
}
// samp.dll RVA: 0x0006c7e0
void CPlayerTags::DrawLabel(const CVector* position, const char* text, DWORD color, float distance, BYTE drawStatus, int status)
{
    CVector point;
    memcpy(&point, position, 12);
    float* height = &point.z;
    // clang-format off
    __asm {
        fld distance
        fmul g_playerTagDistanceHeight
        mov eax,height
        fadd dword ptr [eax]
        fadd g_chatBubbleHeightLift
        fstp dword ptr [eax]
    }
    // clang-format on
    D3DVIEWPORT9 viewport;
    device->GetViewport(&viewport);
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    D3DXVECTOR3 projected;
    D3DXVec3Project(&projected, (D3DXVECTOR3*)&point, &viewport, (D3DXMATRIX*)&g_deviceProjectionMatrix, (D3DXMATRIX*)&g_deviceViewMatrix, &identity);
    BYTE visible;
    float* depth = &projected.z;
    // clang-format off
    __asm {
        mov eax,depth
        fld dword ptr [eax]
        fcomp g_quaternionMatrixOne
        fnstsw ax
        test ah,41h
        setnz visible
    }
    // clang-format on
    // Native accepts unordered projected depth, ignores viewport/projection failures.
    if (!visible) return;
    int x = (int)projected.x, y = (int)projected.y;
    FontSize size;
    g_deviceFonts->GetLittleTextScreenSize(&size, text, 0);
    RECT rect = {x - size.cx / 2, y + 17 - size.cy, x + size.cx / 2, y};
    g_deviceFonts->DrawLittleText(sprite, text, rect, 0x100, color, TRUE);
    if (!drawStatus)
        return;
    CDeathWindow* window = (CDeathWindow*)g_deviceDeathWindow;
    if (!window)
        return;
    if (!*(DWORD*)(window->nativeState + 0x14b))
    {
        window->CreateAuxiliaryFonts();
        window = (CDeathWindow*)g_deviceDeathWindow;
    }
    ID3DXFont* front = *(ID3DXFont**)(window->nativeState + 0x14f);
    ID3DXFont* back = *(ID3DXFont**)(window->nativeState + 0x153);
    if (!front || !back)
        return;
    RECT bounds = {0, 0, 0, 0};
    back->DrawTextA(0, g_playerTagStatusBack, 1, &bounds, 0x505, 0);
    int left = x - 22 - (bounds.right - bounds.left) / 2;
    int top = y + 17 + (bounds.bottom - bounds.top) / 2;
    RECT badge = {left, top, left + 1, top + 2};
    if (status == 2)
    {
        back->DrawTextA(0, g_playerTagStatusBack, 1, &badge, 0x105, 0xff000000);
        badge.top -= 2;
        badge.left -= 1;
        front->DrawTextA(0, g_playerTagStatusFront, 1, &badge, 0x105, 0xffe3e1e3);
    }
}

// samp.dll RVA: 0x00074dc0
void ClientDrawLabels()
{
    if (!g_pNetGame || !(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5))[0x38])
        return;
    g_devicePlayerTags->BeginLabel();
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    CPlayerPool* players = *(CPlayerPool**)(pools + 8);
    g_pGame->GetPlayerPed()->GetMatrix(&g_nametagLocalMatrix);
    int highest = *(int*)((BYTE*)players + 0x22);
    for (DWORD counter = 0; (int)counter <= highest; counter++)
    {
        WORD id = (WORD)counter;
        if (id >= 1004 || *(DWORD*)((BYTE*)players + 0xfd6 + id * 4) != 1)
            continue;
        BYTE* entry = *(BYTE**)((BYTE*)players + 0x26 + id * 4);
        if (!entry)
            continue;
        CRemotePlayer* remote = *(CRemotePlayer**)(entry + 8);
        if (!remote)
            continue;
        BYTE* remoteState = (BYTE*)remote;
        CPed* ped = *(CPed**)(remoteState + 4);
        if (!ped || !remoteState[0x1a] || !*(DWORD*)(remoteState + 0x10))
            continue;
        BYTE nearby;
        // clang-format off
        // Keep the distance in ST0 and reload settings after the callee.
        __asm {
            mov ecx,ped
            call CEntity::GetDistanceToCamera
            mov ecx,g_pNetGame
            mov edx,dword ptr [ecx+3d5h]
            fcomp dword ptr [edx+27h]
            fnstsw ax
            test ah,41h
            setnp nearby
        }
        // clang-format on
        if (!nearby) continue;
        CVehicle* vehicle = *(CVehicle**)(remoteState + 8);
        if (remoteState[0x1a] == 0x13 && vehicle && vehicle->IsRC())
        {
            vehicle = *(CVehicle**)(remoteState + 8);
            vehicle->GetMatrix(&g_nametagVehicleMatrix);
            memcpy(&g_nametagBone, &g_nametagVehicleMatrix.position, 12);
        }
        else
        {
            if (!ped->DoesExist())
                continue;
            memset(&g_nametagBone, 0, 12);
            ped->GetBonePosition(8, &g_nametagBone);
        }
        memcpy(&g_nametagDrawPosition, &g_nametagBone, 12);
        CameraAim* aim = AimStuff::GetInternalAim();
        DWORD visible = 0;
        if ((*(BYTE**)((BYTE*)g_pNetGame + 0x3d5))[0x2f])
            visible = Scripting::ProcessCommand(&g_nametagLineOfSightCommand, g_nametagDrawPosition.x, g_nametagDrawPosition.y, g_nametagDrawPosition.z, aim->source.x,
                                                aim->source.y, aim->source.z, 1, 0, 0, 1, 0);
        if ((*(BYTE**)((BYTE*)g_pNetGame + 0x3d5))[0x2f] && !visible)
            continue;
        sprintf(g_nametagName, g_nametagNameFormat, players->GetName(id), (int)counter);
        if (*(DWORD*)(remoteState + 0x1c))
            continue;
        int status = remote->GetStatus();
        BYTE drawStatus = ((BYTE*)g_pNetGame)[0x234];
        float distance = ped->GetDistanceToCamera();
        DWORD color = remote->GetColorAsARGB();
        g_devicePlayerTags->DrawLabel(&g_nametagDrawPosition, g_nametagName, color, distance, drawStatus, status);
    }
    g_devicePlayerTags->EndLabel();
}

// samp.dll RVA: 0x00073cb0
void ClientDrawPlayerTags()
{
    if (!g_pNetGame || !(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5))[0x38])
        return;
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    CPlayerPool* players = *(CPlayerPool**)(pools + 8);
    g_pGame->GetPlayerPed()->GetMatrix(&g_nametagLocalMatrix);
    g_devicePlayerTags->BeginHealthBar();
    int highest = *(int*)((BYTE*)players + 0x22);
    for (DWORD counter = 0; (int)counter <= highest; counter++)
    {
        WORD id = (WORD)counter;
        if (id >= 1004 || *(DWORD*)((BYTE*)players + 0xfd6 + id * 4) != 1)
            continue;
        BYTE* entry = *(BYTE**)((BYTE*)players + 0x26 + id * 4);
        if (!entry)
            continue;
        BYTE* remote = *(BYTE**)(entry + 8);
        if (!remote)
            continue;
        CPed* ped = *(CPed**)(remote + 4);
        if (!ped || !remote[0x1a] || !*(DWORD*)(remote + 0x10))
            continue;
        BYTE nearby;
        // clang-format off
        __asm {
            mov ecx,ped
            call CEntity::GetDistanceToCamera
            mov ecx,g_pNetGame
            mov edx,dword ptr [ecx+3d5h]
            fcomp dword ptr [edx+27h]
            fnstsw ax
            test ah,41h
            setnp nearby
        }
        // clang-format on
        if (!nearby) continue;
        CVehicle* vehicle = *(CVehicle**)(remote + 8);
        if (remote[0x1a] == 0x13 && vehicle && vehicle->IsRC())
        {
            vehicle = *(CVehicle**)(remote + 8);
            vehicle->GetMatrix(&g_nametagVehicleMatrix);
            memcpy(&g_nametagBone, &g_nametagVehicleMatrix.position, 12);
        }
        else
        {
            if (!ped->DoesExist())
                continue;
            memset(&g_nametagBone, 0, 12);
            ped->GetBonePosition(8, &g_nametagBone);
        }
        memcpy(&g_nametagDrawPosition, &g_nametagBone, 12);
        CameraAim* aim = AimStuff::GetInternalAim();
        DWORD visible = 0;
        if ((*(BYTE**)((BYTE*)g_pNetGame + 0x3d5))[0x2f])
            visible = Scripting::ProcessCommand(&g_nametagLineOfSightCommand, g_nametagDrawPosition.x, g_nametagDrawPosition.y, g_nametagDrawPosition.z, aim->source.x,
                                                aim->source.y, aim->source.z, 1, 0, 0, 1, 0);
        if ((*(BYTE**)((BYTE*)g_pNetGame + 0x3d5))[0x2f] && !visible)
            continue;
        if (*(DWORD*)(remote + 0x1c))
            continue;
        // Reported network values are captured before the distance call.
        float health, armour;
        memcpy(&health, remote + 0x1b0, 4);
        memcpy(&armour, remote + 0x1ac, 4);
        float distance = ped->GetDistanceToCamera();
        g_devicePlayerTags->DrawHealthBar(&g_nametagDrawPosition, health, armour, distance);
    }
    g_devicePlayerTags->EndHealthBar();
}

// Legacy0.2x renderer retained in DL alongside the newer screen-space tags.
// samp.dll RVA: 0x00140998
__declspec(align(4)) BYTE g_legacyPlayerTagStorage[448] = {
    0x9a, 0x99, 0x99, 0xbe, 0x8f, 0xc2, 0xf5, 0xbc, 0x6f, 0x12, 0x03, 0xbb, 0x00, 0x00, 0x00, 0xff, 0x9a, 0x99, 0x99, 0xbe, 0x8f, 0xc2, 0xf5, 0x3c, 0x6f, 0x12, 0x03, 0xbb,
    0x00, 0x00, 0x00, 0xff, 0x9a, 0x99, 0x99, 0x3e, 0x8f, 0xc2, 0xf5, 0x3c, 0x6f, 0x12, 0x03, 0xbb, 0x00, 0x00, 0x00, 0xff, 0x9a, 0x99, 0x99, 0x3e, 0x8f, 0xc2, 0xf5, 0xbc,
    0x6f, 0x12, 0x03, 0xbb, 0x00, 0x00, 0x00, 0xff, 0xe1, 0x7a, 0x94, 0xbe, 0x0a, 0xd7, 0xa3, 0xbc, 0x6f, 0x12, 0x83, 0xba, 0x1e, 0x15, 0x7d, 0xff, 0xe1, 0x7a, 0x94, 0xbe,
    0x0a, 0xd7, 0xa3, 0x3c, 0x6f, 0x12, 0x83, 0xba, 0x1e, 0x15, 0x7d, 0xff, 0xe1, 0x7a, 0x94, 0x3e, 0x0a, 0xd7, 0xa3, 0x3c, 0x6f, 0x12, 0x83, 0xba, 0x1e, 0x15, 0x7d, 0xff,
    0xe1, 0x7a, 0x94, 0x3e, 0x0a, 0xd7, 0xa3, 0xbc, 0x6f, 0x12, 0x83, 0xba, 0x1e, 0x15, 0x7d, 0xff, 0xe1, 0x7a, 0x94, 0xbe, 0x0a, 0xd7, 0xa3, 0xbc, 0x00, 0x00, 0x00, 0x00,
    0x28, 0x22, 0xc3, 0xff, 0xe1, 0x7a, 0x94, 0xbe, 0x0a, 0xd7, 0xa3, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x28, 0x22, 0xc3, 0xff, 0xe1, 0x7a, 0x94, 0x3e, 0x0a, 0xd7, 0xa3, 0x3c,
    0x00, 0x00, 0x00, 0x00, 0x28, 0x22, 0xc3, 0xff, 0xe1, 0x7a, 0x94, 0x3e, 0x0a, 0xd7, 0xa3, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x28, 0x22, 0xc3, 0xff, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x58, 0x00, 0x15, 0x15, 0x00, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x07, 0x08, 0x0a, 0x11, 0x0d, 0x17, 0x0f, 0x06, 0x0a, 0x0a, 0x0d, 0x11, 0x08, 0x0a, 0x08, 0x0a, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0a, 0x0a,
    0x11, 0x11, 0x11, 0x0b, 0x15, 0x0e, 0x0e, 0x0f, 0x10, 0x0d, 0x0c, 0x10, 0x10, 0x08, 0x0a, 0x0f, 0x0c, 0x12, 0x10, 0x11, 0x0d, 0x11, 0x0f, 0x0e, 0x0d, 0x0f, 0x0e, 0x15,
    0x0e, 0x0d, 0x0e, 0x0a, 0x0a, 0x0a, 0x11, 0x0d, 0x0d, 0x0d, 0x0d, 0x0b, 0x0d, 0x0d, 0x07, 0x0d, 0x0d, 0x06, 0x07, 0x0c, 0x06, 0x14, 0x0d, 0x0d, 0x0d, 0x0d, 0x09, 0x0b,
    0x08, 0x0d, 0x0c, 0x11, 0x0c, 0x0c, 0x0b, 0x0d, 0x0a, 0x0d, 0x11, 0x15, 0x0d, 0x15, 0x06, 0x0d, 0x0a, 0x11, 0x0d, 0x0d, 0x0d, 0x20, 0x0e, 0x0a, 0x16, 0x15, 0x0e, 0x15,
    0x15, 0x06, 0x06, 0x0a, 0x0a, 0x0b, 0x0d, 0x15, 0x0d, 0x15, 0x0b, 0x0a, 0x15, 0x15, 0x0b, 0x0d, 0x07, 0x08, 0x0d, 0x0d, 0x0d, 0x0d, 0x0a, 0x0d, 0x0d, 0x15, 0x0b, 0x0e,
    0x11, 0x0a, 0x15, 0x0d, 0x0b, 0x11, 0x0b, 0x0b, 0x0d, 0x0d, 0x0d, 0x08, 0x0d, 0x0b, 0x0b, 0x0e, 0x15, 0x15, 0x15, 0x0b, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x14, 0x0f,
    0x0d, 0x0d, 0x0d, 0x0d, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0f, 0x0f, 0x0f, 0x0f, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x14, 0x0b, 0x0d, 0x0d, 0x0d, 0x0d, 0x06, 0x06, 0x06, 0x06, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x11, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0c, 0x0d, 0x0c};
// samp.dll RVA: 0x0011c6b4
const float g_legacyPlayerTagInv256 = 0.00390625f;
// samp.dll RVA: 0x0011c6b8
const float g_legacyPlayerTagInv16 = 0.0625f;
// samp.dll RVA: 0x0011c6bc
const char g_legacyPlayerTagTextureName[] = "bland.png";
struct LegacyHealthBarVertex
{
    float x, y, z;
    DWORD color;
};
#define HealthBarBDRVertices ((LegacyHealthBarVertex*)(g_legacyPlayerTagStorage))
#define HealthBarBGVertices ((LegacyHealthBarVertex*)(g_legacyPlayerTagStorage + 64))
#define HealthBarVertices ((LegacyHealthBarVertex*)(g_legacyPlayerTagStorage + 128))
#define HealthBarVertices_s LegacyHealthBarVertex
// samp.dll RVA: 0x0006d8b0
void CLegacyPlayerTags::NameTextVertex_s::Initialise(float fX, float fY, float fTu, float fTv)
{
    memcpy(&x, &fX, 4);
    memcpy(&y, &fY, 4);
    z = 0;
    memcpy(&tu, &fTu, 4);
    memcpy(&tv, &fTv, 4);
}
// samp.dll RVA: 0x0006d8e0
CLegacyPlayerTags::CLegacyPlayerTags(IDirect3DDevice9* pDevice)
{
    m_pDevice = pDevice;
    m_pTexture = NULL;
    m_pVB = NULL;
    m_pOldStates = NULL;
    m_pNewStates = NULL;

    m_fCharWidth = 0.0f;
    m_fCharHeight = 0.0f;
    m_fScale = 0.0075f;
}

// samp.dll RVA: 0x0006d910
CLegacyPlayerTags::~CLegacyPlayerTags()
{
    if (m_pTexture)
        m_pTexture->Release();
    if (m_pVB)
        m_pVB->Release();
    if (m_pOldStates)
        m_pOldStates->Release();
    if (m_pNewStates)
        m_pNewStates->Release();
}

// samp.dll RVA: 0x0006df10
void CLegacyPlayerTags::Begin()
{
    if ((!m_pOldStates) || (!m_pNewStates))
    {
        RestoreDeviceObjects();
        if ((!m_pOldStates) || (!m_pNewStates))
            return;
    }
    m_pOldStates->Capture();
    m_pNewStates->Apply();
}

// samp.dll RVA: 0x0006df50
void CLegacyPlayerTags::End()
{
    if (!m_pOldStates)
    {
        RestoreDeviceObjects();
        if (!m_pOldStates)
            return;
    }
    m_pOldStates->Apply();
}

// samp.dll RVA: 0x0006df80
void CLegacyPlayerTags::Draw(D3DXMATRIX* pWorldTransform, char* pNameText, DWORD dwColor, float fHealth)
{
    if ((!m_pVB) || (!m_pTexture))
    {
        RestoreDeviceObjects();
        if ((!m_pVB) || (!m_pTexture))
            return;
    }

    // Set the world transform.
    m_pDevice->SetTransform(D3DTS_WORLD, pWorldTransform);

    // --- begin: NameTag drawing... ---
    m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dwColor);
    m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(NameTextVertex_s));
    m_pDevice->SetTexture(0, m_pTexture);
    m_pDevice->SetFVF((D3DFVF_XYZ | D3DFVF_TEX1));

    int len = (int)strlen(pNameText);
    if (len > 80)
        len = 80;

    NameTextVertex_s* vert_ptr = 0;
    m_pVB->Lock(0, 0, (void**)&vert_ptr, D3DLOCK_DISCARD);

    float fX = 0.0f;
    for (int i = 0; i < len; i++)
    {
        fX += (float)g_legacyPlayerTagStorage[192 + (int)(signed char)pNameText[i]] * m_fScale;
    }
    fX = -(fX / 2);

    for (int i = 0; i < len; i++)
    {
        InitQuad(pNameText[i], fX, 0.05f, vert_ptr);
        fX += (float)g_legacyPlayerTagStorage[192 + (int)(signed char)pNameText[i]] * m_fScale;
        vert_ptr += 6;
    }

    m_pVB->Unlock();

    m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, len * 2);
    // --- end: NameTag drawing ---

    // --- begin: HealthBar drawing... ---
    m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
    m_pDevice->SetTexture(0, NULL);
    m_pDevice->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE));

    if (fHealth > 100.0f)
        fHealth = 100.0f;
    if (fHealth < 0.0f)
        fHealth = 0.0f;

    HealthBarBGVertices[0].x = HealthBarBGVertices[1].x = (0.0058f * fHealth) - 0.29f;

    m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBDRVertices, sizeof(HealthBarVertices_s));
    m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBGVertices, sizeof(HealthBarVertices_s));
    m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarVertices, sizeof(HealthBarVertices_s));
    // --- end: HealthBar drawing ---
}

// samp.dll RVA: 0x0006d950
__declspec(naked) void CLegacyPlayerTags::InitQuad(char c, float x, float y2, NameTextVertex_s* pVB)
{
    // clang-format off
    __asm {
        sub esp, 8
        movsx eax, byte ptr [esp + 0xc]
        movzx edx, byte ptr g_legacyPlayerTagStorage[eax + 192]
        mov dword ptr [esp + 0xc], edx
        mov edx, eax
        and edx, 0x8000000f
        fild dword ptr [esp + 0xc]
        push esi
        push edi
        fst dword ptr [esp + 0x14]
        fmul dword ptr [ecx + 0x1c]
        fadd dword ptr [esp + 0x18]
        fld dword ptr [ecx + 0x18]
        fmul dword ptr [ecx + 0x1c]
        fadd dword ptr [esp + 0x1c]
        jns quad_6d98d
        dec edx
        or edx, 0xfffffff0
        inc edx
    quad_6d98d:
        mov dword ptr [esp + 8], edx
        fild dword ptr [esp + 8]
        sub eax, edx
        mov edx, dword ptr [esp + 0x18]
        fmul g_legacyPlayerTagInv16
        fld dword ptr [esp + 0x14]
        mov dword ptr [esp + 0x14], eax
        fdiv dword ptr [ecx + 0x14]
        mov eax, dword ptr [esp + 0x20]
        mov ecx, dword ptr [esp + 0x1c]
        mov dword ptr [eax + 4], ecx
        xor ecx, ecx
        mov dword ptr [eax + 8], ecx
        mov esi, eax
        fmul g_legacyPlayerTagInv16
        fadd st(0),st(1)
        fstp dword ptr [esp + 8]
        fild dword ptr [esp + 0x14]
        fmul g_legacyPlayerTagInv256
        fst dword ptr [esp + 0x14]
        fadd g_legacyPlayerTagInv16
        fst dword ptr [esp + 0xc]
        fld dword ptr [esp + 0x18]
        fstp dword ptr [eax]
        fld st(1)
        fstp dword ptr [eax + 0xc]
        fstp dword ptr [eax + 0x10]
        mov dword ptr [eax + 0x14], edx
        mov edx, dword ptr [esp + 0x14]
        fld st(1)
        mov dword ptr [eax + 0x24], edx
        fstp dword ptr [eax + 0x18]
        mov edx, dword ptr [esp + 8]
        fstp dword ptr [eax + 0x20]
        mov dword ptr [eax + 0x1c], ecx
        mov dword ptr [eax + 0x34], edx
        fld st(1)
        mov edx, dword ptr [esp + 0x14]
        fstp dword ptr [eax + 0x28]
        mov dword ptr [eax + 0x38], edx
        mov dword ptr [eax + 0x30], ecx
        fstp dword ptr [eax + 0x2c]
        mov edi, dword ptr [esi]
        lea edx, [eax + 0x3c]
        mov dword ptr [edx], edi
        mov edi, dword ptr [esi + 4]
        mov dword ptr [edx + 4], edi
        mov edi, dword ptr [esi + 8]
        mov dword ptr [edx + 8], edi
        mov edi, dword ptr [esi + 0xc]
        mov esi, dword ptr [esi + 0x10]
        mov dword ptr [edx + 0xc], edi
        mov dword ptr [edx + 0x10], esi
        mov esi, dword ptr [eax + 0x28]
        lea edx, [eax + 0x50]
        mov dword ptr [edx], esi
        mov esi, dword ptr [eax + 0x2c]
        mov dword ptr [edx + 4], esi
        mov esi, dword ptr [eax + 0x30]
        mov dword ptr [edx + 8], esi
        mov esi, dword ptr [eax + 0x34]
        mov dword ptr [edx + 0xc], esi
        mov esi, dword ptr [eax + 0x38]
        mov dword ptr [edx + 0x10], esi
        fstp dword ptr [eax + 0x64]
        mov edx, dword ptr [esp + 0x1c]
        mov dword ptr [eax + 0x68], edx
        mov edx, dword ptr [esp + 0xc]
        mov dword ptr [eax + 0x6c], ecx
        mov ecx, dword ptr [esp + 8]
        pop edi
        mov dword ptr [eax + 0x70], ecx
        mov dword ptr [eax + 0x74], edx
        pop esi
        add esp, 8
        ret 0x10
    }
    // clang-format on
}

// samp.dll RVA: 0x0006da80
void CLegacyPlayerTags::DeleteDeviceObjects()
{
    if (m_pVB)
    {
        m_pVB->Release();
        m_pVB = NULL;
    }

    if (m_pOldStates)
    {
        m_pOldStates->Release();
        m_pOldStates = NULL;
    }

    if (m_pNewStates)
    {
        m_pNewStates->Release();
        m_pNewStates = NULL;
    }
}

// samp.dll RVA: 0x0006dad0
void CLegacyPlayerTags::RestoreDeviceObjects()
{
    if (!m_pTexture)
    {
        if (FAILED(D3DXCreateTextureFromFileExA(m_pDevice, g_legacyPlayerTagTextureName, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT,
                                                D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture)))
        {
            m_pTexture = NULL;
        }
        else
        {
            D3DSURFACE_DESC desc;
            m_pTexture->GetLevelDesc(0, &desc);
            m_fCharWidth = desc.Width / 16.0f;
            m_fCharHeight = desc.Height / 16.0f;
        }
    }

    if (!m_pVB)
    {
        if (FAILED(m_pDevice->CreateVertexBuffer(((80 * 6) + 1) * sizeof(NameTextVertex_s), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, (D3DFVF_XYZ | D3DFVF_TEX1), D3DPOOL_DEFAULT,
                                                 &m_pVB, NULL)))
        {
            m_pVB = NULL;
        }
    }

    if (!m_pOldStates)
    {
        D3DXMATRIX matTemp;
        D3DXMatrixIdentity(&matTemp);

        m_pDevice->BeginStateBlock();
        m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
        m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
        m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        m_pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
        m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, FALSE);
        m_pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
        m_pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
        m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        m_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
        m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
        m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
        m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
        m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        m_pDevice->SetTransform(D3DTS_WORLD, &matTemp);
        m_pDevice->SetStreamSource(0, NULL, 0, 0);
        m_pDevice->SetTexture(0, NULL);
        m_pDevice->SetFVF(0);
        m_pDevice->EndStateBlock(&m_pOldStates);
    }

    if (!m_pNewStates)
    {
        m_pDevice->BeginStateBlock();
        m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
        m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
        m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
        m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        m_pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
        m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, FALSE);
        m_pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
        m_pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
        m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        m_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
        m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
        m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
        m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        m_pDevice->EndStateBlock(&m_pNewStates);
    }
}

#undef HealthBarBDRVertices
#undef HealthBarBGVertices
#undef HealthBarVertices
#undef HealthBarVertices_s
