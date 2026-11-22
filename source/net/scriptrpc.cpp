#include "net/scriptrpc.h"

#include "game/actor.h"
#include "game/audio.h"
#include "game/building_removal.h"
#include "game/camera.h"
#include "game/custommodels.h"
#include "game/playerped.h"
#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "gui/textdrawselection.h"
#include "main.h"
#include "net/actorpool.h"
#include "net/gangzonepool.h"
#include "net/localplayer.h"
#include "net/menupool.h"
#include "net/netgame.h"
#include "net/netrpc.h"
#include "net/objectpool.h"
#include "net/playerinfo.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "net/textdrawpool.h"
#include "net/vehiclepool.h"

#include <BitStream.h>
#include <RakClientInterface.h>
#include <StringCompressor.h>

#include <string.h>
// R5 address to verify
const Scripting::OpcodeInfo g_rpcAttachObjectToPlayerCommand = {0x69b, "iiffffff"};
// R5 address to verify
const Scripting::OpcodeInfo g_rpcPlayerOpcode052cCommand = {0x52c, "ii"};
// R5 address to verify
const Scripting::OpcodeInfo g_rpcPlayerOpcode03fdCommand = {0x3fd, "ii"};
// R5 address to verify
const Scripting::OpcodeInfo g_rpcWidescreenCommand = {0x2a3, "i"};
// 0x000e63bc
const char g_rpcWidescreenMessage[] = "Widescreen = %i";
// 0x000e63cc
const char g_rpcInvalidSkinWarning[] = "Warning: SetPlayerSkin %d isn't a valid ped model.";
// 0x000e620c
const Scripting::OpcodeInfo g_rpcExplosionCommand = {0x948, "fffii"};
// 0x000e6404
const char g_rpcLargeTextDrawWarning[] = "Warning: ignoring large TextDraw size=%u";
// 0x000e63b8
const int g_rpcScrDisableVehicleCollisions = 167;
// 0x000e63b4
const int g_rpcScrForceClassSelection = 74;
// 0x000e63b0
const int g_rpcScrAttachObjectToPlayer = 75;
// R5 address to verify
const int g_rpcScrInitMenu = 76;
// 0x000e63a8
const int g_rpcScrShowMenu = 77;
// R5 address to verify
const int g_rpcScrHideMenu = 78;
// 0x000e63a0
const int g_rpcScrSetPlayerWantedLevel = 133;
// 0x000e639c
const int g_rpcScrShowTextDraw = 134;
// 0x000e6398
const int g_rpcScrHideTextDrawForPlayer = 135;
// 0x000e6394
const int g_rpcScrTextDrawSetString = 105;
// 0x000e6390
const int g_rpcScrGangZoneCreate = 108;
// 0x000e638c
const int g_rpcScrGangZoneDestroy = 120;
// 0x000e6388
const int g_rpcScrGangZoneFlash = 121;
// 0x000e6384
const int g_rpcScrGangZoneStopFlash = 85;
// 0x000e6380
const int g_rpcScrApplyAnimation = 86;
// 0x000e637c
const int g_rpcScrClearAnimations = 87;
// 0x000e6378
const int g_rpcScrSetPlayerSpecialAction = 88;
// 0x000e6374
const int g_rpcScrEnableStuntBonusForPlayer = 104;
// 0x000e637c
const int g_rpcScrSetPlayerFightingStyle = 89;
// 0x000e636c
const int g_rpcScrSetPlayerVelocity = 90;
// 0x000e6368
const int g_rpcScrSetVehicleVelocity = 91;
// 0x000e6364
const int g_rpcScrSetPlayerWidescreen = 111;
// 0x000e6360
const int g_rpcScrSetVehicleTires = 98;
// R5 address to verify
const int g_rpcPlayerOpcode052C = 92;
// R5 address to verify
const int g_rpcPlayerOpcode03FD = 150;
// R5 address to verify
const int g_rpcScrPlayCrimeReport = 112;
// 0x000e635c
const int g_rpcScrSetPlayerMapIcon = 56;
// R5 address to verify
const int g_rpcScrRemovePlayerMapIcon = 144;
// R5 address to verify
const int g_rpcScrSetPlayerAmmo = 145;
// R5 address to verify
const int g_rpcScrSetGravity = 146;
// R5 address to verify
const int g_rpcScrSetVehicleHealth = 147;
// R5 address to verify
const int g_rpcScrAttachTrailerToVehicle = 148;
// R5 address to verify
const int g_rpcScrDetachTrailerFromVehicle = 149;
// R5 address to verify
const int g_rpcScrCreateObject = 44;
// R5 address to verify
const int g_rpcScrSetObjectPosition = 45;
// R5 address to verify
const int g_rpcScrSetObjectRotation = 46;
// R5 address to verify
const int g_rpcScrDestroyObject = 47;
// R5 address to verify
const int g_rpcScrCreateExplosion = 79;
// R5 address to verify
const int g_rpcScrShowPlayerNametagForPlayer = 80;
// R5 address to verify
const int g_rpcScrMoveObject = 99;
// R5 address to verify
const int g_rpcScrStopObject = 122;
// R5 address to verify
const int g_rpcScrSetNumberPlate = 123;
// R5 address to verify
const int g_rpcScrTogglePlayerSpectating = 124;
// R5 address to verify
const int g_rpcUnknown001183A0 = 125;
// R5 address to verify
const int g_rpcScrPlayerSpectatePlayer = 126;
// R5 address to verify
const int g_rpcScrPlayerSpectateVehicle = 127;
// 0x000e6310
const int g_rpcScrMoveVehicleComponent = 57;
// 0x000e630c
const int g_rpcScrSetPlayerShopName = 33;
// 0x000e6308
const int g_rpcScrSetPlayerDrunkLevel = 35;
// 0x000e6304
const int g_rpcScrSetPlayerArmedWeapon = 67;
// R5 address to verify
const int g_rpcScrSetPlayerAttachedObject = 113;
// R5 address to verify
const int g_rpcScrPlayAudioStream = 41;
// R5 address to verify
const int g_rpcScrStopAudioStream = 42;
// 0x000e62f4
const int g_rpcScrRemoveBuildingForPlayer = 43;
// 0x000e62f0
const int g_rpcScrSetSpawnInfo = 68;
// 0x000e62ec
const int g_rpcScrSetPlayerTeam = 69;
// R5 address to verify
const int g_rpcScrSetPlayerName = 11;
// R5 address to verify
const int g_rpcScrSetPlayerSkin = 153;
// 0x000e62d4
const int g_rpcScrSetPlayerPosition = 12;
// 0x000e62d0
const int g_rpcScrSetPlayerPositionFindZ = 13;
// 0x000e62cc
const int g_rpcScrSetPlayerHealth = 14;
// R5 address to verify
const int g_rpcScrPutPlayerInVehicle = 70;
// 0x000e62d0
const int g_rpcScrRemovePlayerFromVehicle = 71;
// R5 address to verify
const int g_rpcScrSetPlayerColor = 72;
// 0x000e62c8
const int g_rpcScrDisplayGametext = 73;
// 0x000e62b8
const int g_rpcScrSetPlayerInterior = 156;
// 0x000e62b4
const int g_rpcScrSetPlayerCameraPosition = 157;
// 0x000e62b0
const int g_rpcScrSetPlayerCameraLookAt = 158;
// 0x000e62ac
const int g_rpcScrSetVehiclePosition = 159;
// 0x000e62a8
const int g_rpcScrSetVehicleZAngle = 160;
// 0x000e62a4
const int g_rpcScrSetVehicleParamsForPlayer = 161;
// 0x000e62a0
const int g_rpcScrSetCameraBehindPlayer = 162;
// 0x000e629c
const int g_rpcScrTogglePlayerControllable = 15;
// 0x000e6298
const int g_rpcScrPlaySound = 16;
// 0x000e6294
const int g_rpcScrSetPlayerWorldBounds = 17;
// 0x000e6290
const int g_rpcScrGivePlayerMoney = 18;
// 0x000e628c
const int g_rpcScrSetPlayerFacingAngle = 19;
// 0x000e6288
const int g_rpcScrResetPlayerMoney = 20;
// 0x000e6284
const int g_rpcScrResetPlayerWeapons = 21;
// R5 address to verify
const int g_rpcScrGivePlayerWeapon = 22;
// R5 address to verify
const int g_rpcUnknown0011830C = 64;
// 0x000e6284
const int g_rpcScrLinkVehicleToInterior = 65;
// 0x000e6280
const int g_rpcScrDeathMessage = 55;
// R5 address to verify
const int g_rpcScrAttachCameraToObject = 81;
// R5 address to verify
const int g_rpcScrInterpolateCamera = 82;
// 0x000e6264
const int g_rpcScrSetObjectMaterial = 84;
// 0x000e6260
const int g_rpcScrStopObjectCameraCollision = 169;
// 0x000e625c
const int g_rpcScrSetActorAnimation = 173;
// 0x000e6258
const int g_rpcScrClearActorAnimations = 174;
// 0x000e6254
const int g_rpcScrSetActorRotation = 175;
// 0x000e6250
const int g_rpcScrSetActorPosition = 176;
// 0x000e624c
const int g_rpcScrSetActorHealth = 178;
// R5 address to verify
const int g_rpcSetCustomModelVirtualWorld = 48;
// R5 address to verify
const int g_rpcScrSetPlayerArmor = 66;

