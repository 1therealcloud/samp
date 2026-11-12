unit rwtxd;

interface

uses Classes, SysUtils, Dialogs, OpenGL12, u_txdrecords;

type
  // 0x0006d9ec (VMT); non-published names are descriptive.
  Ttxdloader = class
  private
    FUnknown04: Cardinal;
  public
    Loaded: Boolean;                       // +0x08
    Textures: array of TTextureEntry;      // +0x0c
    // 0x0006da0c
    function FindTexture(Name: string): Cardinal;
    // 0x0006dacc
    procedure LoadFromFile(FileName: string);
    // 0x0006db3c
    procedure LoadFromStream(Stream: TMemoryStream; FileName: string);
  end;

implementation

// 0x0006da0c
function Ttxdloader.FindTexture(Name: string): Cardinal;
var
  I: Integer;
  TextureName: string;
begin
  Result := 0;
  for I := 0 to High(Textures) do
  begin
    TextureName := Textures[I].Name;
    if LowerCase(TextureName) = LowerCase(Name) then
    begin
      Result := Textures[I].TextureID;
      Break;
    end;
  end;
end;

// 0x0006dacc
procedure Ttxdloader.LoadFromFile(FileName: string);
var
  Stream: TMemoryStream;
begin
  Stream := TMemoryStream.Create;
  Stream.LoadFromFile(FileName);
  LoadFromStream(Stream, FileName);
  Stream.Free;
end;

// 0x0006db3c
procedure Ttxdloader.LoadFromStream(Stream: TMemoryStream; FileName: string);
var
  Count: Byte;
  Header: TNativeTextureHeader;
  I, Level: Integer;
  ImageSize: Integer;
  FourCC, MaskName: string;
  Pixels: Pointer;
begin
  Loaded := False;
  Stream.Position := 24;
  Stream.Read(Count, 1);
  Stream.Position := 16;
  if Count > 200 then
    ShowMessage(Format('%s contains unusual number of textures: %d - please investigate.',
      [FileName, Count]));
  SetLength(Textures, Count);
  for I := 0 to Count - 1 do
  begin
    Stream.Read(Header, $7c);
    FillChar(Textures[I].Name, 32, 0);
    Move(Header.Name, Textures[I].Name, 32);
    glGenTextures(1, @Textures[I].TextureID);
    glBindTexture(GL_TEXTURE_2D, Textures[I].TextureID);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if Header.Depth <> 0 then
    begin
      FourCC := Header.D3DFormat;
      if Copy(FourCC, 0, 3) = 'DXT' then
      begin
        for Level := 0 to Header.MipMapCount - 1 do
        begin
          Stream.Read(ImageSize, 4);
          if Level = 0 then
          begin
            Pixels := Pointer(Cardinal(Stream.Memory) + Cardinal(Stream.Position));
            case Header.D3DFormat[3] of
              '1':
                begin
                  // Native @LStrFromArray stops at NUL, bounded by 32 bytes.
                  MaskName := Header.Mask;
                  if MaskName <> '' then
                    glCompressedTexImage2DARB(GL_TEXTURE_2D, Level,
                      GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, Header.Width, Header.Height,
                      0, ImageSize, Pixels)
                  else
                    glCompressedTexImage2DARB(GL_TEXTURE_2D, Level,
                      GL_COMPRESSED_RGB_S3TC_DXT1_EXT, Header.Width, Header.Height,
                      0, ImageSize, Pixels);
                end;
              '3': glCompressedTexImage2DARB(GL_TEXTURE_2D, Level,
                GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, Header.Width, Header.Height,
                0, ImageSize, Pixels);
            else
              FourCC := Header.D3DFormat;
              ShowMessage(FourCC);
            end;
          end;
          Stream.Seek(ImageSize, soFromCurrent);
        end;
      end
      else
      begin
        if Header.MipMapCount = 0 then Header.MipMapCount := 1;
        if (Header.Depth = 32) or (Header.Depth = 24) then
          for Level := 0 to Header.MipMapCount - 1 do
          begin
            Stream.Read(ImageSize, 4);
            if Level = 0 then
            begin
              Pixels := Pointer(Cardinal(Stream.Memory) + Cardinal(Stream.Position));
              if Header.Depth = 32 then
                glTexImage2D(GL_TEXTURE_2D, Level, 4, Header.Width, Header.Height,
                  0, GL_BGRA, GL_UNSIGNED_BYTE, Pixels)
              else
                glTexImage2D(GL_TEXTURE_2D, Level, 3, Header.Width, Header.Height,
                  0, GL_BGR, GL_UNSIGNED_BYTE, Pixels);
            end;
            Stream.Seek(ImageSize, soFromCurrent);
          end;
      end;
    end;
  end;
  Loaded := True;
end;

end.
