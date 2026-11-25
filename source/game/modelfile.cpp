#include "game/modelfile.h"

#include "game/custommodels.h"
#include "game/util.h"

#include <Alloc.h>
#include <LzmaEnc.h>

#include <new>
#include <stdlib.h>
#include <string.h>

const BYTE g_modelLzmaProperties[5] = {0x5d, 0, 0, 1, 0};

const char g_customTxdVerifyError[] = "[warning] Could not verify 0x%X.txd. Error(%d) Deleting..";

const char g_customTxdCountError[] = "[warning] Too many textures in 0x%X.txd. (%u) Deleting..";

const char g_customDffVerifyError[] = "[warning] Could not validate 0x%X.dff. Error(%d) Deleting..";

const char g_customDffParseError[] = "[warning] Could not validate 0x%X.dff. Error(%d:0x%X) Deleting..";

const float g_modelCollisionVertexScale = 128.0f;

const float g_modelCollisionVertexInvScale = 0.0078125f;

extern const float g_textDrawHalf;

void CModelFileStream::Reset()
{
    position = 0;
    ended = 0;
}

BYTE CModelFileStream::HasEnded()
{
    return ended;
}

int CModelGeometry::GetVertexCount()
{
    return vertexCount;
}

BYTE CModelGeometry::HasVertexColors()
{
    return hasColors;
}

void* CModelGeometry::GetVertexColors()
{
    return colors;
}

DWORD CModelGeometry::SetRepeatedVertexColors(const BYTE* first, const BYTE* second)
{
    modified = 1;
    flags = (WORD)((flags & 0xffdf) | 8);
    if (colors)
    {
        free(colors);
        colors = 0;
    }
    if (secondaryColors)
    {
        free(secondaryColors);
        secondaryColors = 0;
    }
    hasColors = 1;
    hasSecondaryColors = 1;
    colors = calloc(vertexCount, 4);
    secondaryColors = calloc(vertexCount, 4);
    for (DWORD i = 0; i != (DWORD)vertexCount; ++i)
    {
        ((BYTE*)colors)[i * 4] = first[0];
        ((BYTE*)colors)[i * 4 + 1] = first[1];
        ((BYTE*)colors)[i * 4 + 2] = first[2];
        ((BYTE*)colors)[i * 4 + 3] = first[3];
        ((BYTE*)secondaryColors)[i * 4] = second[0];
        ((BYTE*)secondaryColors)[i * 4 + 1] = second[1];
        ((BYTE*)secondaryColors)[i * 4 + 2] = second[2];
        ((BYTE*)secondaryColors)[i * 4 + 3] = second[3];
    }
    return 1;
}

bool CDffFile::IsModified()
{
    DWORD count = geometries.count;
    for (DWORD i = 0; i != count; ++i)
        if (geometries.Get(i)->IsModified())
            return true;
    return false;
}

DWORD __stdcall ModelAppendChunk(CModelFileChunk* parent, const CModelFileChunk* child)
{
    if (!parent || !child)
        return 0;
    parent->stream.Write(child, 12);
    parent->stream.Write(child->stream.data, child->stream.length);
    parent->length += child->stream.length + 12;
    return 1;
}

int __stdcall ModelLzmaEncode(BYTE* destination, DWORD* destinationLength, const BYTE* source, DWORD sourceLength, BYTE* properties, DWORD* propertySize, int level,
                              DWORD dictionary, int lc, int lp, int pb, int fastBytes, int threads)
{
    CLzmaEncProps options;
    LzmaEncProps_Init(&options);
    options.level = level;
    options.dictSize = dictionary;
    options.lc = lc;
    options.lp = lp;
    options.pb = pb;
    options.fb = fastBytes;
    options.numThreads = threads;
    return LzmaEncode(destination, (SizeT*)destinationLength, source, sourceLength, &options, properties, (SizeT*)propertySize, 0, 0, &g_Alloc, &g_Alloc);
}

CModelFileStream::CModelFileStream()
{
    data = 0;
    length = 0;
    position = 0;
    ended = 0;
    owned = 0;
    compressed = 0;
    scratchLength = 0;
    scratch = 0;
}

CModelFileStream::~CModelFileStream()
{
    if (owned && data)
    {
        free(data);
        data = 0;
    }
}

int CModelFileStream::Compress()
{
    BYTE properties[5];
    DWORD propertySize = 5;
    scratchLength = length;
    if (compressed)
        return -1;
    scratch = (BYTE*)malloc(length);
    int result = ModelLzmaEncode(scratch, &scratchLength, data, length, properties, &propertySize, 1, 0x10000, 3, 0, 2, 32, 1);
    if (result)
    {
        free(scratch);
        compressed = 0;
        return result;
    }
    if (owned)
        free(data);
    data = scratch;
    owned = 1;
    length = scratchLength;
    compressed = 1;
    return result;
}

int CModelFileStream::Decompress(DWORD outputLength)
{
    if (!compressed)
        return -1;
    scratch = (BYTE*)malloc(outputLength);
    int result = CustomModelLzmaDecode(scratch, &outputLength, data, &length, g_modelLzmaProperties, 5);
    if (result)
    {
        free(scratch);
        compressed = 1;
        return result;
    }
    if (owned)
        free(data);
    length = outputLength;
    data = scratch;
    owned = 1;
    compressed = 0;
    return result;
}

DWORD CModelFileStream::WriteFileChunks(FILE* file)
{
    BYTE* buffer = data;
    DWORD offset = 0;
    while (offset < length)
    {
        DWORD next = offset + 1024;
        if (next < length)
        {
            fwrite(buffer + offset, 1, 1024, file);
            offset = next;
        }
        else
        {
            fwrite(buffer + offset, 1, length - offset, file);
            offset = length;
        }
    }
    return length;
}

DWORD CModelFileStream::SaveFile(const char* filename)
{
    FILE* file = nullptr;
    fopen_s(&file, filename, g_customDownloadWriteMode);
    if (!file)
        return 0;
    WriteFileChunks(file);
    fclose(file);
    return length;
}

DWORD CModelFileStream::SaveHandle(FILE* file)
{
    return WriteFileChunks(file);
}

bool CModelFileStream::CopyBuffer()
{
    BYTE* copy = (BYTE*)malloc(length);
    if (!copy)
        return false;
    memcpy(copy, data, length);
    data = copy;
    owned = 1;
    return true;
}

DWORD CModelFileStream::Write(const void* input, DWORD count)
{
    if (position + count > length)
    {
        if (!owned)
            CopyBuffer();
        DWORD extra = count - length + position;
        data = (BYTE*)realloc(data, length + extra);
        length += extra;
    }
    memcpy(data + position, input, count);
    position += count;
    return count;
}

DWORD CModelFileStream::Read(void* output, DWORD count)
{
    memset(output, 0, count);
    // position + count wraps for big counts coming from the file
    DWORD left = position < length ? length - position : 0;
    if (count > left)
        count = left;
    if (count == 0)
        return 0;
    memcpy(output, data + position, count);
    DWORD next = position + count;
    if (next < length)
        position = next;
    else
    {
        position = length;
        ended = 1;
    }
    return count;
}

DWORD CModelFileStream::LoadFile(const char* filename)
{
    BYTE buffer[2048];
    position = 0;
    if (owned && data)
    {
        free(data);
        data = 0;
    }
    FILE* file = nullptr;
    fopen_s(&file, filename, g_customDownloadReadMode);
    if (!file)
        return 0;
    owned = 1;
    data = (BYTE*)malloc(1);
    length = 1;
    while (!feof(file) && !ferror(file))
    {
        DWORD count = (DWORD)fread(buffer, 1, 2048, file);
        if (count == 0)
            break;
        Write(buffer, count);
    }
    fclose(file);
    position = 0;
    ended = 0;
    return length;
}

// Equivalent native forwarding entry at RVA 0x000b9590; original duplicate identity unknown.

DWORD CModelFileStream::WriteDword(DWORD value)
{
    return Write(&value, 4);
}

DWORD CModelFileStream::WriteWord(WORD value)
{
    return Write(&value, 2);
}

DWORD CModelFileStream::ReadDword()
{
    DWORD value = 0;
    Read(&value, 4);
    return value;
}

BYTE CModelFileStream::ReadByte()
{
    BYTE value = 0;
    Read(&value, 1);
    return value;
}

BYTE* CModelFileStream::Current()
{
    return data + position;
}

DWORD CModelFileStream::Remaining()
{
    return length - position;
}

CModelFileChunk::~CModelFileChunk()
{
    // The stream member performs the native owned-buffer cleanup.
}

DWORD CModelFileChunk::GetType()
{
    return type;
}

DWORD CModelFileChunk::GetLength()
{
    return length;
}

CTxdFile::CTxdFile()
{
    error = 0;
    fileLength = 0;
    root = 0;
    structure = 0;
    textureCount = 0;
    parsedCount = 0;
}

CTxdFile::~CTxdFile()
{
    if (structure)
    {
        delete structure;
        structure = 0;
    }
    if (root)
    {
        delete root;
        root = 0;
    }
}

DWORD CTxdFile::GetTextureCount()
{
    return textureCount;
}

DWORD CTxdFile::GetError()
{
    return error;
}

