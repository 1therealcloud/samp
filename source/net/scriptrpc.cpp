#include "net/scriptrpc.h"

#include "game/actor.h"
#include "game/animation.h"
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

const Scripting::OpcodeInfo g_rpcAttachObjectToPlayerCommand = {0x69b, "iiffffff"};

const Scripting::OpcodeInfo g_rpcPlayerOpcode052cCommand = {0x52c, "ii"};

const Scripting::OpcodeInfo g_rpcPlayerOpcode03fdCommand = {0x3fd, "ii"};

const Scripting::OpcodeInfo g_rpcWidescreenCommand = {0x2a3, "i"};

const char g_rpcWidescreenMessage[] = "Widescreen = %i";

const char g_rpcInvalidSkinWarning[] = "Warning: SetPlayerSkin %d isn't a valid ped model.";

const Scripting::OpcodeInfo g_rpcExplosionCommand = {0x948, "fffii"};

const char g_rpcLargeTextDrawWarning[] = "Warning: ignoring large TextDraw size=%u";

const int g_rpcScrDisableVehicleCollisions = 167;

const int g_rpcScrForceClassSelection = 74;

const int g_rpcScrAttachObjectToPlayer = 75;

const int g_rpcScrInitMenu = 76;

const int g_rpcScrShowMenu = 77;

const int g_rpcScrHideMenu = 78;

const int g_rpcScrSetPlayerWantedLevel = 133;

const int g_rpcScrShowTextDraw = 134;

const int g_rpcScrHideTextDrawForPlayer = 135;

const int g_rpcScrTextDrawSetString = 105;

const int g_rpcScrGangZoneCreate = 108;

const int g_rpcScrGangZoneDestroy = 120;

const int g_rpcScrGangZoneFlash = 121;

const int g_rpcScrGangZoneStopFlash = 85;

const int g_rpcScrApplyAnimation = 86;

const int g_rpcScrClearAnimations = 87;

const int g_rpcScrSetPlayerSpecialAction = 88;

const int g_rpcScrEnableStuntBonusForPlayer = 104;

const int g_rpcScrSetPlayerFightingStyle = 89;

const int g_rpcScrSetPlayerVelocity = 90;

const int g_rpcScrSetVehicleVelocity = 91;

const int g_rpcScrSetPlayerWidescreen = 111;

const int g_rpcScrSetVehicleTires = 98;

const int g_rpcPlayerOpcode052C = 92;

const int g_rpcPlayerOpcode03FD = 150;

const int g_rpcScrPlayCrimeReport = 112;

const int g_rpcScrSetPlayerMapIcon = 56;

const int g_rpcScrRemovePlayerMapIcon = 144;

const int g_rpcScrSetPlayerAmmo = 145;

const int g_rpcScrSetGravity = 146;

const int g_rpcScrSetVehicleHealth = 147;

const int g_rpcScrAttachTrailerToVehicle = 148;

const int g_rpcScrDetachTrailerFromVehicle = 149;

const int g_rpcScrCreateObject = 44;

const int g_rpcScrSetObjectPosition = 45;

const int g_rpcScrSetObjectRotation = 46;

const int g_rpcScrDestroyObject = 47;

const int g_rpcScrCreateExplosion = 79;

const int g_rpcScrShowPlayerNametagForPlayer = 80;

const int g_rpcScrMoveObject = 99;

const int g_rpcScrStopObject = 122;

const int g_rpcScrSetNumberPlate = 123;

const int g_rpcScrTogglePlayerSpectating = 124;

const int g_rpcUnknown001183A0 = 125;

const int g_rpcScrPlayerSpectatePlayer = 126;

const int g_rpcScrPlayerSpectateVehicle = 127;

const int g_rpcScrMoveVehicleComponent = 57;

const int g_rpcScrSetPlayerShopName = 33;

const int g_rpcScrSetPlayerDrunkLevel = 35;

const int g_rpcScrSetPlayerArmedWeapon = 67;

const int g_rpcScrSetPlayerAttachedObject = 113;

const int g_rpcScrPlayAudioStream = 41;