// 0x0001b070
void ObjectReadMaterials(BYTE count, CObject* object, RakNet::BitStream* stream)
{
    DWORD color = 0;
    BYTE length = 0, type, index, size, fontSize, bold, alignment;
    WORD model;
    DWORD fontColor, backgroundColor;
    char dictionary[257], texture[257], font[257], text[2049];
    for (unsigned left = count; left; --left)
    {
        memset(dictionary, 0, sizeof(dictionary));
        memset(texture, 0, sizeof(texture));
        memset(font, 0, sizeof(font));
        memset(text, 0, sizeof(text));
        stream->Read(type);
        if (type == 1)
        {
            stream->Read(index);
            stream->Read(model);
            stream->Read(length);
            stream->Read(dictionary, length);
            stream->Read(length);
            stream->Read(texture, length);
            stream->Read(color);
            if (strlen(dictionary) < 32 && strlen(texture) < 32)
            {
                int materialModel = model;
                if (model == 0xffff || model > 20000)
                    materialModel = -1;
                if (object)
                    object->SetMaterial(materialModel, index, dictionary, texture, color);
            }
        }
        else if (type == 2)
        {
            stream->Read(index);
            stream->Read(size);
            stream->Read(length);
            stream->Read(font, length);
            stream->Read(fontSize);
            stream->Read(bold);
            stream->Read(fontColor);
            stream->Read(backgroundColor);
            stream->Read(alignment);
            StringCompressor::Instance()->DecodeString(text, 2048, stream, 0);
            if (strlen(font) < 32 && strlen(text) && object)
                object->SetMaterialText(index, text, size, font, fontSize, bold, fontColor, backgroundColor, alignment);
        }
    }
}

// 0x0001b340
void ScrCreateObject(RPCParameters* parameters)
{
    WORD id = 0xffff, vehicle = 0xffff, attachedObject = 0xffff;
    int model = -1;
    CVector position = {0, 0, 0}, rotation = {0, 0, 0}, offset = {0, 0, 0}, attachedRotation = {0, 0, 0};
    float distance = 0;
    BYTE noCameraCollision = 0, syncRotation = 0, materials = 0;
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    stream.Read(id);
    stream.Read(model);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    stream.Read(rotation.x);
    stream.Read(rotation.y);
    stream.Read(rotation.z);
    stream.Read(distance);
    stream.Read(noCameraCollision);
    stream.Read(vehicle);
    stream.Read(attachedObject);
    if (vehicle != 0xffff || attachedObject != 0xffff)
    {
        stream.Read(offset.x);
        stream.Read(offset.y);
        stream.Read(offset.z);
        stream.Read(attachedRotation.x);
        stream.Read(attachedRotation.y);
        stream.Read(attachedRotation.z);
        stream.Read(syncRotation);
    }
    stream.Read(materials);
    CObjectPool* pool = ((CNetGame*)g_pNetGame)->pools->objects;
    pool->Create(id, model, position, rotation, distance);
    CObject* object = (CObject*)pool->Get(id);
    if (vehicle != 0xffff)
    {
        if (object)
            object->SetAttachedVehicle(vehicle, &offset, &attachedRotation);
    }
    else if (attachedObject != 0xffff && object)
        object->SetAttachedObject(attachedObject, &offset, &attachedRotation, syncRotation);
    if (materials)
        ObjectReadMaterials(materials, object, &stream);
    if (object && noCameraCollision)
        ((BYTE*)object)[0x53] = 1;
}

// 0x0001b6a0
void ScrSetObjectMaterial(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CObjectPool* pool = ((CNetGame*)g_pNetGame)->pools->objects;
    WORD id;
    stream.Read(id);
    CObject* object = (CObject*)pool->Get(id);
    char dictionary[257] = {0}, texture[257] = {0}, font[257] = {0}, text[2049] = {0};
    BYTE length = 0, type, index, size, fontSize, bold, alignment;
    DWORD color = 0, fontColor, backgroundColor;
    WORD model;
    stream.Read(type);
    if (type == 1)
    {
        stream.Read(index);
        stream.Read(model);
        stream.Read(length);
        stream.Read(dictionary, length);
        stream.Read(length);
        stream.Read(texture, length);
        stream.Read(color);
        if (strlen(dictionary) < 32 && strlen(texture) < 32)
        {
            int materialModel = model;
            if (model == 0xffff)
                materialModel = -1;
            if (object)
                object->SetMaterial(materialModel, index, dictionary, texture, color);
        }
    }
    else if (type == 2)
    {
        stream.Read(index);
        stream.Read(size);
        stream.Read(length);
        stream.Read(font, length);
        stream.Read(fontSize);
        stream.Read(bold);
        stream.Read(fontColor);
        stream.Read(backgroundColor);
        stream.Read(alignment);
        StringCompressor::Instance()->DecodeString(text, 2048, &stream, 0);
        if (strlen(font) < 32 && strlen(text) && object)
            object->SetMaterialText(index, text, size, font, fontSize, bold, fontColor, backgroundColor, alignment);
    }
}

// 0x0001c6a0
void ScrAttachObjectToPlayer(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id, player;
    CVector offset, rotation;
    stream.Read(id);
    stream.Read(player);
    stream.Read(offset.x);
    stream.Read(offset.y);
    stream.Read(offset.z);
    stream.Read(rotation.x);
    stream.Read(rotation.y);
    stream.Read(rotation.z);
    CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
    CObject* object = (CObject*)pools->objects->Get(id);
    if (!object)
        return;
    CPlayerPool* players = pools->players;
    CPed* ped;
    if (player == players->localId)
        ped = players->local->m_state.ped;
    else
    {
        if (player > 1004 || !players->players[player] || !players->players[player]->remote)
            return;
        ped = players->players[player]->remote->m_state.ped;
    }
    Scripting::ProcessCommand(&g_rpcAttachObjectToPlayerCommand, *(DWORD*)((BYTE*)object + 0x44), *(DWORD*)((BYTE*)ped + 0x44), (double)offset.x, (double)offset.y,
                              (double)offset.z, (double)rotation.x, (double)rotation.y, (double)rotation.z);
}

