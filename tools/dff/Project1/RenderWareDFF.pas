unit RenderWareDFF;

// Record type names and managed-field layouts recovered from Delphi RTTI.
// Non-published member names below describe the observed native operations.
// Recovered against the native loader, color-preview and rendering instructions.

interface

uses Classes, SysUtils, VectorTypes, VectorGeometry, OpenGL12, rwtxd;

type
  TDFFVector = TAffineVector;
  TDFFMatrix = TMatrix;
  TDFFRotation = TAffineMatrix;
  TDFFUV = TVector2f;
  TDFFUVMAP = array of TDFFUV;
  TDFFTriangle = record
    VertexA, VertexB, Material, VertexC: Word;
  end;
  // 0x0006e05c (type information)
  TDFFFrame = record
    Name: string;                         // +0x00
    Matrix: TDFFMatrix;                    // +0x04
    Rotation: TDFFRotation;                // +0x44
    Position: TDFFVector;                  // +0x68
    ParentIndex: Integer;                 // +0x74
    Flags, Unknown7a: Word;                // +0x78
    Unknown7c: Cardinal;
  end;
  // 0x0006e0a4 (type information)
  TDFFDataFrameList = record
    Count: Cardinal;
    Frames: array of TDFFFrame;
  end;
  // 0x0006e408 (type information)
  TDFFFrameList = record
    Data: TDFFDataFrameList;
  end;
  // 0x0006e20c (type information)
  TDFFDataGeometry = record
    Flags: Word;                          // +0x00
    UVCount, Unknown03: Byte;
    TriangleCount, VertexCount, MorphTargetCount: Cardinal;
    Surface: array[0..2] of Single;         // +0x10
    Colors: array of Cardinal;             // +0x1c
    NightColors: array of Cardinal;        // +0x20
    EditColors: array of Cardinal;         // +0x24
    EditNightColors: array of Cardinal;    // +0x28
    UVMaps: array of TDFFUVMAP;             // +0x2c
    Triangles: array of TDFFTriangle;      // +0x30
    SphereCenter: TDFFVector;              // +0x34
    SphereRadius: Single;
    HasVertices, HasNormals: Cardinal;
    Vertices: array of TDFFVector;         // +0x4c
    Normals: array of TDFFVector;          // +0x50
  end;
  // 0x0006e26c (type information)
  TDFFTexture = record
    Name, Mask: string;
    HasName: Boolean;
    ExtraData: array[0..258] of Byte;      // +0x09
    PluginSize: Cardinal;                 // +0x10c
    PluginData: array[0..375] of Byte;     // +0x110
  end;
  TDFFRGBA = packed record
    case Integer of
      0: (Value: Cardinal);
      1: (R, G, B, A: Byte);
  end;
  // 0x0006e298 (type information)
  TDFFMaterial = record
    Flags: Cardinal;
    Color: TDFFRGBA;
    Unused, Textured: Cardinal;
    Surface: array[0..2] of Single;
    Unknown1c: Byte;
    SourcePosition: Integer;              // +0x20
    Texture: TDFFTexture;                 // +0x24
  end;
  // 0x0006e2e4 (type information)
  TDFFSplit = record
    IndexCount, MaterialIndex: Cardinal;
    Indices: array of Cardinal;
  end;
  // 0x0006e32c (type information)
  TDFFMaterialSplit = record
    Flags, SplitCount, IndexCount: Cardinal;
    Splits: array of TDFFSplit;
  end;
  // 0x0006e37c (type information)
  TDFFMaterialList = record
    Count, FirstMaterialIndex: Cardinal;
    Materials: array of TDFFMaterial;
    CurrentMaterial, Unknown0e: Word;
  end;
  TDFFMatrixExtension = packed record
    Data: array[0..47] of Byte;
    Unknown30: Byte;
  end;
  // 0x0006e3cc (type information)
  TDFFGeometry = record
    Data: TDFFDataGeometry;
    MaterialList: TDFFMaterialList;
    MaterialSplit: TDFFMaterialSplit;
    MatrixExtensions: array of TDFFMatrixExtension;
  end;
  // 0x0006e454 (type information)
  TDFFGeometryList = record
    Count: Cardinal;
    Geometries: array of TDFFGeometry;
    CurrentGeometry: Integer;
  end;
  TDFFAtomic = record
    FrameIndex, GeometryIndex, Flags, Unknown0c: Cardinal;
  end;
  // 0x0006e4a4 (type information)
  TDFFClump = record
    AtomicCount: Cardinal;
    FrameList: TDFFFrameList;
    GeometryList: TDFFGeometryList;
    Atomics: array of TDFFAtomic;
    CurrentAtomic, Unknown1e: Word;
    AnimationStream: TMemoryStream;
    Radius: Single;
    HasCol: Boolean;
  end;
  TDFFChunkHeader = record
    Position, DataPosition: Cardinal;
    ChunkType, DataSize, Version: Cardinal;
  end;
  // 0x0006e544 (VMT)
  TDffLoader = class
  public
    Clumps: array of TDFFClump;            // +0x04
  private
    FFrameIndex: Integer;                 // +0x08
    FUnknown0c: Cardinal;
  public
    Loaded: Boolean;                      // +0x10
    FileName: string;                     // +0x14
    DisplayList: Cardinal;                // +0x18
    Flag1c, Drawing: Boolean;
    Unknown1e: Word;
    VehicleColor1, VehicleColor2: Cardinal; // +0x20, +0x24
    // 0x0006e6dc
    procedure Clear;
    // 0x0006e764
    procedure LoadFromStream(Stream: TStream);
    // 0x0006e7bc
    procedure LoadFromFile(AFileName: string);
    // 0x000701b4
    procedure Draw(DayTxd, NightTxd: Ttxdloader; Outline: Boolean;
      Mode, ColorMode: Integer; NoCache: Boolean);
    // 0x0006ff00
    procedure SetEditColors(Color: Cardinal; UnusedOption: Integer;
      X, Y, Z, Minimum, Maximum: Single);
    // 0x00070010
    procedure SetEditNightColors(Color: Cardinal; UnusedOption: Integer;
      X, Y, Z, Minimum, Maximum: Single);
  private
    // 0x00070284
    procedure RenderFrame(ClumpIndex, FrameIndex: Cardinal;
      DayTxd, NightTxd: Ttxdloader; Root, Outline: Boolean;
      Mode, ColorMode: Integer; NoCache: Boolean);
    // 0x00070c28
    procedure RenderWheel(Name: string; DayTxd, NightTxd: Ttxdloader;
      NoCache: Boolean);
    // 0x0006faf8
    procedure CalculateNormals;
    // 0x0006e834
    procedure ReadMaterialSplit(Stream: TStream);
    // 0x0006e92c
    procedure ReadStruct(Stream: TStream; Header: TDFFChunkHeader; ParentType: Integer);
    // 0x0006f124
    procedure ReadString(Stream: TStream; Header: TDFFChunkHeader;
      ParentType, Depth: Integer);
    // 0x0006f2f0
    procedure ReadChunks(Stream: TStream; Parent: TDFFChunkHeader;
      ParentType, Depth: Integer);
    // 0x0006fa94
    function ReadHeader(Stream: TStream; Depth, ParentType: Integer): TDFFChunkHeader;
  end;