const int g_rpcScrStopAudioStream = 42;

const int g_rpcScrRemoveBuildingForPlayer = 43;

const int g_rpcScrSetSpawnInfo = 68;

const int g_rpcScrSetPlayerTeam = 69;

const int g_rpcScrSetPlayerName = 11;

const int g_rpcScrSetPlayerSkin = 153;

const int g_rpcScrSetPlayerPosition = 12;

const int g_rpcScrSetPlayerPositionFindZ = 13;

const int g_rpcScrSetPlayerHealth = 14;

const int g_rpcScrPutPlayerInVehicle = 70;

const int g_rpcScrRemovePlayerFromVehicle = 71;

const int g_rpcScrSetPlayerColor = 72;

const int g_rpcScrDisplayGametext = 73;

const int g_rpcScrSetPlayerInterior = 156;

const int g_rpcScrSetPlayerCameraPosition = 157;

const int g_rpcScrSetPlayerCameraLookAt = 158;

const int g_rpcScrSetVehiclePosition = 159;

const int g_rpcScrSetVehicleZAngle = 160;

const int g_rpcScrSetVehicleParamsForPlayer = 161;

const int g_rpcScrSetCameraBehindPlayer = 162;

const int g_rpcScrTogglePlayerControllable = 15;

const int g_rpcScrPlaySound = 16;

const int g_rpcScrSetPlayerWorldBounds = 17;

const int g_rpcScrGivePlayerMoney = 18;

const int g_rpcScrSetPlayerFacingAngle = 19;

const int g_rpcScrResetPlayerMoney = 20;

const int g_rpcScrResetPlayerWeapons = 21;

const int g_rpcScrGivePlayerWeapon = 22;

const int g_rpcUnknown0011830C = 64;

const int g_rpcScrLinkVehicleToInterior = 65;

const int g_rpcScrDeathMessage = 55;

const int g_rpcScrAttachCameraToObject = 81;

const int g_rpcScrInterpolateCamera = 82;

const int g_rpcScrSetObjectMaterial = 84;

const int g_rpcScrStopObjectCameraCollision = 169;

const int g_rpcScrSetActorAnimation = 173;

const int g_rpcScrClearActorAnimations = 174;

const int g_rpcScrSetActorRotation = 175;

const int g_rpcScrSetActorPosition = 176;

const int g_rpcScrSetActorHealth = 178;

const int g_rpcSetCustomModelVirtualWorld = 48;

const int g_rpcScrSetPlayerArmor = 66;

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
        if (player >= 1004 || !players->players[player] || !players->players[player]->remote)
            return;
        ped = players->players[player]->remote->m_state.ped;
    }
    Scripting::ProcessCommand(&g_rpcAttachObjectToPlayerCommand, *(DWORD*)((BYTE*)object + 0x44), *(DWORD*)((BYTE*)ped + 0x44), (double)offset.x, (double)offset.y,
                              (double)offset.z, (double)rotation.x, (double)rotation.y, (double)rotation.z);
}

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

void ScrAttachCameraToObject(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CObjectPool* pool = ((CNetGame*)g_pNetGame)->pools->objects;
    if (id < 2100 && pool->active[id])
    {
        CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
        local->m_state.spectateProcessed = 1;
        CEntity* object = ((CNetGame*)g_pNetGame)->pools->objects->Get(id);
        g_pGame->m_pCamera->Attach(object);
    }
}

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

void ScrStopObjectCameraCollision(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CEntity* object = ((CNetGame*)g_pNetGame)->pools->objects->Get(id);
    if (object)
        ((BYTE*)object)[0x53] = 1;
}

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

void ScrIgnoreRpc125(RPCParameters* parameters)
{
    (void)parameters;
}

void ScrReadAndDiscardRpc64(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    WORD ignored;
    stream.Read(ignored);
}

void ScrSetSpawnInfo(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    BYTE info[sizeof(LocalSpawnInfo)] = {0};
    stream.Read((char*)info, GetLocalSpawnInfoWireSize());
    pool->local->SetSpawnInfo(info);
}

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
    else if (id < 1004)
    {
        CPlayerInfo* info = pool->players[id];
        if (info && info->remote)
            info->remote->m_state.team = team;
    }
}