// 0x00018f00
void ScrSetPlayerAttachedObject(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    PedAccessoryState info = {0};
    WORD player;
    int index;
    bool create;
    stream.Read(player);
    stream.Read(index);
    stream.Read(create);
    if (create)
        stream.Read((char*)&info, 52);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    if (!pool)
        return;
    CPed* ped;
    if (player == pool->localId)
        ped = pool->local->m_state.ped;
    else
    {
        if (player >= 1004 || !pool->connected[player])
            return;
        CRemotePlayer* remote = pool->players[player] ? pool->players[player]->remote : 0;
        ped = remote->m_state.ped;
    }
    if (ped)
    {
        if (create)
            ped->AddAccessory(index, &info);
        else
            ped->DeleteAccessory(index);
    }
}

// 0x0001dcc0
void ScrSetCustomModelVirtualWorld(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    DWORD value = 0;
    stream.Read(value);
    BYTE* models = (BYTE*)g_customModels;
    if (models && *(DWORD*)(models + 0x217) != value)
    {
        *(DWORD*)(models + 0x217) = value;
        models[0x221] = 0;
        models[0x220] = 0;
    }
}

// 0x0001dfd0
void ScrSetPlayerName(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    WORD id;
    BYTE length, status;
    char name[25];
    stream.Read(id);
    stream.Read(length);
    if (length < 25)
    {
        stream.Read(name, length);
        stream.Read(status);
        name[length] = 0;
        if (status == 1)
            pool->SetName(id, name);
        if (pool->localId == id)
            pool->localName.assign(name, strlen(name));
    }
}

// 0x00018db0
void ScrSetPlayerDrunkLevel(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    DWORD level;
    stream.Read(level);
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped)
        ped->SetDrunkLevel(level);
}

// 0x00018e50
void ScrSetPlayerArmedWeapon(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped)
    {
        int weapon;
        stream.Read(weapon);
        if (weapon >= 0 && weapon < 47)
            ped->SetArmedWeapon(weapon, 0);
    }
}

// 0x00019ff0
void ScrAttachCameraToObject(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CObjectPool* pool = ((CNetGame*)g_pNetGame)->pools->objects;
    if (id <= 2100 && pool->active[id])
    {
        CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
        local->m_state.spectateProcessed = 1;
        CEntity* object = ((CNetGame*)g_pNetGame)->pools->objects->Get(id);
        g_pGame->m_pCamera->Attach(object);
    }
}

// 0x0001a0f0
void ScrInterpolateCamera(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    bool move = true;
    CVector from, to;
    int time;
    BYTE mode;
    stream.Read(move);
    stream.Read(from.x);
    stream.Read(from.y);
    stream.Read(from.z);
    stream.Read(to.x);
    stream.Read(to.y);
    stream.Read(to.z);
    stream.Read(time);
    stream.Read(mode);
    if (mode < 1 || mode > 2)
        mode = 2;
    if (time > 0)
    {
        CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
        local->m_state.spectateProcessed = 1;
        if (move)
            g_pGame->m_pCamera->SetMoveVector(&from, &to, time, mode);
        else
            g_pGame->m_pCamera->SetTrackVector(&from, &to, time, mode);
    }
}

// 0x0001d650
void ClickTextDraw(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    bool enabled = false;
    DWORD color = 0;
    stream.Read(enabled);
    stream.Read(color);
    if (g_renderObjectSelection && g_renderObjectSelection->enabled)
        g_renderObjectSelection->Enable(FALSE);
    CObjectEdit* edit = (CObjectEdit*)g_deviceObjectEdit;
    if (edit && edit->editing)
        edit->Disable(FALSE);
    if (g_renderTextDrawSelection)
    {
        if (enabled)
            g_renderTextDrawSelection->Enable(color);
        else
            g_renderTextDrawSelection->Disable();
    }
}

// 0x0001c170
void ScrStopObjectCameraCollision(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CEntity* object = ((CNetGame*)g_pNetGame)->pools->objects->Get(id);
    if (object)
        ((BYTE*)object)[0x53] = 1;
}

// 0x0001d530
void ScrRemoveBuildingForPlayer(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    int model = 0;
    CVector position = {0, 0, 0};
    float radius = 0;
    stream.Read(model);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    stream.Read(radius);
    GameRemoveBuilding(model, position.x, position.y, position.z, radius);
}

// 0x00019050
void ScrPlayCrimeReport(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    WORD unused;
    BOOL useVehicle = 0;
    int model = 0;
    DWORD color = 0, first = 0;
    CVector position;
    stream.Read(unused);
    stream.Read(useVehicle);
    stream.Read(model);
    stream.Read(color);
    stream.Read(first);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    // Native passes a pointer to the three-word vector, not its first DWORD.
    g_pGame->ApplyPreviewAnimation(first, (DWORD)&position, useVehicle, model, color);
}

// R5 address to verify
void ScrPlayerOpcode052C(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    DWORD value;
    stream.Read(value);
    if (g_pGame->GetPlayerPed())
        Scripting::ProcessCommand(&g_rpcPlayerOpcode052cCommand, 0, value);
}

// R5 address to verify
void ScrPlayerOpcode03FD(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    DWORD value;
    stream.Read(value);
    if (g_pGame->GetPlayerPed())
        Scripting::ProcessCommand(&g_rpcPlayerOpcode03fdCommand, 0, value);
}

// The original returns immediately.
// R5 address to verify
void ScrRpc125(RPCParameters* parameters)
{
    (void)parameters;
}

// 0x0001a700
void ScrRpc64(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    WORD ignored;
    stream.Read(ignored);
}

// 0x00017f50
void ScrSetSpawnInfo(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    BYTE info[LocalSpawnInfoWireSize] = {0};
    stream.Read((char*)info, sizeof(info));
    pool->local->SetSpawnInfo(info);
}

// 0x00018740
void ScrSetPlayerFightingStyle(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    BYTE style = 0;
    stream.Read(id);
    stream.Read(style);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    if (pool)
    {
        CPed* ped;
        if (id == pool->localId)
            ped = pool->local->m_state.ped;
        else
        {
            if (id >= 1004 || !pool->connected[id])
                return;
            CRemotePlayer* remote = pool->GetPlayer(id);
            ped = remote->m_state.ped;
        }
        if (ped)
            ped->SetFightingStyle(style);
    }
}

// 0x0001ac10
void ScrSetPlayerAmmo(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    BYTE weapon;
    WORD ammo;
    stream.Read(weapon);
    stream.Read(ammo);
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.ped->SetWeaponAmmo(weapon, ammo);
}

// 0x00019710
void ScrSetPlayerTeam(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    WORD id;
    BYTE team;
    stream.Read(id);
    stream.Read(team);
    if (id == pool->localId)
        pool->local->m_state.team = team;
    else if (id <= 1004)
    {
        CPlayerInfo* info = pool->players[id];
        if (info && info->remote)
            info->remote->m_state.team = team;
    }
}