function DecodeDffVersion(Value: Cardinal): Cardinal;

implementation

uses Forms;


// 0x0006e6a0
function DecodeDffVersion(Value: Cardinal): Cardinal;
var
  HighByte: Cardinal;
begin
  HighByte := Value shr 24;
  Result := ((HighByte shr 6) + 3) shl 16;
  Result := Result + ((HighByte shr 2) and $0f) shl 12;
  Result := Result + (HighByte and 3) shl 8;
  Result := Result + ((Value shr 16) and $ff);
end;

// 0x0006e6dc
procedure TDffLoader.Clear;
var
  I: Integer;
begin
  for I := 0 to High(Clumps) do
  begin
    SetLength(Clumps[I].Atomics, 0);
    SetLength(Clumps[I].GeometryList.Geometries, 0);
    SetLength(Clumps[I].FrameList.Data.Frames, 0);
  end;
  SetLength(Clumps, 0);
end;

// 0x0006e764
procedure TDffLoader.LoadFromStream(Stream: TStream);
var
  Header: TDFFChunkHeader;
begin
  Clear;
  Header.Position := $10;
  Header.DataPosition := 0;
  Header.ChunkType := 0;
  Header.DataSize := Cardinal(Stream.Size);
  Header.Version := 0;
  Loaded := False;
  Flag1c := False;
  ReadChunks(Stream, Header, $10, 0);
  Loaded := True;
end;

// 0x0006e7bc
procedure TDffLoader.LoadFromFile(AFileName: string);
var
  Stream: TMemoryStream;
begin
  Stream := TMemoryStream.Create;
  FileName := AFileName;
  Stream.LoadFromFile(AFileName);
  LoadFromStream(Stream);
  Stream.Free;
end;

// 0x0006e834
procedure TDffLoader.ReadMaterialSplit(Stream: TStream);
var
  C, G: Integer;
  I: Integer;
begin
  C := High(Clumps);
  G := Clumps[C].GeometryList.CurrentGeometry - 1;
  with Clumps[C].GeometryList.Geometries[G].MaterialSplit do
  begin
    Stream.Read(Flags, 12);
    SetLength(Splits, SplitCount);
    for I := 0 to Integer(SplitCount) - 1 do
    begin
      Stream.Read(Splits[I].IndexCount, 4);
      Stream.Read(Splits[I].MaterialIndex, 4);
      SetLength(Splits[I].Indices, Splits[I].IndexCount);
      Stream.Read(Splits[I].Indices[0], Splits[I].IndexCount * 4);
    end;
  end;