// the three ReadChunk copies below share this. a chunk that claims more data than
// its parent has left is rejected (and flagged), so child streams stay in bounds
static CModelFileChunk* ModelReadChunk(CModelFileStream* input, bool* truncated)
{
    if (!input->length || input->ended)
        return 0;
    CModelFileChunk* chunk = (CModelFileChunk*)::operator new(sizeof(CModelFileChunk));
    if (!chunk)
        return 0;
    chunk->stream.data = 0;
    chunk->stream.length = 0;
    chunk->stream.position = 0;
    chunk->stream.ended = 0;
    chunk->stream.owned = 0;
    chunk->stream.compressed = 0;
    chunk->stream.scratchLength = 0;
    chunk->stream.scratch = 0;
    chunk->type = 0;
    chunk->length = 0;
    chunk->build = 0xffff;
    chunk->version = 0x1803;
    DWORD word = 0;
    input->Read(&word, 4);
    chunk->type = word;
    word = 0;
    input->Read(&word, 4);
    chunk->length = word;
    word = 0;
    input->Read(&word, 2);
    chunk->build = (WORD)word;
    word = 0;
    input->Read(&word, 2);
    chunk->version = (WORD)word;
    DWORD left = input->position < input->length ? input->length - input->position : 0;
    if (chunk->length > left)
    {
        *truncated = true;
        input->position = input->length;
        input->ended = 1;
        delete chunk;
        return 0;
    }
    chunk->stream.length = chunk->length;
    chunk->stream.data = input->data + input->position;
    chunk->stream.position = 0;
    chunk->stream.ended = 0;
    chunk->stream.owned = 0;
    DWORD next = input->position + chunk->length;
    if (next < input->length)
        input->position = next;
    else
    {
        input->position = input->length;
        input->ended = 1;
    }
    return chunk;
}

CModelFileChunk* CTxdFile::ReadChunk(CModelFileStream* input)
{
    bool truncated = false;
    CModelFileChunk* chunk = ModelReadChunk(input, &truncated);
    if (truncated && !error)
        error = 0xabf4;
    return chunk;
}

DWORD CTxdFile::ReadStructure(CModelFileChunk* chunk)
{
    if (structure)
        return 0;
    structure = chunk;
    DWORD count = 0;
    chunk->stream.Read(&count, 2);
    textureCount = (WORD)count;
    return 1;
}

DWORD CTxdFile::ReadTexture(CModelFileChunk* chunk)
{
    DWORD header[3];
    CModelFileStream* input = &chunk->stream;
    input->Read(header, 12);
    if (header[0] != 1)
        return 0;
    // Native builds a temporary 88-byte descriptor; it is never published.
    DWORD value = 0;
    input->Read(&value, 4);
    if (value != 8 && value != 9)
    {
        if (!error)
            error = 0xd2f6;
        return 0;
    }
    DWORD platform = value;
    value = 0;
    input->Read(&value, 4);
    char name[65] = {0}, mask[65] = {0};
    input->Read(name, 32);
    if (strlen(name) > 31)
    {
        if (!error)
            error = 0xd2f5;
        return 0;
    }
    char savedName[32], savedMask[32];
    strncpy_s(savedName, name, 31);
    input->Read(mask, 32);
    if (strlen(mask) > 31)
    {
        if (!error)
            error = 0xd2f5;
        return 0;
    }
    strncpy_s(savedMask, mask, 31);
    DWORD rasterFormat = 0, d3dFormat = 0;
    input->Read(&rasterFormat, 4);
    input->Read(&d3dFormat, 4);
    DWORD width = 0, height = 0;
    input->Read(&width, 2);
    input->Read(&height, 2);
    if ((WORD)width > 2048 || (WORD)height > 2048)
    {
        if (!error)
            error = 0xd2f7;
        return 0;
    }
    BYTE depth = input->ReadByte();
    BYTE mipCount = input->ReadByte();
    input->ReadByte();
    BYTE compression = input->ReadByte();
    // the mip sizes come from the file and the game trusts them when it reads the pixels,
    // so they have to match the raster. the fixed fields above are 88 bytes of the struct
    DWORD dataLeft = header[1] >= 88 ? header[1] - 88 : 0;
    DWORD palette = (rasterFormat & 0x2000) ? 1024 : ((rasterFormat & 0x4000) ? 128 : 0);
    if (header[1] < 88 || dataLeft > input->Remaining() || !mipCount || mipCount > 16 || !depth || depth > 32 || palette > dataLeft)
    {
        if (!error)
            error = 0xd2f8;
        return 0;
    }
    input->Skip(palette);
    dataLeft -= palette;
    DWORD block = 0;
    if (platform == 9)
    {
        if (d3dFormat == 0x31545844)
            block = 8;
        else if (d3dFormat >= 0x32545844 && d3dFormat <= 0x35545844)
            block = 16;
    }
    else if (compression >= 1 && compression <= 5)
        block = compression == 1 ? 8 : 16;
    for (DWORD level = 0; level < mipCount; ++level)
    {
        DWORD w = (WORD)width >> level, h = (WORD)height >> level;
        if (!w)
            w = 1;
        if (!h)
            h = 1;
        DWORD expected = block ? ((w + 3) / 4) * ((h + 3) / 4) * block : w * h * ((depth + 7) / 8);
        DWORD size = 0;
        if (dataLeft < 4)
        {
            if (!error)
                error = 0xd2f8;
            return 0;
        }
        input->Read(&size, 4);
        dataLeft -= 4;
        if (size > expected || size > dataLeft)
        {
            if (!error)
                error = 0xd2f8;
            return 0;
        }
        input->Skip(size);
        dataLeft -= size;
    }
    ++parsedCount;
    return 1;
}

DWORD CTxdFile::ReadChildren(CModelFileChunk* parent)
{
    CModelFileStream* input = &parent->stream;
    CModelFileChunk* chunk = ReadChunk(input);
    while (chunk)
    {
        if (chunk->type == 1)
        {
            if (!ReadStructure(chunk))
                delete chunk;
        }
        else if (chunk->type == 0x15)
        {
            if (!ReadTexture(chunk))
            {
                delete chunk;
                if (!error)
                    error = 0xd2f2;
                return 0;
            }
            // Native retains successful texture chunks without publishing a pointer.
        }
        else
            delete chunk;
        chunk = ReadChunk(input);
    }
    return 1;
}

DWORD CTxdFile::LoadFile(const char* filename)
{
    if (!stream.LoadFile(filename))
        return 0xffffffff;
    fileLength = stream.length;
    root = ReadChunk(&stream);
    if (!root)
    {
        if (!error)
            error = 54000;
        return 0;
    }
    if (root)
    {
        if (root->type != 0x16)
        {
            if (!error)
                error = 54000;
            return 0;
        }
        if (!ReadChildren(root) || error)
            return 0;
        if (textureCount != parsedCount)
        {
            if (!error)
                error = 0xd2f2;
            return 0;
        }
    }
    return 1;
}

DWORD CDffFile::StoreFrames(CModelFileChunk* chunk)
{
    if (!frames)
    {
        frames = chunk;
        return 1;
    }
    return 0;
}

DWORD CDffFile::StoreGeometryList(CModelFileChunk* chunk)
{
    if (!geometryList)
    {
        geometryList = chunk;
        return 1;
    }
    return 0;
}

DWORD CDffFile::StoreAtomic(CModelFileChunk* chunk)
{
    if (!atomic)
    {
        atomic = chunk;
        return 1;
    }
    return 0;
}

DWORD CDffFile::StoreStructure(CModelFileChunk* chunk)
{
    if (!structure)
    {
        structure = chunk;
        return 1;
    }
    return 0;
}

DWORD CDffFile::StoreExtension(CModelFileChunk* chunk)
{
    if (!extension)
    {
        extension = chunk;
        return 1;
    }
    return 0;
}

DWORD CDffFile::ReadChildren(CModelFileChunk* parent)
{
    CModelFileStream* input = &parent->stream;
    CModelFileChunk* chunk = ReadChunk(input);
    while (chunk)
    {
        DWORD retained = 0, failure = 0;
        switch (chunk->type)
        {
            case 1:
                retained = StoreStructure(chunk);
                break;
            case 3:
                retained = StoreExtension(chunk);
                break;
            case 14:
                retained = StoreFrames(chunk);
                if (!retained)
                    failure = 0xabf0;
                break;
            case 20:
                retained = StoreAtomic(chunk);
                if (!retained)
                    failure = 0xabf1;
                break;
            case 26:
                retained = StoreGeometryList(chunk);
                if (!retained)
                    failure = 0xabef;
                break;
        }
        if (!retained)
            delete chunk;
        if (failure)
        {
            if (!error)
                error = failure;
            return 0;
        }
        chunk = ReadChunk(input);
    }
    return 1;
}

DWORD CDffFile::LoadFile(const char* filename)
{
    if (!stream.LoadFile(filename))
        return 0xffffffff;
    fileLength = stream.length;
    root = ReadChunk(&stream);
    if (!root)
    {
        if (!error)
            error = 44000;
        return 0;
    }
    if (root)
    {
        if (root->type == 0x2b)
        {
            delete root;
            root = 0;
            root = ReadChunk(&stream);
        }
        if (!root)
        {
            if (!error)
                error = 44000;
            return 0;
        }
        if (root->type != 0x10)
        {
            if (!error)
                error = 44000;
            return 0;
        }
        if (!ReadChildren(root) || error)
            return 0;
        if (extension && extension->length)
        {
            CModelFileChunk* chunk = ReadChunk(&extension->stream);
            if (chunk && chunk->type == 0x253f2ff)
                hasCollision = 1;
            // The collision probe chunk is deliberately not deleted by native.
        }
    }
    return 1;
}