// 0x00019190
void ScrSetPlayerSkin(RPCParameters* parameters)
{
    int model = 0;
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    if (!pool)
        return;
    DWORD id;
    stream.Read(id);
    stream.Read(model);
    if (!GameIsPedModel(model))
    {
        if (g_pChat)
            g_pChat->AddInfoMessage(g_rpcInvalidSkinWarning, model);
        return;
    }
    if (id == pool->localId)
        pool->local->m_state.ped->SetModelIndex(model);
    else if ((WORD)id < 1004 && pool->connected[(WORD)id])
    {
        CRemotePlayer* remote = pool->GetPlayer((WORD)id);
        if (remote->m_state.ped && remote->m_state.playerState)
        {
            remote = pool->GetPlayer((WORD)id);
            CPed* ped = remote->m_state.ped;
            if (ped)
                ped->SetModelIndex(model);
        }
    }
}

// 0x0001a950
void ScrApplyAnimation(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    char library[256] = {0}, name[256] = {0};
    WORD id;
    BYTE libraryLength, nameLength;
    float speed;
    bool loop, lockX, lockY, freeze;
    int time;
    stream.Read(id);
    stream.Read(libraryLength);
    stream.Read(library, libraryLength);
    stream.Read(nameLength);
    stream.Read(name, nameLength);
    stream.Read(speed);
    stream.Read(loop);
    stream.Read(lockX);
    stream.Read(lockY);
    stream.Read(freeze);
    stream.Read(time);
    library[libraryLength] = 0;
    name[nameLength] = 0;
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    if (pool)
    {
        CPed* ped;
        if (id == pool->localId)
            ped = pool->local->m_state.ped;
        else
        {
            if (id >= 1004 || !pool->connected[id] || !pool->players[id] || !pool->players[id]->remote)
                return;
            CRemotePlayer* remote = pool->players[id]->remote;
            ped = remote->m_state.ped;
            remote->m_state.scriptedAnimation = 1;
        }
        if (ped)
            ped->ApplyAnimation(name, library, speed, loop, lockX, lockY, freeze, time);
    }
}

// 0x00018580
void ScrClearAnimations(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    if (pool)
    {
        CPed* ped;
        if (id == pool->localId)
            ped = pool->local->m_state.ped;
        else
        {
            if (id >= 1004 || !pool->connected[id] || !pool->players[id] || !pool->players[id]->remote)
                return;
            CRemotePlayer* remote = pool->players[id]->remote;
            ped = remote->m_state.ped;
            remote->m_state.scriptedAnimation = 0;
        }
        if (ped)
        {
            CMatrix matrix;
            ped->GetMatrix(&matrix);
            ped->Teleport(matrix.position);
        }
    }
}

// 0x0001c5c0
void ScrMoveVehicleComponent(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CVehiclePool* pool = ((CNetGame*)g_pNetGame)->pools->vehicles;
    if (pool)
    {
        WORD id, component;
        stream.Read(id);
        stream.Read(component);
        if (id < 2000 && pool->m_bNotEmpty[id])
            pool->m_pObjects[id]->RemoveComponent(component);
    }
}

// 0x00018ac0
void ScrSetPlayerWidescreen(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    BYTE enabled;
    stream.Read(enabled);
    g_pChat->AddDebugMessage(g_rpcWidescreenMessage, (int)enabled);
    Scripting::ProcessCommand(&g_rpcWidescreenCommand, (int)enabled);
}

// R5 address to verify
void ScrSetVehicleTires(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    BYTE tires;
    stream.Read(id);
    stream.Read(tires);
    CVehiclePool* pool = ((CNetGame*)g_pNetGame)->pools->vehicles;
    CVehicle* vehicle = pool->Get(id);
    if (pool && vehicle)
        vehicle->SetTires(tires);
}

// 0x0001ba10
void ScrSetObjectPosition(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    CVector position;
    float unusedRotation;
    stream.Read(id);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    stream.Read(unusedRotation);
    CEntity* object = ((CNetGame*)g_pNetGame)->pools->objects->Get(id);
    if (object)
        object->Teleport(position);
}

// 0x0001bb20
void ScrSetObjectRotation(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    CVector rotation;
    stream.Read(id);
    stream.Read(rotation.x);
    stream.Read(rotation.y);
    stream.Read(rotation.z);
    CObjectPool* pool = ((CNetGame*)g_pNetGame)->pools->objects;
    if (pool)
    {
        CObject* object = (CObject*)pool->Get(id);
        if (object)
            object->SetRotation(&rotation);
    }
}

// 0x0001bc20
void ScrDestroyObject(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CObjectPool* pool = ((CNetGame*)g_pNetGame)->pools->objects;
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    CEntity* object = pool->Get(id);
    if (object)
    {
        if (local && local->m_state.surfingEntity == object)
        {
            local->m_state.surfingEntity = 0;
            local->m_state.surfingMode = 0;
        }
        pool->Delete(id);
    }
}

// 0x0001bf00
void ScrMoveObject(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    CVector origin, position, rotation;
    float speed;
    stream.Read(id);
    stream.Read(origin.x);
    stream.Read(origin.y);
    stream.Read(origin.z);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    stream.Read(speed);
    stream.Read(rotation.x);
    stream.Read(rotation.y);
    stream.Read(rotation.z);
    CObjectEdit* edit = (CObjectEdit*)g_deviceObjectEdit;
    if (edit && edit->editing && edit->objectId == id)
        return;
    CObject* object = (CObject*)((CNetGame*)g_pNetGame)->pools->objects->Get(id);
    if (object)
        object->Move(position, speed, rotation);
}

// 0x0001c0b0
void ScrStopObject(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CObject* object = (CObject*)((CNetGame*)g_pNetGame)->pools->objects->Get(id);
    if (object)
        object->Stop();
}

// 0x0001d750
void ScrSetActorAnimation(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CActorPool* pool = ((CNetGame*)g_pNetGame)->pools->actors;
    if (pool)
    {
        char library[256] = {0}, name[256] = {0};
        BYTE libraryLength, nameLength;
        WORD id;
        float speed;
        bool loop, lockX, lockY, freeze;
        int time;
        stream.Read(id);
        stream.Read(libraryLength);
        stream.Read(library, libraryLength);
        stream.Read(nameLength);
        stream.Read(name, nameLength);
        stream.Read(speed);
        stream.Read(loop);
        stream.Read(lockX);
        stream.Read(lockY);
        stream.Read(freeze);
        stream.Read(time);
        library[libraryLength] = 0;
        name[nameLength] = 0;
        CActor* actor = pool->Get(id);
        if (actor)
            actor->PerformAnimation(name, library, speed, loop, lockX, lockY, freeze, time);
    }
}

// R5 address to verify
void ScrClearActorAnimations(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CActorPool* pool = ((CNetGame*)g_pNetGame)->pools->actors;
    if (pool)
    {
        CActor* actor = pool->Get(id);
        if (actor)
            actor->ClearTasks();
    }
}

// 0x0001d9f0
void ScrSetActorRotation(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    float angle;
    stream.Read(id);
    stream.Read(angle);
    CActorPool* pool = ((CNetGame*)g_pNetGame)->pools->actors;
    if (pool)
    {
        CActor* actor = pool->Get(id);
        if (actor)
            actor->SetRotation(angle);
    }
}

// 0x0001dad0
void ScrSetActorPosition(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    CVector position;
    stream.Read(id);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    CActorPool* pool = ((CNetGame*)g_pNetGame)->pools->actors;
    if (pool)
    {
        CActor* actor = pool->Get(id);
        if (actor)
            actor->Teleport(position);
    }
}

// 0x0001dbe0
void ScrSetActorHealth(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    float health;
    stream.Read(id);
    stream.Read(health);
    CActorPool* pool = ((CNetGame*)g_pNetGame)->pools->actors;
    if (pool)
    {
        CActor* actor = pool->Get(id);
        if (actor)
            actor->SetHealth(health);
    }
}