end;

// 0x0006e92c
procedure TDffLoader.ReadStruct(Stream: TStream; Header: TDFFChunkHeader;
  ParentType: Integer);
var
  C, G, M, J: Integer;
  I: Cardinal;
  StartPosition: Integer;
begin
  C := High(Clumps);
  case ParentType of
    $10: Stream.Read(Clumps[C].AtomicCount, 4);
    $1a: Stream.Read(Clumps[C].GeometryList.Count, 4);
    $08:
      begin
        G := Clumps[C].GeometryList.CurrentGeometry - 1;
        with Clumps[C].GeometryList.Geometries[G].MaterialList do
        begin
          Stream.Read(Count, 4);
          Stream.Read(FirstMaterialIndex, 4);
        end;
      end;
    $07:
      begin
        G := Clumps[C].GeometryList.CurrentGeometry - 1;
        M := Clumps[C].GeometryList.Geometries[G].MaterialList.CurrentMaterial - 1;
        with Clumps[C].GeometryList.Geometries[G].MaterialList.Materials[M] do
        begin
          Texture.HasName := False;
          SourcePosition := Integer(Stream.Position);
          Stream.Read(Flags, $1d);
        end;
      end;
    $14:
      with Clumps[C] do
      begin
        I := CurrentAtomic - 1;
        Stream.Read(Atomics[I].FrameIndex, 4);
        Stream.Read(Atomics[I].GeometryIndex, 4);
        Stream.Read(Atomics[I].Flags, 4);
        Stream.Read(Atomics[I].Unknown0c, 4);
      end;
    $0e:
      with Clumps[C].FrameList.Data do
      begin
        Stream.Read(Count, 4);
        SetLength(Frames, Count);
        FFrameIndex := 0;
        for I := 0 to Count - 1 do
        begin
          for J := 0 to 2 do
            Stream.Read(Frames[I].Rotation[J], 12);
          Stream.Read(Frames[I].Position, 12);
          FillChar(Frames[I].Matrix, 64, 0);
          Frames[I].Matrix[0, 0] := 1;
          Frames[I].Matrix[1, 1] := 1;
          Frames[I].Matrix[2, 2] := 1;
          for J := 0 to 2 do
          begin
            Move(Frames[I].Rotation[J], Frames[I].Matrix[J], 12);
          end;
          Frames[I].Matrix[3, 0] := Frames[I].Position[0];
          Frames[I].Matrix[3, 1] := Frames[I].Position[1];
          Frames[I].Matrix[3, 2] := Frames[I].Position[2];
          Frames[I].Matrix[3, 3] := 1;
          Stream.Read(Frames[I].ParentIndex, 4);
          Stream.Read(Frames[I].Flags, 2);
          Stream.Read(Frames[I].Unknown7a, 2);
        end;
      end;
    $0f:
      begin
        G := Clumps[C].GeometryList.CurrentGeometry - 1;
        with Clumps[C].GeometryList.Geometries[G].Data do
        begin
          StartPosition := Integer(Stream.Position);
          Stream.Read(Flags, 16);
          if DecodeDffVersion(Header.Version) < $34000 then
            Stream.Read(Surface, 12)
          else
            FillChar(Surface, 12, 0);
          if (Flags and $08) = $08 then
          begin
            SetLength(Colors, VertexCount);
            Stream.Read(Colors[0], VertexCount * 4);
          end
          else
            SetLength(Colors, 0);
          if UVCount <> 0 then
          begin
            SetLength(UVMaps, UVCount);
            for J := 0 to UVCount - 1 do
            begin
              SetLength(UVMaps[J], VertexCount);
              Stream.Read(UVMaps[J][0], VertexCount * 8);
            end;
          end
          else
            SetLength(UVMaps, 0);
          SetLength(Triangles, TriangleCount);
          Stream.Read(Triangles[0], TriangleCount * 8);
          Stream.Read(SphereCenter, 24);
          SetLength(Vertices, VertexCount);
          Stream.Read(Vertices[0], VertexCount * 12);
          Clumps[C].Radius := 0;
          for J := 0 to High(Vertices) do
          begin
            if Clumps[C].Radius < Vertices[J][0] then
              Clumps[C].Radius := Vertices[J][0];
            if Clumps[C].Radius < Vertices[J][1] then
              Clumps[C].Radius := Vertices[J][1];
            if Clumps[C].Radius < Vertices[J][2] then
              Clumps[C].Radius := Vertices[J][2];
          end;
          if (Flags and $10) = $10 then
          begin
            SetLength(Normals, VertexCount);
            Stream.Read(Normals[0], VertexCount * 12);
          end
          else
            SetLength(Normals, 0);
          Stream.Position := Integer(Cardinal(Cardinal(StartPosition) + Header.DataSize));
        end;
      end;
  end;