DWORD CDffFile::LoadCollisionFile(const char* filename)
{
    if (!stream.LoadFile(filename))
        return 0xffffffff;
    fileLength = stream.length;
    root = ReadChunk(&stream);
    if (root && root->type == 0x10)
    {
        ReadChildren(root);
        if (extension && extension->length > 0)
        {
            CModelFileChunk* chunk = ReadChunk(&extension->stream);
            if (chunk && chunk->type == 0x253f2ff)
                hasCollision = 1;
        }
    }
    return 1;
}

DWORD CDffFile::VerifyFrameExtension(CModelFileChunk* chunk)
{
    while (!chunk->stream.ended)
    {
        DWORD header[3];
        chunk->stream.Read(header, 12);
        if (header[0] == 0x253f2fe && header[1] >= 24)
            return 0;
        chunk->stream.Skip(header[1]);
    }
    return 1;
}

DWORD CDffFile::ReadFrames()
{
    if (!frames)
        return 0;
    frames->stream.position = 0;
    frames->stream.ended = 0;
    DWORD header[3];
    frames->stream.Read(header, 12);
    if (header[0] != 1)
    {
        if (!error)
            error = 0xabe7;
        detail = 1;
        return 0;
    }
    DWORD count = 0;
    frames->stream.Read(&count, 4);
    if (count >= 256)
    {
        if (!error)
            error = 0xabec;
        detail = count;
        return 0;
    }
    if (!count || frames->stream.Remaining() < count * 56)
    {
        if (!error)
            error = 0xabf6;
        detail = count;
        return 0;
    }
    frameData = calloc(count, 56);
    if (!frameData)
    {
        if (!error)
            error = 0xabf6;
        return 0;
    }
    frames->stream.Read(frameData, count * 56);
    frameCount = count;
    // parent is a dword at +48 of every 56 byte frame: -1 or another frame, and no loops
    for (DWORD i = 0; i < count; ++i)
    {
        DWORD parent = *(DWORD*)((BYTE*)frameData + i * 56 + 48);
        bool valid = parent == 0xffffffff || parent < count;
        for (DWORD steps = 0; valid && parent != 0xffffffff; ++steps)
        {
            if (steps > count)
                valid = false;
            else
                parent = *(DWORD*)((BYTE*)frameData + parent * 56 + 48);
            if (valid && parent != 0xffffffff && parent >= count)
                valid = false;
        }
        if (!valid)
        {
            if (!error)
                error = 0xabf6;
            detail = i;
            return 0;
        }
    }
    CModelFileChunk* chunk = ReadChunk(&frames->stream);
    while (chunk)
    {
        if (chunk->type == 3 && chunk->length && !VerifyFrameExtension(chunk))
        {
            delete chunk;
            if (!error)
                error = 0xabee;
            return 0;
        }
        delete chunk;
        chunk = ReadChunk(&frames->stream);
    }
    if (error)
        return 0;
    return 1;
}

CModelFileChunk* CDffFile::ReadChunk(CModelFileStream* input)
{
    bool truncated = false;
    CModelFileChunk* chunk = ModelReadChunk(input, &truncated);
    if (truncated && !error)
        error = 0xabf4;
    return chunk;
}

CModelGeometryArray::CModelGeometryArray()
{
    items = 0;
    count = 0;
}

CModelGeometry* CModelGeometryArray::Get(DWORD index)
{
    if (index >= count)
        return 0;
    return items[index];
}

DWORD CModelGeometryArray::GetCount()
{
    return count;
}

CModelMaterialArray::CModelMaterialArray()
{
    items = 0;
    count = 0;
}

void* CModelMaterialArray::Get(DWORD index)
{
    return index < count ? items[index] : 0;
}

DWORD CModelMaterialArray::GetCount()
{
    return count;
}

CModelMeshArray::CModelMeshArray()
{
    items = 0;
    count = 0;
}

BYTE* CModelMeshArray::Get(DWORD index)
{
    return index < count ? items[index] : 0;
}

DWORD CModelMeshArray::GetCount()
{
    return count;
}

bool CModelMaterialArray::Resize(DWORD size)
{
    if (!size)
    {
        if (items)
        {
            free(items);
            items = 0;
        }
        count = 0;
        return true;
    }
    void** replacement = (void**)realloc(items, size * 4);
    if (!replacement)
        return false;
    items = replacement;
    DWORD i = count;
    if (i < size)
    {
        do
        {
            items[i] = 0;
        }
        while (++i != size);
    }
    count = size;
    return true;
}

bool CModelMeshArray::Resize(DWORD size)
{
    if (!size)
    {
        if (items)
        {
            free(items);
            items = 0;
        }
        count = 0;
        return true;
    }
    BYTE** replacement = (BYTE**)realloc(items, size * 4);
    if (!replacement)
        return false;
    items = replacement;
    DWORD i = count;
    if (i < size)
    {
        do
        {
            items[i] = 0;
        }
        while (++i != size);
    }
    count = size;
    return true;
}

bool CModelGeometryArray::Resize(DWORD size)
{
    if (!size)
    {
        if (items)
        {
            free(items);
            items = 0;
        }
        count = 0;
        return true;
    }
    CModelGeometry** replacement = (CModelGeometry**)realloc(items, size * 4);
    if (!replacement)
        return false;
    items = replacement;
    DWORD i = count;
    if (i < size)
    {
        do
        {
            items[i] = 0;
        }
        while (++i != size);
    }
    count = size;
    return true;
}

DWORD CModelGeometryArray::Insert(CModelGeometry* geometry)
{
    DWORD previous = count, index = 0;
    if (previous)
    {
        CModelGeometry** entries = items;
        CModelGeometry** slot = entries;
        do
        {
            if (!*slot)
            {
                if ((LONG)index >= 0)
                {
                    entries[index] = geometry;
                    return index;
                }
                break;
            }
            ++index;
            ++slot;
        }
        while (index != previous);
    }
    if (!Resize(previous + 1))
        return 0xffffffff;
    items[previous] = geometry;
    return previous;
}

CModelGeometry::CModelGeometry()
{
    error = 0;
    detail = 0;
    flags = 0;
    textureSets = 0;
    triangleCount = 0;
    vertexCount = 0;
    morphCount = 0;
    hasColors = 0;
    colors = 0;
    hasSecondaryColors = 0;
    secondaryColors = 0;
    uv = 0;
    triangles = 0;
    memset((BYTE*)this + offsetof(CModelGeometry, sphere), 0, offsetof(CModelGeometry, material) - offsetof(CModelGeometry, sphere));
    materialCount = 0;
    hasBinMesh = 0;
    binMeshFlags = 0;
    binMeshCount = 0;
    binMeshIndexCount = 0;
    modified = 0;
    modifiedOther = 0;
    memset(opaque75, 0, 44);
    material = 0;
    extension = 0;
}

CModelGeometry::~CModelGeometry()
{
    // These plugin buffer roles remain opaque; only the native ownership is established.
    void** buffers[] = {&normals,
                        &vertices,
                        &triangles,
                        &uv,
                        &colors,
                        &secondaryColors,
                        (void**)(opaque75 + 4),
                        (void**)(opaque75 + 8),
                        (void**)(opaque75 + 12),
                        (void**)(opaque75 + 16),
                        (void**)(opaque75 + 32),
                        (void**)(opaque75 + 36),
                        (void**)(opaque75 + 40)};
    for (unsigned i = 0; i < sizeof(buffers) / sizeof(buffers[0]); ++i)
    {
        if (*buffers[i])
        {
            free(*buffers[i]);
            *buffers[i] = 0;
        }
    }
    DWORD materialCount = materials.count;
    for (DWORD j = 0; j != materialCount; ++j)
        if (j < materials.count && materials.items[j])
            free(materials.items[j]);
    DWORD meshCount = meshes.count;
    for (DWORD k = 0; k != meshCount; ++k)
    {
        if (k < meshes.count && meshes.items[k])
        {
            BYTE* mesh = meshes.items[k];
            void*& indices = *(void**)(mesh + 8);
            if (indices)
            {
                free(indices);
                indices = 0;
            }
            free(mesh);
        }
    }
    if (material)
    {
        delete material;
        material = 0;
    }
    if (extension)
    {
        delete extension;
        extension = 0;
    }
    // Member destructors clear meshes, then materials, matching native order.
}

bool CModelGeometry::IsModified()
{
    if (modified || modifiedOther)
        return true;
    return false;
}

DWORD CModelGeometry::GetError()
{
    return error;
}

DWORD CModelGeometry::GetDetail()
{
    return detail;
}

void CModelGeometry::SetError(DWORD value)
{
    if (!error)
        error = value;
}

void CModelGeometry::SetDetail(DWORD value)
{
    detail = value;
}

CDffFile::CDffFile()
{
    error = 0;
    detail = 0;
    fileLength = 0;
    root = 0;
    structure = 0;
    frames = 0;
    geometryList = 0;
    atomic = 0;
    extension = 0;
    hasCollision = 0;
    frameCount = 0;
    frameData = 0;
}

CDffFile::~CDffFile()
{
    if (extension)
    {
        delete extension;
        extension = 0;
    }
    if (atomic)
    {
        delete atomic;
        atomic = 0;
    }
    if (geometryList)
    {
        delete geometryList;
        geometryList = 0;
    }
    if (frames)
    {
        delete frames;
        frames = 0;
    }
    if (structure)
    {
        delete structure;
        structure = 0;
    }
    if (root)
    {
        delete root;
        root = 0;
    }
    if (frameData)
    {
        ::operator delete(frameData);
        frameData = 0;
    }
    int count = (int)geometries.count;
    for (DWORD i = 0; (int)i < count; ++i)
        if (i < geometries.count && geometries.items[i])
            delete geometries.items[i];
    // Native explicit clear precedes the geometry-array member destructor.
    geometries.Resize(0);
}