void ScrSetPlayerSkin(RPCParameters* parameters)
{
    int model = 0;
    int customModel = 0; // g_DLClient

    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    if (!pool)
        return;

    DWORD id = 0;
    if (g_DLClient)
    {
        WORD id16;
        stream.Read(id16);
        id = id16;
    }
    else
        stream.Read(id);

    stream.Read(model);
    if (g_DLClient)
    {
        stream.Read(customModel);
        if (customModel > 20000 && (GameIsPedModel(customModel) || (g_customModels && g_customModels->DoesExist(customModel))))
            model = customModel;
    } // g_DLClient
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
    if (!GameIsAnimationNameValid(library, name))
        return;
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

void ScrStopObject(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    stream.Read(id);
    CObject* object = (CObject*)((CNetGame*)g_pNetGame)->pools->objects->Get(id);
    if (object)
        object->Stop();
}

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
        if (!GameIsAnimationNameValid(library, name))
            return;
        CActor* actor = pool->Get(id);
        if (actor)
            actor->PerformAnimation(name, library, speed, loop, lockX, lockY, freeze, time);
    }
}

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

void ScrRemovePlayerMapIcon(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    BYTE id;
    stream.Read(id);
    ((CNetGame*)g_pNetGame)->RemoveMapIcon(id);
}

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

void ScrLinkVehicleToInterior(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    WORD id;
    BYTE interior;
    stream.Read(id);
    stream.Read(interior);
    ((CNetGame*)g_pNetGame)->pools->vehicles->ChangeInterior(id, interior);
}

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

void ScrTogglePlayerSpectating(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    BOOL enabled;
    stream.Read(enabled);
    ((CNetGame*)g_pNetGame)->pools->players->local->ToggleSpectating(enabled);
}

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

void ScrRemovePlayerFromVehicle(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.ped->ExitVehicle();
}

void ScrDisplayGametext(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    char text[256] = {0};
    int style, time, length;
    stream.Read(style);
    stream.Read(time);
    stream.Read(length);
    // the game has 7 styles (0..6)
    if (length > 0 && length < 201 && style >= 0 && style <= 6)
    {
        stream.Read(text, length);
        text[length] = 0;
        g_pGame->DisplayGameText(text, time, style);
    }
}

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
    else if (player < 1004)
    {
        CPlayerInfo* info = pool->players[player];
        if (info && info->remote)
            info->remote->SetColor(color);
    }
}

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

void ScrInitMenu(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    if (!g_pNetGame)
        return;
    CMenuPool* pool = ((CNetGame*)g_pNetGame)->pools->menus;
    if (!pool)
        return;
    BYTE id;
    int twoColumns;
    // one extra byte so the strings are always terminated
    char text[33] = {0};
    char columnTitles[2][33] = {{0}};
    char items[2][12][33] = {{{0}}};
    float x = 0, y = 0, width1 = 0, width2 = 0;
    CMenuInteraction interaction = {};
    stream.Read(id);
    stream.Read(twoColumns);
    stream.Read(text, 32);
    stream.Read(x);
    stream.Read(y);
    stream.Read(width1);
    if (twoColumns)
        stream.Read(width2);
    // only the first 13 dwords are sent
    stream.Read(interaction.enabled);
    for (int i = 0; i < 12; i++)
        stream.Read(interaction.rows[i]);
    // the original also checks menu ID 128
    if (id >= 128)
        return;
    // read everything before touching the pool, so a bad packet leaves it alone
    BYTE columns = twoColumns ? 2 : 1;
    BYTE counts[2] = {0, 0};
    for (BYTE column = 0; column < columns; column++)
    {
        stream.Read(columnTitles[column], 32);
        stream.Read(counts[column]);
        if (counts[column] > 12)
            return;
        for (BYTE row = 0; row < counts[column]; row++)
            stream.Read(items[column][row], 32);
    }
    if (pool->active[id])
        pool->Delete(id);
    CMenu* menu = pool->Create(id, text, x, y, columns, width1, width2, &interaction);
    if (menu)
    {
        for (BYTE column = 0; column < columns; column++)
        {
            menu->SetColumnTitle(column, columnTitles[column]);
            for (BYTE row = 0; row < counts[column]; row++)
                menu->AddItem(column, row, items[column][row]);
        }
    }
}

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