end;

// 0x0006f124
procedure TDffLoader.ReadString(Stream: TStream; Header: TDFFChunkHeader;
  ParentType, Depth: Integer);
var
  Buffer: PChar;
  Indent: string;
  C, G, M, I: Integer;
begin
  Indent := '';
  for I := 0 to Depth do
    Indent := Indent + '      ';
  GetMem(Buffer, Header.DataSize + 1);
  Buffer[Header.DataSize] := #0;
  Stream.Read(Buffer^, Header.DataSize);
  C := High(Clumps);
  if ParentType = $06 then
  begin
    G := Clumps[C].GeometryList.CurrentGeometry - 1;
    M := Clumps[C].GeometryList.Geometries[G].MaterialList.CurrentMaterial - 1;
    with Clumps[C].GeometryList.Geometries[G].MaterialList.Materials[M].Texture do
    begin
      if not HasName then
        Name := Trim(string(Buffer))
      else
        Mask := Trim(string(Buffer));
      HasName := True;
    end;
  end
  else if ParentType = $0e then
  begin
    Clumps[C].FrameList.Data.Frames[FFrameIndex].Name := Trim(string(Buffer));
    Inc(FFrameIndex);
  end;
  FreeMem(Buffer);
end;

// 0x0006f2f0
procedure TDffLoader.ReadChunks(Stream: TStream; Parent: TDFFChunkHeader;
  ParentType, Depth: Integer);
var
  Header: TDFFChunkHeader;
  C, G, M, I, SavedPosition: Integer;
  ContinueReading: Boolean;
begin
  ContinueReading := True;
  C := High(Clumps);
  while ContinueReading do
  begin
    Header := ReadHeader(Stream, Depth, ParentType);
    if Header.ChunkType = $10 then
    begin
      SetLength(Clumps, Length(Clumps) + 1);
      Depth := 0;
    end;
    case Header.ChunkType of
      $0f:
        begin
          Inc(Clumps[C].GeometryList.CurrentGeometry);
          SetLength(Clumps[C].GeometryList.Geometries,
            Clumps[C].GeometryList.CurrentGeometry);
          G := High(Clumps[C].GeometryList.Geometries);
          FillChar(Clumps[C].GeometryList.Geometries[G], SizeOf(TDFFGeometry), 0);
        end;
      $07:
        begin
          G := Clumps[C].GeometryList.CurrentGeometry - 1;
          Inc(Clumps[C].GeometryList.Geometries[G].MaterialList.CurrentMaterial);
          with Clumps[C].GeometryList.Geometries[G].MaterialList do
            SetLength(Materials, CurrentMaterial);
        end;
      $14:
        begin
          Inc(Clumps[C].CurrentAtomic);
          SetLength(Clumps[C].Atomics, Clumps[C].CurrentAtomic);
          I := High(Clumps[C].Atomics);
          FillChar(Clumps[C].Atomics[I], SizeOf(TDFFAtomic), 0);
        end;
      $120, $253f2f6:
        begin
          SavedPosition := Integer(Stream.Position);
          Stream.Seek(4, soFromCurrent);
          G := Clumps[C].GeometryList.CurrentGeometry - 1;
          M := Clumps[C].GeometryList.Geometries[G].MaterialList.CurrentMaterial - 1;
          with Clumps[C].GeometryList.Geometries[G].MaterialList.Materials[M].Texture do
            if Header.ChunkType = $120 then
            begin
              FillChar(PluginData, Header.DataSize, 0);
              PluginSize := Header.DataSize;
              Stream.Read(PluginData, Header.DataSize);
            end
            else
              Stream.Read(ExtraData, Header.DataSize - 4);
          Stream.Position := SavedPosition;
        end;
      $253f2fa:
        begin
          SavedPosition := Integer(Stream.Position);
          Clumps[C].AnimationStream := TMemoryStream.Create;
          Clumps[C].AnimationStream.Clear;
          Clumps[C].AnimationStream.CopyFrom(Stream, Header.DataSize);
          Stream.Position := SavedPosition;
        end;
      $253f2ff: Clumps[C].HasCol := True;
    end;
    case Header.ChunkType of
      $01:
        begin
          // The original catches all exceptions around this particular reader.
          try
            ReadStruct(Stream, Header, ParentType);
          except
          end;
        end;
      $02, $253f2fe: ReadString(Stream, Header, ParentType, Depth + 1);
      $03:
        if Header.DataSize > 0 then
          ReadChunks(Stream, Header, ParentType, Depth + 1);
      $06, $07, $08, $0e, $0f, $10, $14, $1a:
        ReadChunks(Stream, Header, Header.ChunkType, Depth + 1);
      $50e: ReadMaterialSplit(Stream);
      $253f2f8:
        begin
          SavedPosition := Integer(Stream.Position);
          G := Clumps[C].GeometryList.CurrentGeometry - 1;
          with Clumps[C].GeometryList.Geometries[G] do
          begin
            SetLength(MatrixExtensions, Header.DataSize div 48);
            if Length(MatrixExtensions) > 0 then
              for I := 0 to High(MatrixExtensions) do
                Stream.Read(MatrixExtensions[I].Data, 48);
          end;
          Stream.Position := SavedPosition;
        end;
      $253f2f9:
        if Length(Clumps[C].GeometryList.Geometries) > 0 then
        begin
          G := Clumps[C].GeometryList.CurrentGeometry - 1;
          with Clumps[C].GeometryList.Geometries[G].Data do
          begin
            SetLength(NightColors, VertexCount);
            Stream.Seek(4, soFromCurrent);
            Stream.Read(NightColors[0], VertexCount * 4);
          end;
        end;
    else
      Stream.Seek(4, soFromCurrent);
      Stream.Seek(Integer(Header.DataSize), soFromCurrent);
    end;
    Stream.Seek(Integer(Cardinal(Header.DataPosition + Header.DataSize)), soFromBeginning);
    if (Stream.Position >= Int64(Cardinal(Parent.DataPosition + Parent.DataSize))) or
       (Header.ChunkType = 0) then
      ContinueReading := False;
  end;