CModelFileChunk* CModelGeometry::ReadChunk(CModelFileStream* input)
{
    bool truncated = false;
    CModelFileChunk* chunk = ModelReadChunk(input, &truncated);
    if (truncated && !error)
        error = 0xabf4;
    return chunk;
}

DWORD CModelMaterialArray::Insert(void* entry)
{
    DWORD previous = count, index = 0;
    if (previous)
    {
        void** entries = items;
        void** slot = entries;
        do
        {
            if (!*slot)
            {
                if ((LONG)index >= 0)
                {
                    entries[index] = entry;
                    return index;
                }
                break;
            }
            ++index;
            ++slot;
        }
        while (index != previous);
    }
    if (!Resize(previous + 1))
        return 0xffffffff;
    items[previous] = entry;
    return previous;
}

DWORD CModelMeshArray::Insert(BYTE* entry)
{
    DWORD previous = count, index = 0;
    if (previous)
    {
        BYTE** entries = items;
        BYTE** slot = entries;
        do
        {
            if (!*slot)
            {
                if ((LONG)index >= 0)
                {
                    entries[index] = entry;
                    return index;
                }
                break;
            }
            ++index;
            ++slot;
        }
        while (index != previous);
    }
    if (!Resize(previous + 1))
        return 0xffffffff;
    items[previous] = entry;
    return previous;
}

DWORD CModelFileStream::Skip(DWORD count)
{
    DWORD left = position < length ? length - position : 0;
    if (count < left)
    {
        position += count;
        return position;
    }
    position = length;
    ended = 1;
    return length;
}

bool CModelGeometry::ReadMaterial(CModelFileStream* input)
{
    CModelMaterialRecord* record = (CModelMaterialRecord*)calloc(1, 0xa4);
    DWORD header[3], nested[3], failure = 9, remaining = 0;
    if (!record)
    {
        if (!error)
            error = 0xabe1;
        return false;
    }
    input->Read(header, 12);
    if (header[0] != 1 || header[1] != 28)
        goto malformed;
    for (unsigned field = 0; field < 7; ++field)
    {
        DWORD value = 0;
        input->Read(&value, 4);
        record->values[field] = value;
    }
    if (record->values[3] > 2)
    {
        if (!error)
            error = 0xabe8;
        detail = record->values[3];
        free(record);
        return false;
    }
    for (DWORD i = 0; i != record->values[3]; ++i)
    {
        input->Read(header, 12);
        if (header[0] != 6)
            continue;
        input->Read(header, 12);
        if (header[0] != 1 || header[1] != 4)
        {
            failure = 4;
            goto malformed;
        }
        DWORD value = 0;
        input->Read(&value, 4);
        record->textures[i].flags = value;
        input->Read(header, 12);
        if (header[0] != 2 || header[1] > 32)
        {
            failure = 5;
            goto malformed;
        }
        input->Read(record->textures[i].name, header[1]);
        // 32 bytes without a nul
        if (record->textures[i].name[31])
        {
            failure = 5;
            goto malformed;
        }
        input->Read(header, 12);
        if (header[0] != 2 || header[1] > 32)
        {
            failure = 6;
            goto malformed;
        }
        input->Read(record->textures[i].mask, header[1]);
        if (record->textures[i].mask[31])
        {
            failure = 6;
            goto malformed;
        }
        input->Read(header, 12);
        if (header[0] == 3)
        {
            if (header[1])
                input->Skip(header[1]);
        }
        else if (header[1])
        {
            failure = 7;
            goto malformed;
        }
    }
    input->Read(header, 12);
    if (header[0] != 3)
    {
        failure = 8;
        goto malformed;
    }
    remaining = header[1];
    while (remaining)
    {
        input->Read(nested, 12);
        if (remaining < 12)
        {
            failure = 8;
            goto malformed;
        }
        remaining -= 12;
        if (nested[0] != 0x253f2fc && nested[0] != 0x253f2f6 && nested[0] != 0x120 && nested[0] != 0x135)
        {
            if (!error)
                error = 0xabe2;
            detail = nested[0];
            free(record);
            return false;
        }
        if (nested[1] > remaining)
        {
            failure = 8;
            goto malformed;
        }
        input->Skip(nested[1]);
        remaining -= nested[1];
    }
    materials.Insert(record);
    return true;
malformed:
    if (!error)
        error = 0xabe7;
    detail = failure;
    if (record)
        free(record);
    return false;
}

bool CModelGeometry::ReadMaterials(CModelFileStream* input)
{
    material = ReadChunk(input);
    if (!material || material->type != 8)
    {
        if (!error)
            error = 0xabf3;
        return false;
    }
    DWORD header[3];
    material->stream.Read(header, 12);
    if (header[0] == 1)
    {
        DWORD count = 0;
        material->stream.Read(&count, 4);
        materialCount = (int)count;
        material->stream.Skip(header[1] - 4);
        if (materialCount <= 0 || materialCount >= 256)
        {
            if (!error)
                error = 0xabe9;
            return false;
        }
        for (int i = 0; i < materialCount; ++i)
        {
            material->stream.Read(header, 12);
            if (header[0] == 7 && !ReadMaterial(&material->stream))
            {
                if (!error)
                    error = 0xabe9;
                return false;
            }
        }
    }
    return true;
}

bool CModelGeometry::ReadSkin(CModelFileStream* input)
{
    if (vertexCount <= 0)
        return false;
    BYTE value = 0;
    input->Read(&value, 1);
    skin.boneCount = value;
    value = 0;
    input->Read(&value, 1);
    skin.usedBoneCount = value;
    value = 0;
    input->Read(&value, 1);
    skin.maxWeights = value;
    value = 0;
    input->Read(&value, 1);
    skin.unknown3 = value;
    if (skin.boneCount >= 64 || skin.usedBoneCount > skin.boneCount)
        return false;
    skin.usedBones = calloc(skin.usedBoneCount, 1);
    input->Read(skin.usedBones, skin.usedBoneCount);
    skin.indices = calloc(vertexCount, 4);
    input->Read(skin.indices, (DWORD)vertexCount * 4);
    skin.weights = calloc(vertexCount, 16);
    input->Read(skin.weights, (DWORD)vertexCount * 16);
    skin.matrices = calloc(skin.boneCount, 64);
    if (!skin.usedBones || !skin.indices || !skin.weights || !skin.matrices)
        return false;
    input->Read(skin.matrices, (DWORD)skin.boneCount * 64);
    DWORD word = 0;
    input->Read(&word, 4);
    skin.unknown14 = word;
    word = 0;
    input->Read(&word, 4);
    skin.unknown18 = word;
    word = 0;
    input->Read(&word, 4);
    skin.unknown1c = word;
    return !skin.unknown14 && !skin.unknown18 && !skin.unknown1c;
}

bool CModelGeometry::ReadExtensions(CModelFileStream* input)
{
    extension = ReadChunk(input);
    if (!extension || extension->type != 3)
    {
        if (!error)
            error = 0xabf2;
        detail = 0;
        return false;
    }
    while (extension->stream.Remaining() > 12)
    {
        DWORD header[3];
        extension->stream.Read(header, 12);
        switch (header[0])
        {
            case 0x50e:
            {
                hasBinMesh = 1;
                DWORD value = 0;
                extension->stream.Read(&value, 4);
                binMeshFlags = value;
                value = 0;
                extension->stream.Read(&value, 4);
                binMeshCount = value;
                value = 0;
                extension->stream.Read(&value, 4);
                binMeshIndexCount = value;
                if (binMeshCount >= 100000)
                {
                    if (!error)
                        error = 0xabe5;
                    return false;
                }
                DWORD summedIndices = 0;
                for (DWORD i = 0; i < binMeshCount; ++i)
                {
                    DWORD indexCount = 0, materialIndex = 0;
                    extension->stream.Read(&indexCount, 4);
                    extension->stream.Read(&materialIndex, 4);
                    // the count has to fit in what is left of the chunk, and the material has to exist
                    if (indexCount > extension->stream.Remaining() / 4 || materialIndex >= (DWORD)materialCount)
                    {
                        if (!error)
                            error = 0xabf9;
                        detail = i;
                        return false;
                    }
                    BYTE* mesh = (BYTE*)calloc(1, 12);
                    DWORD* indices = (DWORD*)calloc(indexCount ? indexCount : 1, 4);
                    if (!mesh || !indices)
                    {
                        free(mesh);
                        free(indices);
                        if (!error)
                            error = 0xabe1;
                        return false;
                    }
                    *(DWORD*)mesh = indexCount;
                    *(DWORD*)(mesh + 4) = materialIndex;
                    *(void**)(mesh + 8) = indices;
                    extension->stream.Read(indices, indexCount * 4);
                    summedIndices += indexCount;
                    bool valid = true;
                    for (DWORD k = 0; k < indexCount; ++k)
                        if (indices[k] >= (DWORD)vertexCount)
                            valid = false;
                    if (!valid || meshes.Insert(mesh) == 0xffffffff)
                    {
                        free(indices);
                        free(mesh);
                        if (!error)
                            error = 0xabf9;
                        detail = i;
                        return false;
                    }
                }
                if (summedIndices != binMeshIndexCount)
                {
                    if (!error)
                        error = 0xabf9;
                    detail = summedIndices;
                    return false;
                }
                break;
            }
            case 0x253f2fd:
                extension->stream.Skip(header[1]);
                break;
            case 0x253f2f9:
                if (((header[1] - 4) >> 2) == (DWORD)vertexCount)
                {
                    hasSecondaryColors = 1;
                    extension->stream.Skip(4);
                    secondaryColors = calloc(vertexCount, 4);
                    if (!secondaryColors || extension->stream.Remaining() < (DWORD)vertexCount * 4)
                    {
                        if (!error)
                            error = 0xabe1;
                        detail = 0x253f2f9;
                        return false;
                    }
                    extension->stream.Read(secondaryColors, (DWORD)vertexCount * 4);
                }
                break;
            case 0x116:
            {
                DWORD previous = extension->stream.position;
                if (!ReadSkin(&extension->stream) || extension->stream.position - previous != header[1])
                {
                    if (!error)
                        error = 0xabe7;
                    detail = 0x116;
                    return false;
                }
                break;
            }
            case 0x105:
            case 0x253f2f8:
                extension->stream.Skip(header[1]);
                break;
            default:
                if (!error)
                    error = 0xabe2;
                detail = header[0];
                return false;
        }
    }
    return true;
}