// 0x0001a790
void ScrSetPlayerMapIcon(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    BYTE id, icon, type;
    CVector position;
    DWORD color;
    stream.Read(id);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    stream.Read(icon);
    stream.Read(color);
    stream.Read(type);
    ((CNetGame*)g_pNetGame)->CreateMapIcon(id, position, icon, color, type);
}

// 0x0001a8b0
void ScrRemovePlayerMapIcon(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    BYTE id;
    stream.Read(id);
    ((CNetGame*)g_pNetGame)->RemoveMapIcon(id);
}

// 0x0001bd10
void ScrCreateExplosion(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CVector position;
    int type;
    float radius;
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    stream.Read(type);
    stream.Read(radius);
    // Preserve native descriptor "fffii" despite its double-promoted radius argument.
    Scripting::ProcessCommand(&g_rpcExplosionCommand, (double)position.x, (double)position.y, (double)position.z, type, (double)radius);
}

// 0x0001be20
void ScrShowPlayerNametagForPlayer(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    BYTE show;
    stream.Read(id);
    stream.Read(show);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    if (id < 1004 && pool->connected[id])
    {
        CRemotePlayer* remote = pool->GetPlayer(id);
        remote->m_state.showNameTag = show != 0;
    }
}

// 0x00019c70
void ScrSetVehiclePosition(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    CVector position;
    stream.Read(id);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    if (g_pNetGame)
    {
        CVehiclePool* pool = ((CNetGame*)g_pNetGame)->pools->vehicles;
        if (pool && id < 2000 && pool->m_bNotEmpty[id])
            pool->m_pObjects[id]->Teleport(position);
    }
}

// 0x00019d80
void ScrSetVehicleZAngle(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    WORD id;
    float angle;
    stream.Read(id);
    stream.Read(angle);
    CVehiclePool* pool = ((CNetGame*)g_pNetGame)->pools->vehicles;
    if (pool && id < 2000 && pool->m_bNotEmpty[id])
        pool->m_pObjects[id]->SetRotation(angle);
}

// 0x00019e60
void ScrSetVehicleParamsForPlayer(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    WORD id;
    BYTE objective, locked;
    stream.Read(id);
    stream.Read(objective);
    stream.Read(locked);
    CVehiclePool* pool = ((CNetGame*)g_pNetGame)->pools->vehicles;
    if (pool)
        pool->AssignSpecialParams(id, objective, locked);
}

// 0x00019f30
void ScrLinkVehicleToInterior(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    BYTE interior;
    stream.Read(id);
    stream.Read(interior);
    ((CNetGame*)g_pNetGame)->pools->vehicles->ChangeInterior(id, interior);
}

// 0x0001ad70
void ScrSetVehicleHealth(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    float health;
    stream.Read(id);
    stream.Read(health);
    CVehiclePool* pool = ((CNetGame*)g_pNetGame)->pools->vehicles;
    if (pool && id < 2000 && pool->m_bNotEmpty[id])
    {
        CVehicle* vehicle = pool->Get(id);
        if (vehicle)
            vehicle->SetHealth(health);
    }
}

// 0x0001ae50
void ScrAttachTrailerToVehicle(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    if (!((CNetGame*)g_pNetGame)->pools->vehicles)
        return;
    WORD trailerId, tractorId;
    stream.Read(trailerId);
    stream.Read(tractorId);
    CVehiclePool* pool = ((CNetGame*)g_pNetGame)->pools->vehicles;
    CVehicle *trailer = pool->Get(trailerId), *tractor = pool->Get(tractorId);
    if (trailer && tractor)
    {
        tractor->SetTrailer(trailer);
        tractor->AttachTrailer();
    }
}

// 0x0001af90
void ScrDetachTrailerFromVehicle(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    unsigned bits = parameters->numberOfBitsOfData;
    if (!((CNetGame*)g_pNetGame)->pools->vehicles)
        return;
    RakNet::BitStream stream(input, (int)bits / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CVehicle* vehicle = ((CNetGame*)g_pNetGame)->pools->vehicles->Get(id);
    if (vehicle)
    {
        vehicle->DetachTrailer();
        vehicle->SetTrailer(0);
    }
}

// 0x00018950
void ScrSetVehicleVelocity(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    BYTE type = 0;
    CVector velocity;
    stream.Read(type);
    stream.Read(velocity.x);
    stream.Read(velocity.y);
    stream.Read(velocity.z);
    CVehiclePool* pool = ((CNetGame*)g_pNetGame)->pools->vehicles;
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped && ped->IsInVehicle())
    {
        WORD id = pool->Find(ped->GetVehicle());
        if (id != 0xffff)
        {
            CVehicle* vehicle = pool->Get(id);
            if (vehicle)
            {
                if (type == 0)
                    vehicle->SetSpeed(velocity);
                else if (type == 1)
                    vehicle->SetTurnSpeed(velocity);
            }
        }
    }
}

// 0x0001c230
void ScrSetNumberPlate(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    char text[33] = {0};
    BYTE length = 0;
    WORD id;
    stream.Read(id);
    stream.Read(length);
    if (length <= 32)
    {
        stream.Read(text, length);
        CVehiclePool* pool = ((CNetGame*)g_pNetGame)->pools->vehicles;
        if (pool && id < 2000 && pool->m_bNotEmpty[id])
            pool->m_pObjects[id]->SetLicensePlateText(text);
    }
}

// 0x0001c350
void ScrTogglePlayerSpectating(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    BOOL enabled;
    stream.Read(enabled);
    ((CNetGame*)g_pNetGame)->pools->players->local->ToggleSpectating(enabled);
}

// 0x0001c400
void ScrPlayerSpectatePlayer(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    BYTE mode;
    stream.Read(id);
    stream.Read(mode);
    mode = mode == 2 ? 15 : mode == 3 ? 14 : 4;
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.spectateCameraMode = mode;
    local->SpectatePlayer(id);
}

// 0x0001c4e0
void ScrPlayerSpectateVehicle(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    BYTE mode;
    stream.Read(id);
    stream.Read(mode);
    mode = mode == 2 ? 15 : mode == 3 ? 14 : 3;
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.spectateCameraMode = mode;
    local->SpectateVehicle(id);
}

// 0x00019600
void ScrPutPlayerInVehicle(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    BYTE seat;
    stream.Read(id);
    stream.Read(seat);
    DWORD ref = ((CNetGame*)g_pNetGame)->pools->vehicles->GetRef((int)id);
    CVehicle* vehicle = ((CNetGame*)g_pNetGame)->pools->vehicles->Get(id);
    if (ref && vehicle)
        g_pGame->GetPlayerPed()->PutIntoVehicle(ref, seat);
}

// 0x00017ff0
void ScrRemovePlayerFromVehicle(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.ped->ExitVehicle();
}

// 0x000198f0
void ScrDisplayGametext(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    char text[256] = {0};
    int style, time, length;
    stream.Read(style);
    stream.Read(time);
    stream.Read(length);
    if (length > 0 && length < 201)
    {
        stream.Read(text, length);
        text[length] = 0;
        g_pGame->DisplayGameText(text, time, style);
    }
}

// 0x00017e50
void ScrSetPlayerShopName(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    char name[33] = {0};
    stream.Read(name, 32);
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped)
    {
        if (strlen(name))
        {
            ped->SetBackendShoppingName(name);
            ped->LoadShoppingDataSubsection(name);
        }
        else
        {
            ped->SetBackendShoppingName(0);
            ped->LoadShoppingDataSubsection("");
        }
    }
}