end;

// 0x0006fa94
function TDffLoader.ReadHeader(Stream: TStream; Depth,
  ParentType: Integer): TDFFChunkHeader;
begin
  Result.Position := Cardinal(Stream.Position);
  Stream.Read(Result.ChunkType, 4);
  Stream.Read(Result.DataSize, 4);
  Stream.Read(Result.Version, 4);
  Result.DataPosition := Cardinal(Stream.Position);
end;

// 0x0006faf8
procedure TDffLoader.CalculateNormals;
var
  Geometry: TDFFGeometry;
  I: Integer;
  A, B, C, Edge1, Edge2, Normal, Zero: TAffineVector;
begin
  Geometry := Clumps[0].GeometryList.Geometries[0];
  Zero := AffineVectorMake(0, 0, 0);
  with Geometry.Data do
  begin
    for I := 0 to Integer(VertexCount) - 1 do
      Normals[I] := Zero;
    for I := 0 to Integer(TriangleCount) - 1 do
    begin
      A := Vertices[Triangles[I].VertexA];
      B := Vertices[Triangles[I].VertexB];
      C := Vertices[Triangles[I].VertexC];
      Edge1 := VectorSubtract(A, B);
      Edge2 := VectorSubtract(C, B);
      Normal := VectorCrossProduct(Edge1, Edge2);
      Normals[Triangles[I].VertexA] := VectorAdd(Normals[Triangles[I].VertexA], Normal);
      Normals[Triangles[I].VertexB] := VectorAdd(Normals[Triangles[I].VertexB], Normal);
      Normals[Triangles[I].VertexC] := VectorAdd(Normals[Triangles[I].VertexC], Normal);
    end;
    for I := 0 to Integer(VertexCount) - 1 do
      Normals[I] := VectorNormalize(Normals[I]);
  end;
end;

// 0x0006e56c; descriptive name, original spelling is not available.
function MaxSingle(A, B: Single): Single;
begin
  Result := A;
  if B > A then Result := B;
end;

// Descriptive name; the native helper is not published in RTTI.
// 0x0006fdac
function LightVertexColor(Color: Cardinal; Normal, Direction: TAffineVector;
  Minimum, Maximum: Single): Cardinal;
const
  Zero: Single = 0;
var
  R, G, B, Brightness, Component: Single;
  Red, Green, Blue: Byte;
begin
  Red := Byte(Color);
  Green := Byte(Color shr 8);
  Blue := Byte(Color shr 16);
  R := Red;
  G := Green;
  B := Blue;
  if (Direction[0] = 0) and (Direction[1] = 0) and (Direction[2] = 0) then
  begin
    Result := $ff000000 or (Cardinal(Blue) shl 16) or
      (Cardinal(Green) shl 8) or Red;
    Exit;
  end;
  Brightness := MaxSingle(VectorDotProduct(Normal, Direction), Zero);
  if Brightness < Minimum then Brightness := Minimum;
  if Brightness > Maximum then Brightness := Maximum;
  Component := R * Brightness;
  Red := Byte(VectorGeometry.Round(Component));
  Component := G * Brightness;
  Green := Byte(VectorGeometry.Round(Component));
  Component := B * Brightness;
  Blue := Byte(VectorGeometry.Round(Component));
  Result := $ff000000 or (Cardinal(Blue) shl 16) or
    (Cardinal(Green) shl 8) or Red;
end;