bool CModelGeometry::Read(CModelFileStream* input, WORD version)
{
    DWORD value = 0;
    input->Read(&value, 2);
    flags = (WORD)value;
    value = 0;
    input->Read(&value, 2);
    textureSets = (WORD)value;
    value = 0;
    input->Read(&value, 4);
    triangleCount = (int)value;
    value = 0;
    input->Read(&value, 4);
    vertexCount = (int)value;
    value = 0;
    input->Read(&value, 4);
    morphCount = (int)value;
    if ((WORD)((version >> 6) + 0x300) < 0x340)
    {
        value = 0;
        input->Read(&value, 4);
        value = 0;
        input->Read(&value, 4);
        value = 0;
        input->Read(&value, 4);
    }
    if (input->ended)
        goto incomplete;
    if (triangleCount < 0 || triangleCount > 100000)
    {
        if (!error)
            error = 0xabe5;
        return false;
    }
    if (vertexCount < 0 || vertexCount > 200000)
    {
        if (!error)
            error = 0xabe4;
        return false;
    }
    // only the first morph target is laid out, anything else shifts the rest of the data
    if (morphCount != 1)
    {
        if (!error)
            error = 0xabf5;
        detail = morphCount;
        return false;
    }
    if (flags & 8)
    {
        hasColors = 1;
        colors = calloc(vertexCount, 4);
        if (!colors || input->Remaining() < (DWORD)vertexCount * 4)
            goto incomplete;
        input->Read(colors, (DWORD)vertexCount * 4);
    }
    if (flags & 0x84)
    {
        uv = calloc(vertexCount, 8);
        if (!uv || input->Remaining() < (DWORD)vertexCount * 8)
            goto incomplete;
        input->Read(uv, (DWORD)vertexCount * 8);
    }
    triangles = calloc(triangleCount, 8);
    if (!triangles || input->Remaining() < (DWORD)triangleCount * 8)
        goto incomplete;
    input->Read(triangles, (DWORD)triangleCount * 8);
    if (input->ended)
        goto incomplete;
    input->Read(sphere, 16);
    hasVertices = input->ReadDword();
    hasNormals = input->ReadDword();
    if (hasVertices)
    {
        vertices = calloc(vertexCount, 12);
        if (!vertices || input->Remaining() < (DWORD)vertexCount * 12)
            goto incomplete;
        input->Read(vertices, (DWORD)vertexCount * 12);
    }
    if (flags & 0x10)
    {
        normals = calloc(vertexCount, 12);
        if (!normals || input->Remaining() < (DWORD)vertexCount * 12)
            goto incomplete;
        input->Read(normals, (DWORD)vertexCount * 12);
    }
    if (!ReadMaterials(input))
    {
        if (!error)
            error = 0xabea;
        return false;
    }
    // triangle: vertex2, vertex1, material, vertex3 (words)
    {
        const WORD* triangle = (const WORD*)triangles;
        for (int i = 0; i < triangleCount; ++i, triangle += 4)
        {
            if (triangle[0] >= vertexCount || triangle[1] >= vertexCount || triangle[3] >= vertexCount || triangle[2] >= materialCount)
            {
                if (!error)
                    error = 0xabf8;
                detail = i;
                return false;
            }
        }
    }
    if (!ReadExtensions(input))
    {
        if (!error)
            error = 0xabeb;
        return false;
    }
    return true;
incomplete:
    if (!error)
        error = 0xabe1;
    return false;
}

DWORD CDffFile::ReadGeometries()
{
    if (!geometryList)
        return 0;
    geometryList->stream.position = 0;
    geometryList->stream.ended = 0;
    DWORD header[3];
    geometryList->stream.Read(header, 12);
    if (header[0] != 1 || header[1] != 4)
    {
        if (!error)
            error = 0xabe7;
        return 0;
    }
    DWORD count = 0;
    geometryList->stream.Read(&count, 4);
    if (count != 1)
    {
        if (!error)
            error = 0xabe6;
        return 0;
    }
    CModelFileChunk* chunk = ReadChunk(&geometryList->stream);
    if (chunk)
    {
        if (chunk->type == 15)
        {
            chunk->stream.Read(header, 12);
            if (header[0] == 1)
            {
                CModelGeometry* geometry = new CModelGeometry;
                if (!geometry)
                {
                    if (!error)
                        error = 0xabe1;
                    delete chunk;
                    return 0;
                }
                if (!geometry->Read(&chunk->stream, chunk->version))
                {
                    if (!error)
                        error = geometry->error;
                    detail = geometry->detail;
                    delete geometry;
                    delete chunk;
                    return 0;
                }
                geometries.Insert(geometry);
            }
        }
        delete chunk;
    }
    // the list says one geometry, so one has to be there
    if (error || geometries.GetCount() != 1)
    {
        if (!error)
            error = 0xabe6;
        return 0;
    }
    return 1;
}

// atomic struct: frame index, geometry index, flags
DWORD CDffFile::ReadAtomic()
{
    if (!atomic)
        return 1;
    atomic->stream.position = 0;
    atomic->stream.ended = 0;
    DWORD header[3];
    atomic->stream.Read(header, 12);
    if (header[0] != 1 || header[1] < 8 || header[1] > atomic->stream.Remaining())
    {
        if (!error)
            error = 0xabf7;
        return 0;
    }
    DWORD frameIndex = 0, geometryIndex = 0;
    atomic->stream.Read(&frameIndex, 4);
    atomic->stream.Read(&geometryIndex, 4);
    if (frameIndex >= frameCount || geometryIndex >= geometries.GetCount())
    {
        if (!error)
            error = 0xabf7;
        detail = frameIndex;
        return 0;
    }
    return 1;
}

void CModelFileStream::SetBuffer(BYTE* buffer, DWORD size)
{
    data = buffer;
    length = size;
    position = 0;
    ended = 0;
    owned = 0;
}

DWORD CModelFileStream::WriteByte(BYTE value)
{
    return Write(&value, 1);
}

WORD CModelFileStream::ReadWord()
{
    WORD value = 0;
    Read(&value, 2);
    return value;
}

BYTE* CModelFileStream::GetBuffer()
{
    return data;
}

DWORD CModelFileStream::GetLength()
{
    return length;
}

CModelFileChunk::CModelFileChunk()
{
    type = 0;
    length = 0;
    build = 0xffff;
    version = 0x1803;
}

void CModelFileChunk::SetBuffer(BYTE* buffer, DWORD size)
{
    stream.data = buffer;
    stream.length = size;
    stream.position = 0;
    stream.ended = 0;
    stream.owned = 0;
}

void CModelFileChunk::UpdateLength()
{
    length = stream.length;
}

void CDffFile::SetError(DWORD value)
{
    if (!error)
        error = value;
}

void CDffFile::SetDetail(DWORD value)
{
    detail = value;
}

DWORD CDffFile::AppendChunk(CModelFileChunk* parent, const CModelFileChunk* child)
{
    if (!parent || !child)
        return 0;
    parent->stream.Write(child, 12);
    parent->stream.Write(child->stream.data, child->stream.length);
    parent->length += child->stream.length + 12;
    return 1;
}

DWORD CDffFile::AppendCollisionFile(const char* filename)
{
    CModelFileChunk chunk;
    if (!root || !extension)
        return (DWORD)-1;
    if (!chunk.stream.LoadFile(filename))
        return (DWORD)-2;
    chunk.length = chunk.stream.length;
    chunk.type = 0x253f2ff;
    return AppendChunk(extension, &chunk);
}

DWORD __stdcall ModelSaveChunk(const CModelFileChunk* chunk, const char* filename)
{
    CModelFileStream output;
    output.Write(chunk, 12);
    output.Write(chunk->stream.data, chunk->stream.length);
    return output.SaveFile(filename);
}

bool CModelGeometry::CopySecondaryColors(const CModelGeometry* other)
{
    if (!other->hasColors)
        return false;
    if (secondaryColors)
    {
        free(secondaryColors);
        secondaryColors = 0;
    }
    secondaryColors = calloc(vertexCount, 4);
    memcpy(secondaryColors, colors, (DWORD)vertexCount * 4);
    int count = other->vertexCount;
    if (count > vertexCount)
        count = vertexCount;
    memcpy(secondaryColors, other->colors, (DWORD)count * 4);
    hasSecondaryColors = 1;
    modified = 1;
    return true;
}