// 0x00019800
void ScrSetPlayerColor(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    WORD player;
    DWORD color;
    stream.Read(player);
    stream.Read(color);
    if (player == pool->localId)
        pool->local->SetColor(color);
    // The original also accepts player ID 1004.
    else if (player <= 1004)
    {
        CPlayerInfo* info = pool->players[player];
        if (info && info->remote)
            info->remote->SetColor(color);
    }
}

// 0x0001dd70
void ScrDeathMessage(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    WORD killer, victim;
    BYTE weapon;
    stream.Read(killer);
    stream.Read(victim);
    stream.Read(weapon);
    if (victim == 0xffff)
        return;
    const char *killerName = 0, *victimName = 0;
    DWORD killerColor = 0, victimColor = 0;
    if (killer != 0xffff)
    {
        if (killer == pool->localId)
        {
            killerName = pool->localName.c_str();
            killerColor = pool->local->GetColorAsARGB();
        }
        else if (killer < 1004 && pool->connected[killer])
        {
            killerName = pool->GetName(killer);
            killerColor = pool->GetPlayer(killer)->GetColorAsARGB();
        }
    }
    if (victim == pool->localId)
    {
        victimName = pool->localName.c_str();
        victimColor = pool->local->GetColorAsARGB();
    }
    else if (victim < 1004 && pool->connected[victim])
    {
        victimName = pool->GetName(victim);
        CPlayerInfo* info = pool->players[victim];
        CRemotePlayer* remote = info ? info->remote : 0;
        victimColor = remote->GetColorAsARGB();
    }
    if (g_deviceDeathWindow)
        ((CDeathWindow*)g_deviceDeathWindow)->AddMessage(killerName, victimName, killerColor, victimColor, weapon);
}

// 0x0001c870
void ScrInitMenu(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    if (!g_pNetGame)
        return;
    CMenuPool* pool = ((CNetGame*)g_pNetGame)->pools->menus;
    BYTE id;
    int twoColumns;
    char text[32];
    float x, y, width1, width2 = 0;
    DWORD interaction[13];
    stream.Read(id);
    stream.Read(twoColumns);
    stream.Read(text, 32);
    stream.Read(x);
    stream.Read(y);
    stream.Read(width1);
    if (twoColumns)
        stream.Read(width2);
    stream.Read(interaction[0]);
    for (int i = 1; i < 13; i++)
        stream.Read(interaction[i]);
    // The original also checks menu ID 128.
    if (id <= 128 && pool->active[id])
        pool->Delete(id);
    CMenu* menu = pool->Create(id, text, x, y, (BYTE)((BYTE)twoColumns + 1), width1, width2, interaction);
    if (menu)
    {
        stream.Read(text, 32);
        menu->SetColumnTitle(0, text);
        BYTE count;
        stream.Read(count);
        for (BYTE row = 0; row < count; row++)
        {
            stream.Read(text, 32);
            menu->AddItem(0, row, text);
        }
        if (twoColumns)
        {
            stream.Read(text, 32);
            menu->SetColumnTitle(1, text);
            stream.Read(count);
            for (BYTE row = 0; row < count; row++)
            {
                stream.Read(text, 32);
                menu->AddItem(1, row, text);
            }
        }
    }
}

// 0x0001cb90
void ScrShowMenu(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    if (g_pNetGame)
    {
        BYTE id;
        stream.Read(id);
        ((CNetGame*)g_pNetGame)->pools->menus->Show(id);
    }
}

// 0x0001cc40
void ScrHideMenu(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    if (g_pNetGame)
    {
        BYTE id;
        stream.Read(id);
        ((CNetGame*)g_pNetGame)->pools->menus->Hide(id);
    }
}

// 0x0001cd90
void ScrShowTextDraw(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD length = 0;
    CTextDrawPool* pool = ((CNetGame*)g_pNetGame)->pools->textDraws;
    if (pool)
    {
        WORD id;
        TextDrawTransmit info;
        char text[800];
        stream.Read(id);
        stream.Read((char*)&info, 63);
        stream.Read(length);
        if (length < 800)
        {
            stream.Read(text, length);
            text[length] = 0;
            pool->Create(id, &info, text);
        }
        else if (g_pChat)
            g_pChat->AddInfoMessage(g_rpcLargeTextDrawWarning, (unsigned)length);
    }
}

// 0x0001cec0
void ScrHideTextDrawForPlayer(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    CTextDrawPool* pool = ((CNetGame*)g_pNetGame)->pools->textDraws;
    if (pool)
    {
        WORD id;
        stream.Read(id);
        pool->Delete(id);
    }
}

// 0x0001cf70
void ScrTextDrawSetString(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    WORD length = 0;
    CTextDrawPool* pool = ((CNetGame*)g_pNetGame)->pools->textDraws;
    if (pool)
    {
        WORD id;
        char text[800];
        stream.Read(id);
        stream.Read(length);
        // Native accepts800 and writes its terminator one byte beyond this buffer.
        if (length <= 800)
        {
            stream.Read(text, length);
            text[length] = 0;
            if (id < 2304 && pool->active[id] && pool->draws[id])
                pool->draws[id]->SetText(text);
        }
    }
}

// 0x0001a330
void ScrPlaySound(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    int sound;
    CVector position;
    stream.Read(sound);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    g_pGame->m_pAudio->Play(sound, position);
}

// 0x0001d3c0
void ScrPlayAudioStream(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    char url[257] = {0};
    BYTE length = 0;
    CVector position = {0, 0, 0};
    float radius = 0;
    bool spatial = false;
    stream.Read(length);
    stream.Read(url, length);
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    stream.Read(radius);
    stream.ReadBits((BYTE*)&spatial, 8, true);
    if (g_renderAudioStream)
        g_renderAudioStream->Play(url, position.x, position.y, position.z, radius, spatial);
}

// 0x000180f0
void ScrStopAudioStream(RPCParameters* parameters)
{
    (void)parameters;
    if (g_renderAudioStream)
        g_renderAudioStream->Stop(false);
}

// 0x00019aa0
void ScrSetPlayerCameraPosition(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CVector position, rotation = {0, 0, 0};
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    g_pGame->m_pCamera->Set(position, rotation);
}

// 0x00019b70
void ScrSetPlayerCameraLookAt(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CVector position;
    BYTE style = 0;
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    stream.Read(style);
    if (style < 1 || style > 2)
        style = 2;
    g_pGame->m_pCamera->PointAt(position, style);
}

// 0x00018080
void ScrSetCameraBehindPlayer(RPCParameters* parameters)
{
    (void)parameters;
    g_pGame->m_pCamera->Restore();
}

// 0x00019320
void ScrSetPlayerPosition(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CPlayerPool* players = ((CNetGame*)g_pNetGame)->pools->players;
    if (players)
    {
        CLocalPlayer* local = players->local;
        if (local)
        {
            CVector position;
            stream.Read(position.x);
            stream.Read(position.y);
            stream.Read(position.z);
            local->m_state.surfingMode = 0;
            local->SetSurfingEntity(0, 0);
            local->m_state.ped->Teleport(position);
        }
    }
}

// 0x00019440
void ScrSetPlayerPositionFindZ(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    CVector position;
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    position.z = g_pGame->FindGroundZ(position.x, position.y, position.z) + 1.5f;
    local->m_state.surfingMode = 0;
    local->SetSurfingEntity(0, 0);
    local->m_state.ped->Teleport(position);
}