// 0x0006ff00
procedure TDffLoader.SetEditColors(Color: Cardinal; UnusedOption: Integer;
  X, Y, Z, Minimum, Maximum: Single);
var
  UnusedColorCount: Integer;
  I, Count: Cardinal;
  Direction: TAffineVector;
begin
  Count := Clumps[0].GeometryList.Geometries[0].Data.VertexCount;
  SetLength(Clumps[0].GeometryList.Geometries[0].Data.EditColors, Count);
  if Int64(Length(Clumps[0].GeometryList.Geometries[0].Data.Normals)) <> Int64(Count) then
  begin
    SetLength(Clumps[0].GeometryList.Geometries[0].Data.Normals, Count);
    CalculateNormals;
  end;
  // Native code evaluates this length without using the comparison result.
  UnusedColorCount := Length(Clumps[0].GeometryList.Geometries[0].Data.Colors);
  Direction := AffineVectorMake(X, Y, Z);
  for I := 0 to Count - 1 do
    with Clumps[0].GeometryList.Geometries[0].Data do
    begin
      EditColors[I] := Color;
      EditColors[I] := LightVertexColor(Color, Normals[I], Direction, Minimum, Maximum);
    end;
end;

// 0x00070010
procedure TDffLoader.SetEditNightColors(Color: Cardinal; UnusedOption: Integer;
  X, Y, Z, Minimum, Maximum: Single);
var
  I, Count: Cardinal;
  Direction: TAffineVector;
begin
  Count := Clumps[0].GeometryList.Geometries[0].Data.VertexCount;
  SetLength(Clumps[0].GeometryList.Geometries[0].Data.EditNightColors, Count);
  if Int64(Length(Clumps[0].GeometryList.Geometries[0].Data.Normals)) <> Int64(Count) then
  begin
    SetLength(Clumps[0].GeometryList.Geometries[0].Data.Normals, Count);
    CalculateNormals;
  end;
  Direction := AffineVectorMake(X, Y, Z);
  for I := 0 to Count - 1 do
    with Clumps[0].GeometryList.Geometries[0].Data do
    begin
      EditNightColors[I] := Color;
      EditNightColors[I] := LightVertexColor(Color, Normals[I], Direction, Minimum, Maximum);
    end;
end;

// Descriptive color-helper names; native routines return integer 0 or 1.
// 0x0006e590
function IsVehicleColor1(Color: TDFFRGBA): Integer;
begin
  Result := 0;
  if (Color.R = $3c) and (Color.G = $ff) and (Color.B = 0) and
     (Color.A = $ff) then Result := 1;
end;

// 0x0006e5c0
function IsSpecialColor1(Color: TDFFRGBA): Integer;
begin
  Result := 0;
  if (Color.R = 0) and (Color.G = $ff) and (Color.B = $ff) then Result := 1;
end;

// 0x0006e5ec
function IsSpecialColor2(Color: TDFFRGBA): Integer;
begin
  Result := 0;
  if (Color.R = $ff) and (Color.G = $af) and (Color.B = 0) then Result := 1;
  if (Color.R = 0) and (Color.G = $ff) and (Color.B = $c8) then Result := 1;
  if (Color.R = $b9) and (Color.G = $ff) and (Color.B = $c8) then Result := 1;
  if (Color.R = $ff) and (Color.G = 0) and (Color.B = $af) then Result := 1;
  if (Color.R = $ff) and (Color.G = 0) and (Color.B = $af) and
     (Color.A = $ff) then Result := 1;
end;

// 0x0006e670
function IsVehicleColor2(Color: TDFFRGBA): Integer;
begin
  Result := 0;
  if (Color.R = $ff) and (Color.G = 0) and (Color.B = $af) and
     (Color.A = $ff) then Result := 1;
end;

// 0x000701b4
procedure TDffLoader.Draw(DayTxd, NightTxd: Ttxdloader; Outline: Boolean;
  Mode, ColorMode: Integer; NoCache: Boolean);
var
  I: Cardinal;
  // 0x00070100
  procedure CompileList(AMode: Integer);
  var
    Frame: Cardinal;
  begin
    DisplayList := glGenLists(1);
    glNewList(DisplayList, GL_COMPILE);
    if (Pointer(Clumps) <> nil) and (Clumps[0].FrameList.Data.Count > 0) then
      for Frame := 0 to Clumps[0].FrameList.Data.Count - 1 do
        if Clumps[0].FrameList.Data.Frames[Frame].ParentIndex = -1 then
          RenderFrame(0, Frame, DayTxd, NightTxd, True, False,
            AMode, ColorMode, NoCache);
    glEndList;
  end;