CModelFileChunk* CModelGeometry::BuildStructure()
{
    CModelFileChunk* chunk = new CModelFileChunk;
    chunk->type = 1;
    DWORD value = flags;
    chunk->stream.Write(&value, 2);
    value = textureSets;
    chunk->stream.Write(&value, 2);
    value = (DWORD)triangleCount;
    chunk->stream.Write(&value, 4);
    value = (DWORD)vertexCount;
    chunk->stream.Write(&value, 4);
    value = (DWORD)morphCount;
    chunk->stream.Write(&value, 4);
    if ((flags & 8) && hasColors)
        chunk->stream.Write(colors, (DWORD)vertexCount * 4);
    if (flags & 0x84)
        chunk->stream.Write(uv, (DWORD)vertexCount * 8);
    chunk->stream.Write(triangles, (DWORD)triangleCount * 8);
    chunk->stream.Write(sphere, 16);
    value = hasVertices;
    chunk->stream.Write(&value, 4);
    value = hasNormals;
    chunk->stream.Write(&value, 4);
    if (hasVertices && vertices)
        chunk->stream.Write(vertices, (DWORD)vertexCount * 12);
    if ((flags & 0x10) && normals)
        chunk->stream.Write(normals, (DWORD)vertexCount * 12);
    chunk->length = chunk->stream.length;
    return chunk;
}

bool CModelGeometry::RemoveExtension(DWORD type)
{
    CModelFileChunk* replacement = new CModelFileChunk;
    replacement->type = 3;
    extension->stream.Reset();
    while (extension->stream.Remaining() >= 12)
    {
        DWORD header[3];
        extension->stream.Read(header, 12);
        if (header[0] != type)
        {
            replacement->stream.Write(header, 12);
            replacement->stream.Write(extension->stream.Current(), header[1]);
        }
        extension->stream.Skip(header[1]);
    }
    replacement->length = replacement->stream.length;
    if (extension)
    {
        delete extension;
        extension = 0;
    }
    extension = replacement;
    return true;
}

CModelFileChunk* CModelGeometry::BuildChunk()
{
    CModelFileChunk* chunk = new CModelFileChunk;
    CModelFileChunk* structure = BuildStructure();
    chunk->type = 0xf;
    ModelAppendChunk(chunk, structure);
    if (structure)
        delete structure;
    ModelAppendChunk(chunk, material);
    if (secondaryColors)
    {
        CModelFileChunk* secondary = new CModelFileChunk;
        secondary->type = 0x253f2f9;
        DWORD marker = 0x37505050;
        secondary->stream.Write(&marker, 4);
        secondary->stream.Write(secondaryColors, (DWORD)vertexCount * 4);
        secondary->length = secondary->stream.length;
        RemoveExtension(0x253f2f9);
        ModelAppendChunk(extension, secondary);
        delete secondary;
    }
    ModelAppendChunk(chunk, extension);
    return chunk;
}

bool CDffFile::RebuildGeometryList()
{
    if (!geometries.count)
        return false;
    CModelGeometry* geometry = geometries.items[0];
    if (!geometry || !geometry->IsModified())
        return false;
    CModelFileChunk* replacement = new CModelFileChunk;
    CModelFileChunk* structure = new CModelFileChunk;
    replacement->type = 0x1a;
    structure->type = 1;
    DWORD count = 1;
    structure->stream.Write(&count, 4);
    structure->length = structure->stream.length;
    CModelFileChunk* body = geometry->BuildChunk();
    AppendChunk(replacement, structure);
    AppendChunk(replacement, body);
    delete structure;
    if (body)
        delete body;
    if (geometryList)
    {
        delete geometryList;
        geometryList = 0;
    }
    geometryList = replacement;
    return true;
}

DWORD CDffFile::SaveFile(const char* filename)
{
    CModelFileStream output;
    if (!root)
        return 0;
    root->length = geometryList->length + extension->length + frames->length + structure->length + atomic->length + 60;
    output.Write(root, 12);
    structure->length = 12;
    output.Write(structure, 12);
    structure->stream.Reset();
    DWORD value = 0;
    structure->stream.Read(&value, 4);
    output.Write(&value, 4);
    value = 0;
    output.Write(&value, 4);
    value = 0;
    output.Write(&value, 4);
    output.Write(frames, 12);
    output.Write(frames->stream.data, frames->stream.length);
    output.Write(geometryList, 12);
    output.Write(geometryList->stream.data, geometryList->stream.length);
    output.Write(atomic, 12);
    output.Write(atomic->stream.data, atomic->stream.length);
    output.Write(extension, 12);
    output.Write(extension->stream.data, extension->stream.length);
    return output.SaveFile(filename);
}

DWORD CDffFile::SaveCompressedFile(const char* filename)
{
    CModelFileStream output;
    if (!root)
        return 0;
    DWORD value = 0x504d4153;
    output.Write(&value, 4);
    BYTE mode = 1;
    output.Write(&mode, 1);
    value = root->stream.length;
    output.Write(&value, 4);
    root->stream.Compress();
    value = root->stream.length;
    output.Write(&value, 4);
    output.Write(root->stream.data, root->stream.length);
    return output.SaveFile(filename);
}

CModelFileStream::CModelFileStream(BYTE* buffer, DWORD size)
{
    data = buffer;
    length = size;
    position = 0;
    ended = 0;
    owned = 0;
    compressed = 0;
    scratchLength = 0;
    scratch = 0;
}

float CModelFileStream::ReadFloat()
{
    float value = 0;
    Read(&value, 4);
    return value;
}

int CModelFileStream::ReadInt()
{
    int value = 0;
    Read(&value, 4);
    return value;
}

void CModelGeometryArray::Clear()
{
    Resize(0);
}

void CModelMaterialArray::Clear()
{
    Resize(0);
}

void CModelMeshArray::Clear()
{
    Resize(0);
}

CModelMaterialArray::~CModelMaterialArray()
{
    Resize(0);
}

CModelMeshArray::~CModelMeshArray()
{
    Resize(0);
}

CModelGeometryArray::~CModelGeometryArray()
{
    Resize(0);
}

extern "C" __int64 __cdecl _ftol2();

DWORD CModelCollisionFile::SetInitialHeaderSize()
{
    *(DWORD*)(nativeState + 4) = 0x74;
    return 1;
}

DWORD CModelCollisionFile::CalculateGeometryBounds(const CModelGeometry* geometry, float* minimum, float* maximum)
{
    minimum[0] = 100000.0f;
    minimum[1] = 100000.0f;
    minimum[2] = 100000.0f;
    maximum[0] = -100000.0f;
    maximum[1] = -100000.0f;
    maximum[2] = -100000.0f;
    if (!geometry->vertices)
        return 0;
    for (DWORD i = 0; i != (DWORD)geometry->vertexCount; ++i)
    {
        if (((float*)geometry->vertices)[i * 3] < minimum[0])
            minimum[0] = ((float*)geometry->vertices)[i * 3];
        if (((float*)geometry->vertices)[i * 3 + 1] < minimum[1])
            minimum[1] = ((float*)geometry->vertices)[i * 3 + 1];
        if (((float*)geometry->vertices)[i * 3 + 2] < minimum[2])
            minimum[2] = ((float*)geometry->vertices)[i * 3 + 2];
        if (((float*)geometry->vertices)[i * 3] > maximum[0])
            maximum[0] = ((float*)geometry->vertices)[i * 3];
        if (((float*)geometry->vertices)[i * 3 + 1] > maximum[1])
            maximum[1] = ((float*)geometry->vertices)[i * 3 + 1];
        if (((float*)geometry->vertices)[i * 3 + 2] > maximum[2])
            maximum[2] = ((float*)geometry->vertices)[i * 3 + 2];
    }
    return 1;
}

DWORD CModelCollisionFile::SetGeometryBounds(const CModelGeometry* geometry)
{
    CalculateGeometryBounds(geometry, (float*)(nativeState + 0x20), (float*)(nativeState + 0x2c));
    memcpy(nativeState + 0x44, geometry->sphere + 3, 4);
    return 1;
}

DWORD CModelCollisionFile::AddSphere(ModelCollisionSphere* sphere)
{
    return ((CModelCollisionSphereArray*)(nativeState + 0x80))->Insert(sphere);
}

DWORD CModelCollisionFile::AddBox(ModelCollisionBox* box)
{
    return ((CModelCollisionBoxArray*)(nativeState + 0x78))->Insert(box);
}

DWORD CModelCollisionFile::BuildBoxFromGeometry(const CModelGeometry* geometry)
{
    CalculateGeometryBounds(geometry, (float*)(nativeState + 0x20), (float*)(nativeState + 0x2c));
    memcpy(nativeState + 0x44, geometry->sphere + 3, 4);
    ModelCollisionBox* box = (ModelCollisionBox*)calloc(1, 28);
    memcpy(box->bounds.minimum, nativeState + 0x20, 12);
    memcpy(box->bounds.maximum, nativeState + 0x2c, 12);
    box->data[3] = box->data[2] = 255;
    box->data[0] = 0;
    AddBox(box);
    return 1;
}