// 0x00018850
void ScrSetPlayerVelocity(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    CVector velocity;
    stream.Read(velocity.x);
    stream.Read(velocity.y);
    stream.Read(velocity.z);
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped)
    {
        if (ped->IsOnGround())
            ped->SetFlags(ped->GetFlags() ^ 3);
        ped->SetSpeed(velocity);
    }
}

// 0x0001a410
void ScrSetPlayerWorldBounds(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 5, 32, true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 9, 32, true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 0xd, 32, true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 0x11, 32, true);
}

// 0x0001a5a0
void ScrSetPlayerFacingAngle(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    float angle;
    stream.Read(angle);
    g_pGame->GetPlayerPed()->SetRotation(angle);
}

// 0x00017d50
void ScrEnableStuntBonusForPlayer(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    bool enabled;
    stream.Read(enabled);
    g_pGame->EnableStuntBonus((BYTE)enabled);
}

// 0x000180d0
void ScrForceClassSelection(RPCParameters* parameters)
{
    (void)parameters;
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.wantsAnotherClass = 1;
}

// 0x000180a0
void ScrResetPlayerWeapons(RPCParameters* parameters)
{
    (void)parameters;
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.ped->ClearWeapons();
}

// 0x0001a640
void ScrGivePlayerWeapon(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    int weapon, ammo;
    stream.Read(weapon);
    stream.Read(ammo);
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.ped->GiveWeapon(weapon, ammo);
}

// 0x0001d080
void ScrGangZoneCreate(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    CGangZonePool* zones = ((CNetGame*)g_pNetGame)->pools->gangZones;
    if (zones)
    {
        WORD id;
        float minX, minY, maxX, maxY;
        DWORD color;
        stream.Read(id);
        stream.Read(minX);
        stream.Read(minY);
        stream.Read(maxX);
        stream.Read(maxY);
        stream.Read(color);
        zones->Create(id, minX, minY, maxX, maxY, color);
    }
}

// 0x0001d1a0
void ScrGangZoneDestroy(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    CGangZonePool* zones = ((CNetGame*)g_pNetGame)->pools->gangZones;
    if (zones)
    {
        WORD id;
        stream.Read(id);
        zones->Delete(id);
    }
}

// 0x0001d250
void ScrGangZoneFlash(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    CGangZonePool* zones = ((CNetGame*)g_pNetGame)->pools->gangZones;
    if (zones)
    {
        WORD id;
        DWORD color;
        stream.Read(id);
        stream.Read(color);
        zones->StartFlashing(id, color);
    }
}

// 0x0001d310
void ScrGangZoneStopFlash(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    CGangZonePool* zones = ((CNetGame*)g_pNetGame)->pools->gangZones;
    if (zones)
    {
        WORD id;
        stream.Read(id);
        zones->StopFlashing(id);
    }
}

// 0x00017de0
void ScrDisableVehicleCollisions(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    bool disabled = false;
    stream.Read(disabled);
    if (g_pNetGame)
        ((CNetGame*)g_pNetGame)->disableVehicleCollisions = (BYTE)disabled;
}

// 0x0001acd0
void ScrSetGravity(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    float gravity;
    stream.Read(gravity);
    g_pGame->SetWorldGravity(gravity);
}

// 0x0001ccf0
void ScrSetPlayerWantedLevel(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    if (g_pGame)
    {
        BYTE level;
        stream.Read(level);
        g_pGame->SetWantedLevel(level);
    }
}

// 0x00019550
void ScrSetPlayerHealth(RPCParameters* parameters)
{
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    float health;
    stream.Read(health);
    local->m_state.ped->SetHealth(health);
}

// 0x0001ab60
void ScrSetPlayerArmor(RPCParameters* parameters)
{
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    float armour;
    stream.Read(armour);
    local->m_state.ped->SetArmour(armour);
}

// 0x00019a00
void ScrSetPlayerInterior(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    BYTE interior;
    stream.Read(interior);
    g_pGame->GetPlayerPed()->LinkToInterior(interior, TRUE);
}

// 0x0001a500
void ScrGivePlayerMoney(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    int amount;
    stream.Read(amount);
    g_pGame->IncreasePlayerMoney(amount);
}

// 0x00018090
void ScrResetPlayerMoney(RPCParameters* parameters)
{
    (void)parameters;
    g_pGame->ResetMoney();
}

// 0x00018690
void ScrSetPlayerSpecialAction(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    BYTE action;
    stream.Read(action);
    CPlayerPool* players = ((CNetGame*)g_pNetGame)->pools->players;
    if (players)
        players->local->SetSpecialAction(action);
}

// 0x0001a290
void ScrTogglePlayerControllable(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    BYTE enabled;
    stream.Read(enabled);
    g_pGame->GetPlayerPed()->SetControllable(enabled);
}
// 0x0001e130
void RegisterScriptRPCs(RakClientInterface* client)
{
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDisableVehicleCollisions), ScrDisableVehicleCollisions);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerMapIcon), ScrSetPlayerMapIcon);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrRemovePlayerMapIcon), ScrRemovePlayerMapIcon);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerAmmo), ScrSetPlayerAmmo);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetGravity), ScrSetGravity);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleHealth), ScrSetVehicleHealth);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrAttachTrailerToVehicle), ScrAttachTrailerToVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDetachTrailerFromVehicle), ScrDetachTrailerFromVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrCreateObject), ScrCreateObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetObjectPosition), ScrSetObjectPosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetObjectRotation), ScrSetObjectRotation);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDestroyObject), ScrDestroyObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrCreateExplosion), ScrCreateExplosion);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrShowPlayerNametagForPlayer), ScrShowPlayerNametagForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrMoveObject), ScrMoveObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrStopObject), ScrStopObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetNumberPlate), ScrSetNumberPlate);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrTogglePlayerSpectating), ScrTogglePlayerSpectating);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown001183A0), ScrRpc125);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlayerSpectatePlayer), ScrPlayerSpectatePlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlayerSpectateVehicle), ScrPlayerSpectateVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrMoveVehicleComponent), ScrMoveVehicleComponent);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrForceClassSelection), ScrForceClassSelection);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrAttachObjectToPlayer), ScrAttachObjectToPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrInitMenu), ScrInitMenu);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrShowMenu), ScrShowMenu);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrHideMenu), ScrHideMenu);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerWantedLevel), ScrSetPlayerWantedLevel);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrShowTextDraw), ScrShowTextDraw);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrHideTextDrawForPlayer), ScrHideTextDrawForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrTextDrawSetString), ScrTextDrawSetString);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGangZoneCreate), ScrGangZoneCreate);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGangZoneDestroy), ScrGangZoneDestroy);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGangZoneFlash), ScrGangZoneFlash);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGangZoneStopFlash), ScrGangZoneStopFlash);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrApplyAnimation), ScrApplyAnimation);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrClearAnimations), ScrClearAnimations);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerSpecialAction), ScrSetPlayerSpecialAction);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrEnableStuntBonusForPlayer), ScrEnableStuntBonusForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerFightingStyle), ScrSetPlayerFightingStyle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerVelocity), ScrSetPlayerVelocity);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleVelocity), ScrSetVehicleVelocity);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerWidescreen), ScrSetPlayerWidescreen);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleTires), ScrSetVehicleTires);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcPlayerOpcode052C), ScrPlayerOpcode052C);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcPlayerOpcode03FD), ScrPlayerOpcode03FD);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlayCrimeReport), ScrPlayCrimeReport);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetSpawnInfo), ScrSetSpawnInfo);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerTeam), ScrSetPlayerTeam);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerSkin), ScrSetPlayerSkin);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerName), ScrSetPlayerName);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerPosition), ScrSetPlayerPosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerPositionFindZ), ScrSetPlayerPositionFindZ);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerHealth), ScrSetPlayerHealth);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPutPlayerInVehicle), ScrPutPlayerInVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrRemovePlayerFromVehicle), ScrRemovePlayerFromVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerColor), ScrSetPlayerColor);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDisplayGametext), ScrDisplayGametext);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerInterior), ScrSetPlayerInterior);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerCameraPosition), ScrSetPlayerCameraPosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerCameraLookAt), ScrSetPlayerCameraLookAt);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehiclePosition), ScrSetVehiclePosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleZAngle), ScrSetVehicleZAngle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleParamsForPlayer), ScrSetVehicleParamsForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetCameraBehindPlayer), ScrSetCameraBehindPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrTogglePlayerControllable), ScrTogglePlayerControllable);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlaySound), ScrPlaySound);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerWorldBounds), ScrSetPlayerWorldBounds);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGivePlayerMoney), ScrGivePlayerMoney);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerFacingAngle), ScrSetPlayerFacingAngle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrResetPlayerMoney), ScrResetPlayerMoney);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrResetPlayerWeapons), ScrResetPlayerWeapons);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGivePlayerWeapon), ScrGivePlayerWeapon);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown0011830C), ScrRpc64);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrLinkVehicleToInterior), ScrLinkVehicleToInterior);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerArmor), ScrSetPlayerArmor);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDeathMessage), ScrDeathMessage);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerShopName), ScrSetPlayerShopName);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerDrunkLevel), ScrSetPlayerDrunkLevel);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerArmedWeapon), ScrSetPlayerArmedWeapon);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerAttachedObject), ScrSetPlayerAttachedObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlayAudioStream), ScrPlayAudioStream);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrStopAudioStream), ScrStopAudioStream);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrRemoveBuildingForPlayer), ScrRemoveBuildingForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrAttachCameraToObject), ScrAttachCameraToObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrInterpolateCamera), ScrInterpolateCamera);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_textDrawClickRpc), ClickTextDraw);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetObjectMaterial), ScrSetObjectMaterial);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrStopObjectCameraCollision), ScrStopObjectCameraCollision);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetActorAnimation), ScrSetActorAnimation);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrClearActorAnimations), ScrClearActorAnimations);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetActorRotation), ScrSetActorRotation);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetActorPosition), ScrSetActorPosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetActorHealth), ScrSetActorHealth);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcSetCustomModelVirtualWorld), ScrSetCustomModelVirtualWorld);
}