begin
  Drawing := True;
  if (DisplayList = 0) and not Outline and (Mode <> 1) then
    CompileList(Mode);
  if ((Mode = 0) or (Mode = 2)) and not Outline and not NoCache then
  begin
    if DisplayList <> 0 then glCallList(DisplayList);
  end
  else
  begin
    if (Pointer(Clumps) <> nil) and (Clumps[0].FrameList.Data.Count > 0) then
      for I := 0 to Clumps[0].FrameList.Data.Count - 1 do
        if Clumps[0].FrameList.Data.Frames[I].ParentIndex = -1 then
          RenderFrame(0, I, DayTxd, NightTxd, True, Outline,
            Mode, ColorMode, NoCache);
  end;
  Drawing := False;
end;

// 0x00070284
procedure TDffLoader.RenderFrame(ClumpIndex, FrameIndex: Cardinal;
  DayTxd, NightTxd: Ttxdloader; Root, Outline: Boolean;
  Mode, ColorMode: Integer; NoCache: Boolean);
type
  PDFFGeometry = ^TDFFGeometry;
  PDFFMaterial = ^TDFFMaterial;
var
  GeometryIndex, I, SplitIndex, ChildLength: Integer;
  ChildIndex: Cardinal;
  TextureID: Cardinal;
  FrameName, ChildName: string;
  Geometry: PDFFGeometry;
  Material: PDFFMaterial;