DWORD CModelCollisionFile::BuildGeometry(const CModelGeometry* geometry)
{
    CalculateGeometryBounds(geometry, (float*)(nativeState + 0x20), (float*)(nativeState + 0x2c));
    memcpy(nativeState + 0x44, geometry->sphere + 3, 4);
    if (geometry->hasVertices)
    {
        if (*(void**)(nativeState + 0x88))
        {
            free(*(void**)(nativeState + 0x88));
            *(void**)(nativeState + 0x88) = 0;
        }
        *(void**)(nativeState + 0x88) = calloc(geometry->triangleCount, 8);
        *(DWORD*)(nativeState + 0x8c) = geometry->triangleCount;
        for (DWORD i = 0; i != (DWORD)geometry->triangleCount; ++i)
        {
            (*(ModelCollisionTriangle**)(nativeState + 0x88))[i].vertices[0] = ((WORD*)geometry->triangles)[i * 4 + 3];
            (*(ModelCollisionTriangle**)(nativeState + 0x88))[i].vertices[1] = ((WORD*)geometry->triangles)[i * 4];
            (*(ModelCollisionTriangle**)(nativeState + 0x88))[i].vertices[2] = ((WORD*)geometry->triangles)[i * 4 + 1];
            (*(ModelCollisionTriangle**)(nativeState + 0x88))[i].surface = 0;
            (*(ModelCollisionTriangle**)(nativeState + 0x88))[i].lighting = 255;
        }
        if (*(void**)(nativeState + 0x90))
        {
            free(*(void**)(nativeState + 0x90));
            *(void**)(nativeState + 0x90) = 0;
        }
        *(void**)(nativeState + 0x90) = calloc(geometry->vertexCount, 6);
        *(DWORD*)(nativeState + 0x94) = geometry->vertexCount;
        for (DWORD i = 0; i != (DWORD)geometry->vertexCount; ++i)
        {
            (*(ModelCollisionVertex**)(nativeState + 0x90))[i].values[0] = (SHORT)(((const float*)geometry->vertices)[i * 3 + 0] * g_modelCollisionVertexScale);
            (*(ModelCollisionVertex**)(nativeState + 0x90))[i].values[1] = (SHORT)(((const float*)geometry->vertices)[i * 3 + 1] * g_modelCollisionVertexScale);
            (*(ModelCollisionVertex**)(nativeState + 0x90))[i].values[2] = (SHORT)(((const float*)geometry->vertices)[i * 3 + 2] * g_modelCollisionVertexScale);
        }
        return 1;
    }
    return 0;
}

bool CModelCollisionFile::ContainsVertex(const ModelCollisionVertex* vertex, const ModelCollisionBounds* bounds)
{
    float point[3];
    point[0] = vertex->values[0] * g_modelCollisionVertexInvScale;
    point[1] = vertex->values[1] * g_modelCollisionVertexInvScale;
    point[2] = vertex->values[2] * g_modelCollisionVertexInvScale;
    if (point[0] >= bounds->minimum[0] && point[0] <= bounds->maximum[0] && point[1] >= bounds->minimum[1] && point[1] <= bounds->maximum[1] && point[2] >= bounds->minimum[2] &&
        point[2] <= bounds->maximum[2])
        return true;
    return false;
}

bool CModelCollisionFile::TriangleHasVertexInBounds(const ModelCollisionTriangle* triangle, const ModelCollisionBounds* bounds)
{
    ModelCollisionVertex* vertices = *(ModelCollisionVertex**)(nativeState + 0x90);
    if (ContainsVertex(vertices + triangle->vertices[0], bounds) || ContainsVertex(vertices + triangle->vertices[1], bounds) ||
        ContainsVertex(vertices + triangle->vertices[2], bounds))
        return true;
    return false;
}

void __stdcall ModelCollisionMinVertex(ModelCollisionVertex* target, const ModelCollisionVertex* value)
{
    if (value->values[0] < target->values[0])
        target->values[0] = value->values[0];
    if (value->values[1] < target->values[1])
        target->values[1] = value->values[1];
    if (value->values[2] < target->values[2])
        target->values[2] = value->values[2];
}

void __stdcall ModelCollisionMaxVertex(ModelCollisionVertex* target, const ModelCollisionVertex* value)
{
    if (value->values[0] > target->values[0])
        target->values[0] = value->values[0];
    if (value->values[1] > target->values[1])
        target->values[1] = value->values[1];
    if (value->values[2] > target->values[2])
        target->values[2] = value->values[2];
}

void* CModelCollisionBoxArray::Get(DWORD index)
{
    if (index >= count)
        return 0;
    return items[index];
}

DWORD CModelCollisionBoxArray::GetCount()
{
    return count;
}

bool CModelCollisionBoxArray::Resize(DWORD size)
{
    if (!size)
    {
        if (items)
        {
            free(items);
            items = 0;
        }
        count = 0;
        return true;
    }
    void** resized = (void**)realloc(items, size * 4);
    if (!resized)
        return false;
    items = resized;
    DWORD i = count;
    if (i < size)
    {
        do
        {
            items[i] = 0;
        }
        while (++i != size);
    }
    count = size;
    return true;
}

DWORD CModelCollisionBoxArray::Insert(void* value)
{
    DWORD previous = count, index = 0;
    if (previous)
    {
        void** entries = items;
        void** slot = entries;
        do
        {
            if (!*slot)
            {
                if ((LONG)index >= 0)
                {
                    entries[index] = value;
                    return index;
                }
                break;
            }
            ++index;
            ++slot;
        }
        while (index != previous);
    }
    if (!Resize(previous + 1))
        return 0xffffffff;
    items[previous] = value;
    return previous;
}

void* CModelCollisionSphereArray::Get(DWORD index)
{
    if (index >= count)
        return 0;
    return items[index];
}

DWORD CModelCollisionSphereArray::GetCount()
{
    return count;
}

bool CModelCollisionSphereArray::Resize(DWORD size)
{
    if (!size)
    {
        if (items)
        {
            free(items);
            items = 0;
        }
        count = 0;
        return true;
    }
    void** resized = (void**)realloc(items, size * 4);
    if (!resized)
        return false;
    items = resized;
    DWORD i = count;
    if (i < size)
    {
        do
        {
            items[i] = 0;
        }
        while (++i != size);
    }
    count = size;
    return true;
}

DWORD CModelCollisionSphereArray::Insert(void* value)
{
    DWORD previous = count, index = 0;
    if (previous)
    {
        void** entries = items;
        void** slot = entries;
        do
        {
            if (!*slot)
            {
                if ((LONG)index >= 0)
                {
                    entries[index] = value;
                    return index;
                }
                break;
            }
            ++index;
            ++slot;
        }
        while (index != previous);
    }
    if (!Resize(previous + 1))
        return 0xffffffff;
    items[previous] = value;
    return previous;
}

// Shared native half constant, already maintained by textdraw.

CModelCollisionNode::CModelCollisionNode()
{
    right = 0;
    left = 0;
}

CModelCollisionNode::~CModelCollisionNode()
{
    delete left;
    delete right;
}

void CModelCollisionNode::CountLeaves(DWORD* count)
{
    if (left)
    {
        left->CountLeaves(count);
        right->CountLeaves(count);
        return;
    }
    if (triangles.size() > 0)
        ++*count;
}

void CModelCollisionNode::WriteLeaves(CModelCollisionFile* file)
{
    CModelCollisionNode* node = this;
    while (node->left)
    {
        node->left->WriteLeaves(file);
        node = node->right;
    }
    if (node->triangles.empty())
        return;
    BYTE* state = file->nativeState;
    ModelCollisionFaceGroup* groups = *(ModelCollisionFaceGroup**)(state + 0x98);
    DWORD group = *(DWORD*)(state + 0x9c);
    groups[group].first = (WORD) * (DWORD*)(state + 0x8c);
    groups[group].last = (WORD)(groups[group].first + node->triangles.size() - 1);
    ModelCollisionVertex* vertices = *(ModelCollisionVertex**)(state + 0x90);
    ModelCollisionVertex minimum = vertices[node->triangles[0].vertices[0]];
    ModelCollisionVertex maximum = vertices[node->triangles[0].vertices[0]];
    for (DWORD i = 0; i < node->triangles.size(); ++i)
    {
        ModelCollisionTriangle* output = *(ModelCollisionTriangle**)(state + 0x88);
        memcpy(output + *(DWORD*)(state + 0x8c) + i, &node->triangles[i], 8);
        vertices = *(ModelCollisionVertex**)(state + 0x90);
        ModelCollisionTriangle* triangle = &node->triangles[i];
        ModelCollisionMinVertex(&minimum, vertices + triangle->vertices[0]);
        ModelCollisionMinVertex(&minimum, vertices + triangle->vertices[1]);
        ModelCollisionMinVertex(&minimum, vertices + triangle->vertices[2]);
        ModelCollisionMaxVertex(&maximum, vertices + triangle->vertices[0]);
        ModelCollisionMaxVertex(&maximum, vertices + triangle->vertices[1]);
        ModelCollisionMaxVertex(&maximum, vertices + triangle->vertices[2]);
    }
    for (int axis = 0; axis < 3; ++axis)
    {
        (*(ModelCollisionFaceGroup**)(state + 0x98))[*(DWORD*)(state + 0x9c)].bounds.minimum[axis] = minimum.values[axis] * g_modelCollisionVertexInvScale;
    }
    for (int axis = 0; axis < 3; ++axis)
    {
        (*(ModelCollisionFaceGroup**)(state + 0x98))[*(DWORD*)(state + 0x9c)].bounds.maximum[axis] = maximum.values[axis] * g_modelCollisionVertexInvScale;
    }
    ++*(DWORD*)(state + 0x9c);
    *(DWORD*)(state + 0x8c) += node->triangles.size();
}