// 0x00018110
void UnRegisterScriptRPCs(RakClientInterface* client)
{
    const int* ids[] = {
        &g_rpcScrDisableVehicleCollisions,
        &g_rpcScrForceClassSelection,
        &g_rpcScrAttachObjectToPlayer,
        &g_rpcScrInitMenu,
        &g_rpcScrShowMenu,
        &g_rpcScrHideMenu,
        &g_rpcScrSetPlayerWantedLevel,
        &g_rpcScrShowTextDraw,
        &g_rpcScrHideTextDrawForPlayer,
        &g_rpcScrTextDrawSetString,
        &g_rpcScrGangZoneCreate,
        &g_rpcScrGangZoneDestroy,
        &g_rpcScrGangZoneFlash,
        &g_rpcScrGangZoneStopFlash,
        &g_rpcScrApplyAnimation,
        &g_rpcScrClearAnimations,
        &g_rpcScrSetPlayerSpecialAction,
        &g_rpcScrEnableStuntBonusForPlayer,
        &g_rpcScrSetPlayerFightingStyle,
        &g_rpcScrSetPlayerVelocity,
        &g_rpcScrSetVehicleVelocity,
        &g_rpcScrSetPlayerWidescreen,
        &g_rpcScrSetVehicleTires,
        &g_rpcPlayerOpcode052C,
        &g_rpcPlayerOpcode03FD,
        &g_rpcScrPlayCrimeReport,
        &g_rpcScrSetPlayerMapIcon,
        &g_rpcScrRemovePlayerMapIcon,
        &g_rpcScrSetPlayerAmmo,
        &g_rpcScrSetGravity,
        &g_rpcScrSetVehicleHealth,
        &g_rpcScrAttachTrailerToVehicle,
        &g_rpcScrDetachTrailerFromVehicle,
        &g_rpcScrCreateObject,
        &g_rpcScrSetObjectPosition,
        &g_rpcScrSetObjectRotation,
        &g_rpcScrDestroyObject,
        &g_rpcScrCreateExplosion,
        &g_rpcScrShowPlayerNametagForPlayer,
        &g_rpcScrMoveObject,
        &g_rpcScrStopObject,
        &g_rpcScrSetNumberPlate,
        &g_rpcScrTogglePlayerSpectating,
        &g_rpcUnknown001183A0,
        &g_rpcScrPlayerSpectatePlayer,
        &g_rpcScrPlayerSpectateVehicle,
        &g_rpcScrMoveVehicleComponent,
        &g_rpcScrSetPlayerShopName,
        &g_rpcScrSetPlayerDrunkLevel,
        &g_rpcScrSetPlayerArmedWeapon,
        &g_rpcScrSetPlayerAttachedObject,
        &g_rpcScrPlayAudioStream,
        &g_rpcScrStopAudioStream,
        &g_rpcScrRemoveBuildingForPlayer,
        &g_rpcScrSetSpawnInfo,
        &g_rpcScrSetPlayerTeam,
        &g_rpcScrSetPlayerName,
        &g_rpcScrSetPlayerSkin,
        &g_rpcScrSetPlayerPosition,
        &g_rpcScrSetPlayerPositionFindZ,
        &g_rpcScrSetPlayerHealth,
        &g_rpcScrPutPlayerInVehicle,
        &g_rpcScrRemovePlayerFromVehicle,
        &g_rpcScrSetPlayerColor,
        &g_rpcScrDisplayGametext,
        &g_rpcScrSetPlayerInterior,
        &g_rpcScrSetPlayerCameraPosition,
        &g_rpcScrSetPlayerCameraLookAt,
        &g_rpcScrSetVehiclePosition,
        &g_rpcScrSetVehicleZAngle,
        &g_rpcScrSetVehicleParamsForPlayer,
        &g_rpcScrSetCameraBehindPlayer,
        &g_rpcScrTogglePlayerControllable,
        &g_rpcScrPlaySound,
        &g_rpcScrSetPlayerWorldBounds,
        &g_rpcScrGivePlayerMoney,
        &g_rpcScrSetPlayerFacingAngle,
        &g_rpcScrResetPlayerMoney,
        &g_rpcScrResetPlayerWeapons,
        &g_rpcScrGivePlayerWeapon,
        &g_rpcUnknown0011830C,
        &g_rpcScrLinkVehicleToInterior,
        &g_rpcScrDeathMessage,
        &g_rpcScrAttachCameraToObject,
        &g_rpcScrInterpolateCamera,
        &g_textDrawClickRpc,
        &g_rpcScrSetObjectMaterial,
        &g_rpcScrStopObjectCameraCollision,
        &g_rpcScrSetActorAnimation,
        &g_rpcScrClearActorAnimations,
        &g_rpcScrSetActorRotation,
        &g_rpcScrSetActorPosition,
        &g_rpcScrSetActorHealth,
        &g_rpcSetCustomModelVirtualWorld,
    };
    for (unsigned i = 0; i < sizeof(ids) / sizeof(ids[0]); i++)
        client->UnregisterAsRemoteProcedureCall(const_cast<int*>(ids[i]));
}