void ScrStopAudioStream(RPCParameters* parameters)
{
    (void)parameters;
    if (g_renderAudioStream)
        g_renderAudioStream->Stop(false);
}

void ScrSetPlayerCameraPosition(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    CVector position, rotation = {0, 0, 0};
    stream.Read(position.x);
    stream.Read(position.y);
    stream.Read(position.z);
    g_pGame->m_pCamera->Set(position, rotation);
}

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

void ScrSetCameraBehindPlayer(RPCParameters* parameters)
{
    (void)parameters;
    g_pGame->m_pCamera->Restore();
}

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

void ScrSetPlayerWorldBounds(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 5, 32, true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 9, 32, true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 0xd, 32, true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 0x11, 32, true);
}

void ScrSetPlayerFacingAngle(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    float angle;
    stream.Read(angle);
    g_pGame->GetPlayerPed()->SetRotation(angle);
}

void ScrEnableStuntBonusForPlayer(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    bool enabled;
    stream.Read(enabled);
    g_pGame->EnableStuntBonus((BYTE)enabled);
}

void ScrForceClassSelection(RPCParameters* parameters)
{
    (void)parameters;
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.wantsAnotherClass = 1;
}

void ScrResetPlayerWeapons(RPCParameters* parameters)
{
    (void)parameters;
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.ped->ClearWeapons();
}

void ScrGivePlayerWeapon(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    int weapon, ammo;
    stream.Read(weapon);
    stream.Read(ammo);
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    local->m_state.ped->GiveWeapon(weapon, ammo);
}

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

void ScrDisableVehicleCollisions(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    bool disabled = false;
    stream.Read(disabled);
    if (g_pNetGame)
        ((CNetGame*)g_pNetGame)->disableVehicleCollisions = (BYTE)disabled;
}

void ScrSetGravity(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    float gravity;
    stream.Read(gravity);
    g_pGame->SetWorldGravity(gravity);
}

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

void ScrSetPlayerHealth(RPCParameters* parameters)
{
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    float health;
    stream.Read(health);
    local->m_state.ped->SetHealth(health);
}

void ScrSetPlayerArmor(RPCParameters* parameters)
{
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    float armour;
    stream.Read(armour);
    local->m_state.ped->SetArmour(armour);
}

void ScrSetPlayerInterior(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    BYTE interior;
    stream.Read(interior);
    g_pGame->GetPlayerPed()->LinkToInterior(interior, TRUE);
}

void ScrGivePlayerMoney(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    int amount;
    stream.Read(amount);
    g_pGame->IncreasePlayerMoney(amount);
}

void ScrResetPlayerMoney(RPCParameters* parameters)
{
    (void)parameters;
    g_pGame->ResetMoney();
}

void ScrSetPlayerSpecialAction(RPCParameters* parameters)
{
    RakNet::BitStream stream(parameters->input, (int)parameters->numberOfBitsOfData / 8 + 1, false);
    BYTE action;
    stream.Read(action);
    CPlayerPool* players = ((CNetGame*)g_pNetGame)->pools->players;
    if (players)
        players->local->SetSpecialAction(action);
}

void ScrTogglePlayerControllable(RPCParameters* parameters)
{
    unsigned char* input = parameters->input;
    const int bits = (int)parameters->numberOfBitsOfData;
    RakNet::BitStream stream(input, bits / 8 + 1, false);
    BYTE enabled;
    stream.Read(enabled);
    g_pGame->GetPlayerPed()->SetControllable(enabled);
}

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
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown001183A0), ScrIgnoreRpc125);
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
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown0011830C), ScrReadAndDiscardRpc64);
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