void CModelCollisionNode::Split(CModelCollisionFile* file)
{
    CModelCollisionNode* first = new CModelCollisionNode;
    left = first;
    CModelCollisionNode* second = new CModelCollisionNode;
    right = second;
    memcpy(&left->bounds, &bounds, 24);
    memcpy(&right->bounds, &bounds, 24);
    float delta[3];
    delta[0] = bounds.maximum[0] - bounds.minimum[0];
    delta[1] = bounds.maximum[1] - bounds.minimum[1];
    delta[2] = bounds.maximum[2] - bounds.minimum[2];
    int axis = (delta[0] > delta[1] && delta[0] > delta[2]) ? 0 : delta[1] > delta[2] ? 1 : 2;
    const double halfExtent = (double)delta[axis] * g_textDrawHalf;
    left->bounds.maximum[axis] = (float)(halfExtent + bounds.minimum[axis]);
    right->bounds.minimum[axis] = (float)(halfExtent + bounds.minimum[axis]);
    for (DWORD i = 0; i < triangles.size(); ++i)
    {
        if (file->TriangleHasVertexInBounds(&triangles[i], &left->bounds))
            left->triangles.push_back(triangles[i]);
        else
            right->triangles.push_back(triangles[i]);
    }
    // Native releases capacity as well as elements, with delete before pointer clear.
    triangles.~ModelCollisionTriangleVector();
    new (&triangles) ModelCollisionTriangleVector;
    if (left->triangles.size() > 80)
        left->Split(file);
    if (right->triangles.size() > 80)
        right->Split(file);
}

DWORD CModelCollisionFile::BuildFaceGroups()
{
    CModelCollisionNode root;
    for (DWORD i = 0; i < *(DWORD*)(nativeState + 0x8c); ++i)
        root.triangles.push_back((*(ModelCollisionTriangle**)(nativeState + 0x88))[i]);
    memcpy(&root.bounds.maximum, nativeState + 0x2c, 12);
    memcpy(&root.bounds.minimum, nativeState + 0x20, 12);
    root.Split(this);
    DWORD count = 0;
    if (root.left)
    {
        root.left->CountLeaves(&count);
        root.right->CountLeaves(&count);
    }
    else if (root.triangles.size() > 0)
        count = 1;
    *(void**)(nativeState + 0x98) = calloc(count, 28);
    *(DWORD*)(nativeState + 0x9c) = 0;
    *(DWORD*)(nativeState + 0x8c) = 0;
    root.WriteLeaves(this);
    return count;
}

DWORD CModelCollisionFile::SaveFile(const char* filename, const char* modelName)
{
    CModelFileStream output;
    const size_t nameLength = strnlen(modelName, 20);
    memset(nativeState + 8, 0, 20);
    memcpy(nativeState + 8, modelName, nameLength);
    SetInitialHeaderSize();
    output.Write(nativeState, 0x78);
    DWORD total = *(DWORD*)(nativeState + 0x84);
    if (total)
    {
        DWORD offset = output.position - 4;
        *(WORD*)(nativeState + 0x48) = (WORD)total;
        for (DWORD i = 0; i != total; ++i)
            if (i < *(DWORD*)(nativeState + 0x84) && (*(void***)(nativeState + 0x80))[i])
            {
                void* entry = i < *(DWORD*)(nativeState + 0x84) ? (*(void***)(nativeState + 0x80))[i] : 0;
                output.Write(entry, 20);
            }
        *(DWORD*)(nativeState + 4) += output.position - offset - 4;
        *(DWORD*)(nativeState + 0x54) = offset;
    }
    total = *(DWORD*)(nativeState + 0x7c);
    if (total)
    {
        DWORD offset = output.position - 4;
        *(WORD*)(nativeState + 0x4a) = (WORD)total;
        for (DWORD i = 0; i != total; ++i)
            if (i < *(DWORD*)(nativeState + 0x7c) && (*(void***)(nativeState + 0x78))[i])
            {
                void* entry = i < *(DWORD*)(nativeState + 0x7c) ? (*(void***)(nativeState + 0x78))[i] : 0;
                output.Write(entry, 28);
            }
        *(DWORD*)(nativeState + 4) += output.position - offset - 4;
        *(DWORD*)(nativeState + 0x58) = offset;
    }
    if (*(DWORD*)(nativeState + 0x9c))
    {
        DWORD offset = output.position;
        *(DWORD*)(nativeState + 0x50) |= 8;
        output.Write(*(void**)(nativeState + 0x98), *(DWORD*)(nativeState + 0x9c) * 28);
        *(DWORD*)(nativeState + 4) += output.position - offset;
        DWORD count = *(DWORD*)(nativeState + 0x9c);
        output.Write(&count, 4);
        *(DWORD*)(nativeState + 4) += 4;
    }
    if (*(DWORD*)(nativeState + 0x8c))
    {
        DWORD offset = output.position - 4;
        DWORD count = *(DWORD*)(nativeState + 0x8c);
        *(WORD*)(nativeState + 0x4c) = (WORD)count;
        output.Write(*(void**)(nativeState + 0x88), count * 8);
        *(DWORD*)(nativeState + 4) += output.position - offset - 4;
        *(DWORD*)(nativeState + 0x64) = offset;
    }
    if (*(DWORD*)(nativeState + 0x94))
    {
        DWORD offset = output.position - 4;
        output.Write(*(void**)(nativeState + 0x90), *(DWORD*)(nativeState + 0x94) * 6);
        *(DWORD*)(nativeState + 4) += output.position - offset - 4;
        *(DWORD*)(nativeState + 0x60) = offset;
    }
    output.position = 0;
    output.ended = 0;
    output.Write(nativeState, 0x78);
    output.SaveFile(filename);
    return 1;
}

void GameParseStreamData(const void* data, DWORD length, void* destination)
{
    CModelFileStream* reader = new CModelFileStream((BYTE*)data, length);
    DWORD count = 0;
    reader->Read(&count, 4);
    if (count == 0 || count > 128)
    {
        delete reader;
        return;
    }
    DWORD selected = 0;
    for (DWORD i = 0; i < count; i++)
    {
        if (reader->ended)
            break;
        BYTE position[12];
        DWORD type = 0, size = 0;
        reader->Read(position, 12);
        reader->Read(&type, 4);
        reader->Read(&size, 4);
        if ((type == 0 && (size == 76 || size == 80)) || (type == 10 && size == 40))
            ++selected;
        reader->Skip(size);
    }
    if (!selected)
        return; // Native leaks the reader on this path; preserve it.
    BYTE* result = (BYTE*)calloc(1, selected * 64 + 4);
    *(DWORD*)result = selected;
    reader->position = 0;
    reader->ended = false;
    DWORD ignoredCount = 0;
    reader->Read(&ignoredCount, 4);
    DWORD written = 0;
    BYTE* record = result + 4;
    do
    {
        if (reader->ended)
            break;
        BYTE position[12];
        DWORD type = 0, size = 0;
        reader->Read(position, 12);
        reader->Read(&type, 4);
        reader->Read(&size, 4);
        if (type == 0 && (size == 76 || size == 80))
        {
            BYTE effect[56] = {0};
            memcpy(effect, position, 12);
            reader->Read(effect + 16, 4); // Color.
            reader->Read(effect + 20, 4);
            reader->Read(effect + 24, 4);
            reader->Read(effect + 28, 4);
            reader->Read(effect + 32, 4);
            reader->Read(effect + 38, 1);
            reader->Read(effect + 39, 1);
            reader->Read(effect + 40, 1);
            reader->Read(effect + 41, 1);
            BYTE low = 0, high = 0, discard = 0;
            reader->Read(&low, 1);
            char first[65] = {0}, second[65] = {0};
            reader->Read(first, 24);
            reader->Read(second, 24);
            reader->Read(effect + 42, 1);
            reader->Read(&high, 1);
            *(WORD*)(effect + 36) = low | ((WORD)high << 8);
            if (size == 80)
            {
                reader->Read(effect + 43, 1);
                reader->Read(effect + 44, 1);
                reader->Read(effect + 45, 1);
                reader->Read(&discard, 1);
            }
            else
            {
                effect[43] = 0;
                effect[44] = 0;
                effect[45] = 100;
            }
            reader->Read(&discard, 1);
            if (strlen(first))
                *(void**)(effect + 48) = GameLoadDictionaryTexture("particle", first);
            if (strlen(second))
                *(void**)(effect + 52) = GameLoadDictionaryTexture("particle", second);
            memcpy(record, effect, 56);
            record += 64;
            ++written;
        }
        else if (type == 10 && size == 40)
        {
            BYTE effect[53] = {0};
            memcpy(effect, position, 12);
            *(DWORD*)(effect + 12) = 10;
            reader->Read(effect + 16, 12);
            reader->Read(effect + 28, 12);
            reader->Read(effect + 40, 12);
            DWORD value = 0;
            reader->Read(&value, 4);
            effect[52] = (BYTE)value;
            memcpy(record, effect, 53);
            record += 64;
            ++written;
        }
        else
            reader->Skip(size);
    }
    while (written != selected);
    if (!written)
        free(result);
    else
    {
        *(DWORD*)result = written;
        *(void**)((BYTE*)destination + 0x78) = result;
    }
    delete reader;
}