begin
  FrameName := Clumps[ClumpIndex].FrameList.Data.Frames[FrameIndex].Name;
  if LowerCase(FrameName) <> '' then
  begin
    if Pos('_dam', LowerCase(FrameName)) > 0 then Exit;
    if Pos('_vlo', LowerCase(FrameName)) > 0 then Exit;
  end;
  glPushMatrix;
  glColor4f(1, 1, 1, 1);
  GeometryIndex := -1;
  if Clumps[ClumpIndex].CurrentAtomic > 0 then
    for I := 0 to Clumps[ClumpIndex].CurrentAtomic - 1 do
      if Clumps[ClumpIndex].Atomics[I].FrameIndex = FrameIndex then
        GeometryIndex := Clumps[ClumpIndex].Atomics[I].GeometryIndex;
  if not Flag1c then
  begin
    glMultMatrixf(@Clumps[ClumpIndex].FrameList.Data.Frames[FrameIndex].Matrix);
    if (FrameName = 'wheel') or (FrameName = 'wheel2') then
      if Clumps[ClumpIndex].FrameList.Data.Count > 5 then
        Exit; // Native early exit occurs after glPushMatrix, without a pop.
    if (Pos('wheel_', FrameName) > 0) and
       (Clumps[ClumpIndex].FrameList.Data.Count > 5) and
       (FrameName <> 'wheel_front') and (FrameName <> 'wheel_rear') then
    begin
      RenderWheel(FrameName, DayTxd, NightTxd, NoCache);
      glPopMatrix;
      Exit;
    end;
  end;
  // The native bound here is the frame count, not the geometry count.
  if (GeometryIndex < Integer(Clumps[ClumpIndex].FrameList.Data.Count)) and
     (GeometryIndex <> -1) then
  begin
    Geometry := @Clumps[ClumpIndex].GeometryList.Geometries[GeometryIndex];
    for SplitIndex := 0 to Integer(Geometry^.MaterialSplit.SplitCount) - 1 do
    begin
      if Outline <> True then
      begin
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_ALPHA_TEST);
        Material := @Geometry^.MaterialList.Materials[
          Geometry^.MaterialSplit.Splits[SplitIndex].MaterialIndex];
        if Material^.Texture.Name <> '' then
        begin
          glEnable(GL_TEXTURE_2D);
          if Material^.Texture.Mask <> '' then
          begin
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0);
          end;
        end;
        if DayTxd <> nil then
        begin
          glEnable(GL_TEXTURE_2D);
          TextureID := DayTxd.FindTexture(Material^.Texture.Name);
          if TextureID = 0 then
            TextureID := NightTxd.FindTexture(Material^.Texture.Name);
          glBindTexture(GL_TEXTURE_2D, TextureID);
        end;
        if (IsVehicleColor1(Material^.Color) = 0) and
           (IsSpecialColor1(Material^.Color) = 0) and
           (IsSpecialColor2(Material^.Color) = 0) and
           (IsVehicleColor2(Material^.Color) = 0) then
          glColor3ubv(@Material^.Color)
        else if IsVehicleColor1(Material^.Color) = 1 then
          glColor3ubv(@VehicleColor1)
        else if IsVehicleColor2(Material^.Color) = 1 then
          glColor3ubv(@VehicleColor2)
        else
          glColor4f(1, 1, 1, 1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, Pointer(Geometry^.Data.Vertices));
        if Length(Geometry^.Data.UVMaps) > 0 then
        begin
          glEnableClientState(GL_TEXTURE_COORD_ARRAY);
          glTexCoordPointer(2, GL_FLOAT, 0, Pointer(Geometry^.Data.UVMaps[0]));
        end
        else
          glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        if Mode = 0 then
        begin
          if ColorMode = 0 then
            if Length(Geometry^.Data.Colors) > 0 then
            begin
              glEnableClientState(GL_COLOR_ARRAY);
              glColorPointer(4, GL_UNSIGNED_BYTE, 0, Pointer(Geometry^.Data.Colors));
            end
            else glDisableClientState(GL_COLOR_ARRAY);
          if ColorMode = 1 then
            if Length(Geometry^.Data.NightColors) > 0 then
            begin
              glEnableClientState(GL_COLOR_ARRAY);
              glColorPointer(4, GL_UNSIGNED_BYTE, 0, Pointer(Geometry^.Data.NightColors));
            end
            else glDisableClientState(GL_COLOR_ARRAY);
          if ColorMode = 2 then
            if Length(Geometry^.Data.EditColors) > 0 then
            begin
              glEnableClientState(GL_COLOR_ARRAY);
              glColorPointer(4, GL_UNSIGNED_BYTE, 0, Pointer(Geometry^.Data.EditColors));
            end
            else glDisableClientState(GL_COLOR_ARRAY);
          if ColorMode = 3 then
            if Length(Geometry^.Data.EditNightColors) > 0 then
            begin
              glEnableClientState(GL_COLOR_ARRAY);
              glColorPointer(4, GL_UNSIGNED_BYTE, 0, Pointer(Geometry^.Data.EditNightColors));
            end
            else glDisableClientState(GL_COLOR_ARRAY);
        end;
        if Mode = 1 then
          glColor4f(1.0 - 0.1 * Frac(0.1 * Integer(Application.Handle)), 0, 0, 1);
        if Mode = 2 then glColor4f(1, 1, 1, 1);
      end;
      if Geometry^.MaterialSplit.Flags = 0 then
        glDrawElements(GL_TRIANGLES,
          Length(Geometry^.MaterialSplit.Splits[SplitIndex].Indices), GL_UNSIGNED_INT,
          Pointer(Geometry^.MaterialSplit.Splits[SplitIndex].Indices))
      else
        glDrawElements(GL_TRIANGLE_STRIP,
          Length(Geometry^.MaterialSplit.Splits[SplitIndex].Indices), GL_UNSIGNED_INT,
          Pointer(Geometry^.MaterialSplit.Splits[SplitIndex].Indices));
    end;
    if Length(Geometry^.MatrixExtensions) > 0 then
      for I := 0 to High(Geometry^.MatrixExtensions) do
        ; // Native code also enumerates these records without doing anything.
  end;
  if Clumps[ClumpIndex].FrameList.Data.Count > 0 then
    for ChildIndex := 0 to Clumps[ClumpIndex].FrameList.Data.Count - 1 do
      if Clumps[ClumpIndex].FrameList.Data.Frames[ChildIndex].ParentIndex = Integer(FrameIndex) then
      begin
        ChildName := Clumps[ClumpIndex].FrameList.Data.Frames[ChildIndex].Name;
        ChildLength := Length(ChildName);
        if ChildLength < 3 then
          RenderFrame(ClumpIndex, ChildIndex, DayTxd, NightTxd, False,
            Outline, Mode, ColorMode, NoCache)
        else if (ChildName[ChildLength - 2] = '_') and
                (ChildName[ChildLength - 1] in ['L', 'l']) then
        begin
          if StrToIntDef(ChildName[ChildLength], -1) < 1 then
            RenderFrame(ClumpIndex, ChildIndex, DayTxd, NightTxd, True,
              Outline, Mode, ColorMode, NoCache);
        end
        else if (ChildName[1] <> 'C') or (ChildName[2] <> 'o') or (ChildName[3] <> 'l') then
          RenderFrame(ClumpIndex, ChildIndex, DayTxd, NightTxd, False,
            Outline, Mode, ColorMode, NoCache);
      end;
  glPopMatrix;
end;

// 0x00070c28
procedure TDffLoader.RenderWheel(Name: string; DayTxd, NightTxd: Ttxdloader;
  NoCache: Boolean);
var
  I: Cardinal;
  J: Integer;
begin
  if Pos('dummy', Name) > 0 then
  begin
    if Pos('wheel_l', Name) > 0 then glRotatef(180, 0, 0, 1);
    if Pos('steer', Name) = 0 then
    begin
      Flag1c := True;
      for I := 0 to Clumps[0].FrameList.Data.Count - 1 do
        for J := 0 to Integer(Clumps[0].FrameList.Data.Count) - 1 do
          if (LowerCase(Clumps[0].FrameList.Data.Frames[I].Name) = 'wheel_rf_dummy') and
             (Integer(I) = Clumps[0].FrameList.Data.Frames[J].ParentIndex) then
          begin
            RenderFrame(0, I, DayTxd, NightTxd, True, False, 0, 0, NoCache);
            Break;
          end;
      Flag1c := False;
    end;
  end;
end;

end.
