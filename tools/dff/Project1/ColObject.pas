unit ColObject;

interface

uses Windows, Dialogs, Classes, SysUtils, VectorGeometry;

type
  // Record and private field names are descriptive; sizes come from native RTTI.
  TColBounds = packed record
    Min, Max, Center: TAffineVector;
    Radius: Single;
  end;
  TColSphere = packed record
    Position: TAffineVector;
    Radius: Single;
    Surface: array[0..3] of Byte;
    Auxiliary: Word;
  end;
  TColBox = packed record
    Min, Max: TAffineVector;
    Surface: array[0..3] of Byte;
    Auxiliary: Word;
  end;
  TColVertex = packed record
    Position: TAffineVector;
    Auxiliary: Word;
  end;
  TColFace = packed record
    Indices: array[0..2] of Word;
    Surface: array[0..3] of Byte;
    Normal, Center: TAffineVector;
    Auxiliary: Word;
  end;
  TColFaceGroup = packed record
    Min, Max: TAffineVector;
    FirstFace, LastFace: Word;
  end;
  TColVertices = array of TColVertex;
  TColFaces = array of TColFace;

  // 0x00062d10 (VMT), instance size 0x84, inherited TComponent size 0x30.
  TColObject = class(TComponent)
  private
    FUnknown30: Integer;
    FChanged, FOptimizeOnSave, FCompressed: Boolean;
    FFormat: Byte;
    FModelName: string;
    FModelID: Integer;
    FBounds: TColBounds;
    FSpheres: array of TColSphere;
    FBoxes: array of TColBox;
    FVertices: array[0..1] of TColVertices;
    FFaces: array[0..1] of TColFaces;
    FFaceGroups: array of TColFaceGroup;
    // 0x00062ebc
    procedure Changed;
    // 0x00062ed0
    procedure SetFormat(Value: Byte);
    // 0x000646d4
    procedure CalculateFace(Mesh, Face: Integer; Reverse: Boolean);
    // 0x000647b4
    procedure ClearParts(Faces, Groups, Shadow, Boxes, Spheres: Boolean);
    // 0x000648e8
    procedure MergeVertices(Mesh: Integer; Distance: Single);
    // 0x00064bb0
    procedure RemoveDegenerateFaces(Mesh: Integer);
    // 0x00064cb0
    procedure RemoveUnusedVertices(Mesh: Integer);
    // 0x000652ac
    procedure BuildFaceGroups;
    // 0x000655e0
    procedure UpdateBounds(Replace: Boolean);
    // 0x00065744
    function CheckDimensions(Mesh: Integer): Boolean;
  public
    // 0x00062de0
    destructor Destroy; override;
    // 0x00062ee0
    procedure LoadFromFile(const FileName: string);
    // 0x000631fc
    procedure LoadFromStream(Stream: TStream);
    // 0x000656fc
    procedure Optimize(Full, ReplaceBounds: Boolean);
    // 0x00063ce0
    procedure Save(OwnerWindow: HWND; Stream: TStream; FullOptimization: Boolean);
    property Format: Byte read FFormat write SetFormat;
  end;

implementation

// 0x00062de0
destructor TColObject.Destroy;
begin
  FChanged := False;
  FModelName := '';
  SetLength(FSpheres, 0);
  SetLength(FBoxes, 0);
  SetLength(FVertices[0], 0);
  SetLength(FVertices[1], 0);
  SetLength(FFaces[0], 0);
  SetLength(FFaces[1], 0);
  SetLength(FFaceGroups, 0);
  inherited Destroy;
end;

// 0x00062ebc
procedure TColObject.Changed;
begin
  FChanged := True;
end;

// 0x00062ed0
procedure TColObject.SetFormat(Value: Byte);
begin
  if Value <> FFormat then
  begin
    FFormat := Value;
    Changed;
  end;
end;

// 0x00062ee0
procedure TColObject.LoadFromFile(const FileName: string);
var
  Stream: TMemoryStream;
begin
  Stream := TMemoryStream.Create;
  Stream.LoadFromFile(FileName);
  LoadFromStream(Stream);
  Stream.Free;
end;

// 0x000631fc
procedure TColObject.LoadFromStream(Stream: TStream);
var
  Magic, DataSize, BaseOffset: Cardinal;
  Name: array[0..19] of Char;
  SphereCount, BoxCount, GroupCount, Unused: Integer;
  FaceCount, VertexCount: array[0..1] of Integer;
  Flags, SphereOffset, BoxOffset, LineOffset, PlaneOffset: Cardinal;
  VertexOffset, FaceOffset: array[0..1] of Cardinal;
  Mesh, I, J: Integer;

  // 0x00062f54
  function ReadBounds: TColBounds;
  begin
    FillChar(Result, SizeOf(Result), 0);
    if FFormat = 0 then
    begin
      Stream.Read(Result.Radius, 4);
      Stream.Read(Result.Center, 12);
      Stream.Read(Result.Min, 12);
      Stream.Read(Result.Max, 12);
    end
    else
      Stream.Read(Result, 40);
  end;

  // 0x00062fd8
  function ReadSphere: TColSphere;
  begin
    FillChar(Result, SizeOf(Result), 0);
    if FFormat = 0 then
    begin
      Stream.Read(Result.Radius, 4);
      Stream.Read(Result.Position, 12);
      Stream.Read(Result.Surface, 4);
    end
    else
      Stream.Read(Result, 20);
  end;

  // 0x0006304c
  function ReadBox: TColBox;
  begin
    FillChar(Result, SizeOf(Result), 0);
    Stream.Read(Result, 28);
  end;

  // 0x00063078
  function ReadVertex: TColVertex;
  var
    PackedPosition: array[0..2] of SmallInt;
    K: Integer;
  begin
    FillChar(Result, SizeOf(Result), 0);
    if FFormat = 0 then
      Stream.Read(Result.Position, 12)
    else
    begin
      Stream.Read(PackedPosition, 6);
      for K := 0 to 2 do
        Result.Position[K] := PackedPosition[K] / 128;
    end;
  end;

  // 0x000630f4
  function ReadFace: TColFace;
  var
    K: Integer;
  begin
    FillChar(Result, SizeOf(Result), 0);
    if FFormat = 0 then
    begin
      for K := 0 to 2 do
      begin
        Stream.Read(Result.Indices[K], 2);
        Stream.Seek(2, soFromCurrent);
      end;
      Stream.Read(Result.Surface, 4);
    end
    else
    begin
      Stream.Read(Result.Indices, 6);
      Stream.Read(Result.Surface[0], 1);
      Stream.Read(Result.Surface[3], 1);
    end;
  end;

  // 0x000631e0
  function ReadGroup: TColFaceGroup;
  begin
    Stream.Read(Result, 28);
  end;

begin
  FChanged := False;
  if Stream.Read(Magic, 4) < 4 then Exit;
  if Magic = 0 then Exit;
  case Magic of
    $4C4C4F43: FFormat := 0;
    $324C4F43: FFormat := 1;
    $334C4F43: FFormat := 2;
  else
    Exit;
  end;
  FCompressed := FFormat <> 0;
  BaseOffset := Stream.Position;
  Stream.Read(DataSize, 4);
  Stream.Read(Name, 20);
  Stream.Read(FModelID, 4);
  FModelName := Trim(string(Name));
  FBounds := ReadBounds;
  if FFormat = 0 then
  begin
    Stream.Read(SphereCount, 4);
    SetLength(FSpheres, SphereCount);
    for I := 0 to SphereCount - 1 do FSpheres[I] := ReadSphere;
    Stream.Read(Unused, 4);
    Stream.Read(BoxCount, 4);
    SetLength(FBoxes, BoxCount);
    for I := 0 to BoxCount - 1 do FBoxes[I] := ReadBox;
    Stream.Read(VertexCount[0], 4);
    VertexCount[1] := 0;
    SetLength(FVertices[0], VertexCount[0]);
    for I := 0 to VertexCount[0] - 1 do FVertices[0][I] := ReadVertex;
    Stream.Read(FaceCount[0], 4);
    FaceCount[1] := 0;
    SetLength(FFaces[0], FaceCount[0]);
    for I := 0 to FaceCount[0] - 1 do FFaces[0][I] := ReadFace;
    for I := 0 to FaceCount[0] - 1 do CalculateFace(0, I, False);
    FChanged := True;
  end
  else
  begin
    SphereCount := 0;
    Stream.Read(SphereCount, 2);
    BoxCount := 0;
    Stream.Read(BoxCount, 2);
    Stream.Read(FaceCount[0], 4);
    Stream.Read(Flags, 4);
    Stream.Read(SphereOffset, 4);
    Stream.Read(BoxOffset, 4);
    Stream.Read(LineOffset, 4);
    Stream.Read(VertexOffset[0], 4);
    Stream.Read(FaceOffset[0], 4);
    Stream.Read(PlaneOffset, 4);
    if FFormat = 2 then
    begin
      Stream.Read(FaceCount[1], 4);
      Stream.Read(VertexOffset[1], 4);
      Stream.Read(FaceOffset[1], 4);
    end
    else
    begin
      FaceCount[1] := 0;
      VertexOffset[1] := 0;
      FaceOffset[1] := 0;
    end;
    SetLength(FSpheres, SphereCount);
    Stream.Position := BaseOffset + SphereOffset;
    for I := 0 to SphereCount - 1 do FSpheres[I] := ReadSphere;
    SetLength(FBoxes, BoxCount);
    Stream.Position := BaseOffset + BoxOffset;
    for I := 0 to BoxCount - 1 do FBoxes[I] := ReadBox;
    for Mesh := 0 to 1 do
    begin
      VertexCount[Mesh] := 0;
      SetLength(FFaces[Mesh], FaceCount[Mesh]);
      Stream.Position := BaseOffset + FaceOffset[Mesh];
      for I := 0 to FaceCount[Mesh] - 1 do
      begin
        FFaces[Mesh][I] := ReadFace;
        for J := 0 to 2 do
          if VertexCount[Mesh] <= FFaces[Mesh][I].Indices[J] then
            VertexCount[Mesh] := FFaces[Mesh][I].Indices[J] + 1;
      end;
      SetLength(FVertices[Mesh], VertexCount[Mesh]);
      Stream.Position := BaseOffset + VertexOffset[Mesh];
      for I := 0 to VertexCount[Mesh] - 1 do
        FVertices[Mesh][I] := ReadVertex;
      for I := 0 to FaceCount[Mesh] - 1 do CalculateFace(Mesh, I, False);
    end;
    if Flags and 8 = 0 then
      SetLength(FFaceGroups, 0)
    else
    begin
      Stream.Position := BaseOffset + FaceOffset[0] - 4;
      Stream.Read(GroupCount, 4);
      SetLength(FFaceGroups, GroupCount);
      Stream.Position := BaseOffset + FaceOffset[0] - 4 - GroupCount * 28;
      for I := 0 to GroupCount - 1 do FFaceGroups[I] := ReadGroup;
    end;
    FChanged := True;
  end;
  FOptimizeOnSave := False;
  Stream.Position := BaseOffset + 4 + DataSize;
end;

// 0x000646d4
procedure TColObject.CalculateFace(Mesh, Face: Integer; Reverse: Boolean);
var
  Index: Word;
  A, B, C, U, V: TAffineVector;
  K: Integer;
begin
  if Reverse then
  begin
    Index := FFaces[Mesh][Face].Indices[0];
    FFaces[Mesh][Face].Indices[0] := FFaces[Mesh][Face].Indices[1];
    FFaces[Mesh][Face].Indices[1] := Index;
  end;
  A := FVertices[Mesh][FFaces[Mesh][Face].Indices[0]].Position;
  B := FVertices[Mesh][FFaces[Mesh][Face].Indices[2]].Position;
  C := FVertices[Mesh][FFaces[Mesh][Face].Indices[1]].Position;
  // 0x000657c0: normal uses (B-A) cross (C-A).
  U := VectorSubtract(B, A);
  V := VectorSubtract(C, A);
  FFaces[Mesh][Face].Normal := VectorCrossProduct(U, V);
  NormalizeVector(FFaces[Mesh][Face].Normal);
  // 0x00065820
  for K := 0 to 2 do
    FFaces[Mesh][Face].Center[K] := (A[K] + C[K] + B[K]) / 3;
end;

// 0x000647b4
procedure TColObject.ClearParts(Faces, Groups, Shadow, Boxes, Spheres: Boolean);
begin
  FChanged := False;
  if Faces then
  begin
    SetLength(FFaces[0], 0);
    SetLength(FVertices[0], 0);
  end;
  if Faces or Groups then SetLength(FFaceGroups, 0);
  if Spheres then SetLength(FSpheres, 0);
  if Boxes then SetLength(FBoxes, 0);
  if Shadow then
  begin
    SetLength(FFaces[1], 0);
    SetLength(FVertices[1], 0);
  end;
  FChanged := True;
  Changed;
end;

// 0x000648e8
procedure TColObject.MergeVertices(Mesh: Integer; Distance: Single);
var
  Remap: array of Integer;
  Merged: array of Boolean;
  I, J, K, Count, NewCount, VertexCount: Integer;
  Sum, Delta: TAffineVector;
begin
  VertexCount := Length(FVertices[Mesh]);
  SetLength(Remap, VertexCount);
  SetLength(Merged, VertexCount);
  if VertexCount > 0 then FillChar(Merged[0], VertexCount, 0);
  NewCount := 0;
  for I := 0 to VertexCount - 1 do
    if not Merged[I] then
    begin
      Remap[I] := NewCount;
      Count := 0;
      for J := VertexCount - 1 downto I + 1 do
        if not Merged[J] then
        begin
          Delta := VectorSubtract(FVertices[Mesh][I].Position,
            FVertices[Mesh][J].Position);
          if VectorNorm(Delta) <= Distance * Distance then
          begin
            if Count = 0 then
            begin
              Sum := VectorAdd(FVertices[Mesh][I].Position,
                FVertices[Mesh][J].Position);
              Count := 2;
            end
            else
            begin
              AddVector(Sum, FVertices[Mesh][J].Position);
              Inc(Count);
            end;
            Remap[J] := NewCount;
            Merged[J] := True;
          end;
        end;
      if Count > 0 then
        FVertices[Mesh][I].Position := VectorScale(Sum, 1 / Count);
      Inc(NewCount);
    end;
  NewCount := 0;
  for I := 0 to VertexCount - 1 do
    if not Merged[I] then
    begin
      // Native compaction copies only the position, retaining destination metadata.
      FVertices[Mesh][NewCount].Position := FVertices[Mesh][I].Position;
      Inc(NewCount);
    end;
  SetLength(FVertices[Mesh], NewCount);
  for I := 0 to Length(FFaces[Mesh]) - 1 do
  begin
    for K := 0 to 2 do
      FFaces[Mesh][I].Indices[K] := Remap[FFaces[Mesh][I].Indices[K]];
    CalculateFace(Mesh, I, False);
  end;
  Changed;
end;

// 0x00064bb0
procedure TColObject.RemoveDegenerateFaces(Mesh: Integer);
var
  I, Count, OldCount: Integer;
begin
  Count := 0;
  OldCount := Length(FFaces[Mesh]);
  for I := 0 to OldCount - 1 do
    with FFaces[Mesh][I] do
      if (Indices[0] <> Indices[1]) and (Indices[0] <> Indices[2]) and
        (Indices[1] <> Indices[2]) then
      begin
        if Count < I then FFaces[Mesh][Count] := FFaces[Mesh][I];
        Inc(Count);
      end;
  SetLength(FFaces[Mesh], Count);
  if Count < OldCount then
  begin
    if Mesh = 0 then ClearParts(False, True, False, False, False);
    Changed;
  end;
end;

// 0x00064cb0
procedure TColObject.RemoveUnusedVertices(Mesh: Integer);
type
  TRemapEntry = packed record
    Used: Boolean;
    Index: Integer;
  end;
var
  Remap: array of TRemapEntry;
  I, J, Count, OldCount: Integer;
begin
  OldCount := Length(FVertices[Mesh]);
  if OldCount = 0 then Exit;
  FChanged := False;
  SetLength(Remap, OldCount);
  FillChar(Remap[0], OldCount * SizeOf(TRemapEntry), 0);
  for I := 0 to Length(FFaces[Mesh]) - 1 do
    for J := 0 to 2 do Remap[FFaces[Mesh][I].Indices[J]].Used := True;
  Count := 0;
  for I := 0 to OldCount - 1 do
    if Remap[I].Used then
    begin
      if Count < I then FVertices[Mesh][Count] := FVertices[Mesh][I];
      Remap[I].Index := Count;
      Inc(Count);
    end;
  SetLength(FVertices[Mesh], Count);
  for I := 0 to Length(FFaces[Mesh]) - 1 do
    for J := 0 to 2 do
      FFaces[Mesh][I].Indices[J] := Remap[FFaces[Mesh][I].Indices[J]].Index;
  FChanged := True;
  Changed;
end;

// 0x00065888
function ColMin(const A, B: TAffineVector): TAffineVector;
var K: Integer;
begin
  for K := 0 to 2 do
    if A[K] <= B[K] then Result[K] := A[K] else Result[K] := B[K];
end;

// 0x000658f8
function ColMax(const A, B: TAffineVector): TAffineVector;
var K: Integer;
begin
  for K := 0 to 2 do
    if B[K] <= A[K] then Result[K] := A[K] else Result[K] := B[K];
end;

// 0x00065968
procedure SortBounds(var Min, Max: TAffineVector);
var K: Integer; Value: Single;
begin
  for K := 0 to 2 do
    if Max[K] < Min[K] then
    begin
      Value := Min[K];
      Min[K] := Max[K];
      Max[K] := Value;
    end;
end;

// 0x000652ac
procedure TColObject.BuildFaceGroups;
type
  PNode = ^TNode;
  TNode = record
    Min, Max: TAffineVector;
    Count, Axis: Integer;
    Low, High: PNode;
  end;
var
  Nodes: array of PNode;
  SortedFaces: TColFaces;
  Root: PNode;
  GroupCount, GroupIndex, FaceIndex, I, J: Integer;

  // 0x00064ee0
  procedure CenterBounds(Node: PNode);
  var K: Integer; First: Boolean;
  begin
    First := True;
    for K := 0 to Length(Nodes) - 1 do
      if Nodes[K] = Node then
      begin
        if First then
        begin
          Node.Min := FFaces[0][K].Center;
          Node.Max := FFaces[0][K].Center;
          First := False;
        end
        else
        begin
          Node.Min := ColMin(Node.Min, FFaces[0][K].Center);
          Node.Max := ColMax(Node.Max, FFaces[0][K].Center);
        end;
      end;
  end;

  // 0x00064fcc
  procedure LongestAxis(Node: PNode);
  var K: Integer; Extent: Single;
  begin
    Extent := 0;
    for K := 0 to 2 do
      if Extent < Node.Max[K] - Node.Min[K] then
      begin
        Extent := Node.Max[K] - Node.Min[K];
        Node.Axis := K;
      end;
  end;

  // 0x00065000
  procedure Split(Node: PNode);
  var K: Integer; Midpoint: Single;
  begin
    CenterBounds(Node);
    LongestAxis(Node);
    if Node.Count < 51 then
    begin
      Node.Low := nil;
      Node.High := nil;
    end
    else
    begin
      Midpoint := (Node.Min[Node.Axis] + Node.Max[Node.Axis]) / 2;
      Node.Count := 0;
      Inc(GroupCount);
      New(Node.High);
      Node.High.Count := 0;
      Node.High.Min := Node.Min;
      Node.High.Max := Node.Max;
      Node.High.Min[Node.Axis] := Midpoint;
      New(Node.Low);
      Node.Low.Count := 0;
      Node.Low.Min := Node.Min;
      Node.Low.Max := Node.Max;
      Node.Low.Max[Node.Axis] := Midpoint;
      for K := 0 to Length(Nodes) - 1 do
        if Nodes[K] = Node then
          if Midpoint <= FFaces[0][K].Center[Node.Axis] then
          begin
            Nodes[K] := Node.High;
            Inc(Node.High.Count);
          end
          else
          begin
            Nodes[K] := Node.Low;
            Inc(Node.Low.Count);
          end;
      Split(Node.Low);
      Split(Node.High);
    end;
  end;

  // 0x00065160
  procedure Collect(Node: PNode);
  var K: Integer;
  begin
    if Node = nil then Exit;
    if Node.Count > 0 then
    begin
      FFaceGroups[GroupIndex].Min := Node.Min;
      FFaceGroups[GroupIndex].Max := Node.Max;
      FFaceGroups[GroupIndex].FirstFace := FaceIndex;
      for K := 0 to Length(FFaces[0]) - 1 do
        if Nodes[K] = Node then
        begin
          SortedFaces[FaceIndex] := FFaces[0][K];
          Inc(FaceIndex);
        end;
      FFaceGroups[GroupIndex].LastFace := FaceIndex - 1;
      Inc(GroupIndex);
    end;
    Collect(Node.Low);
    Collect(Node.High);
    Dispose(Node);
  end;

begin
  SetLength(FFaceGroups, 0);
  SetLength(Nodes, Length(FFaces[0]));
  if Length(FFaces[0]) > 80 then
  begin
    GroupCount := 1;
    New(Root);
    Root.Count := Length(Nodes);
    for I := 0 to Length(Nodes) - 1 do Nodes[I] := Root;
    Split(Root);
    for I := 0 to Length(FFaces[0]) - 1 do
      for J := 0 to 2 do
      begin
        Nodes[I].Min := ColMin(Nodes[I].Min,
          FVertices[0][FFaces[0][I].Indices[J]].Position);
        Nodes[I].Max := ColMax(Nodes[I].Max,
          FVertices[0][FFaces[0][I].Indices[J]].Position);
      end;
    SetLength(FFaceGroups, GroupCount);
    SetLength(SortedFaces, Length(FFaces[0]));
    GroupIndex := 0;
    FaceIndex := 0;
    Collect(Root);
    Move(SortedFaces[0], FFaces[0][0], Length(SortedFaces) * SizeOf(TColFace));
    SetLength(SortedFaces, 0);
  end;
  Changed;
end;

// 0x000655e0
procedure TColObject.UpdateBounds(Replace: Boolean);
var
  Min, Max, Position: TAffineVector;

  // 0x0006599c (nested in the native UpdateBounds)
  procedure CollectBounds;
  var I, J, K: Integer;
  begin
    if Length(FVertices[0]) > 0 then Min := FVertices[0][0].Position
    else if Length(FVertices[1]) > 0 then Min := FVertices[1][0].Position
    else if Length(FSpheres) > 0 then Min := FSpheres[0].Position
    else if Length(FBoxes) > 0 then Min := FBoxes[0].Min
    else
    begin
      Min := NullVector;
      Max := NullVector;
      Exit;
    end;
    Max := Min;
    for J := 0 to 1 do
      for I := 0 to Length(FVertices[J]) - 1 do
      begin
        Min := ColMin(Min, FVertices[J][I].Position);
        Max := ColMax(Max, FVertices[J][I].Position);
      end;
    for I := 0 to Length(FSpheres) - 1 do
    begin
      for K := 0 to 2 do Position[K] := FSpheres[I].Position[K] - FSpheres[I].Radius;
      Min := ColMin(Min, Position);
      for K := 0 to 2 do Position[K] := FSpheres[I].Position[K] + FSpheres[I].Radius;
      Max := ColMax(Max, Position);
    end;
    for I := 0 to Length(FBoxes) - 1 do
    begin
      SortBounds(FBoxes[I].Min, FBoxes[I].Max);
      Min := ColMin(Min, FBoxes[I].Min);
      Max := ColMax(Max, FBoxes[I].Max);
    end;
  end;

begin
  SortBounds(FBounds.Min, FBounds.Max);
  CollectBounds;
  if Replace then
  begin
    FBounds.Min := Min;
    FBounds.Max := Max;
  end
  else
  begin
    FBounds.Min := ColMin(FBounds.Min, Min);
    FBounds.Max := ColMax(FBounds.Max, Max);
  end;
  FBounds.Center := VectorAdd(FBounds.Min, FBounds.Max);
  ScaleVector(FBounds.Center, 0.5);
  Position := VectorSubtract(FBounds.Max, FBounds.Center);
  FBounds.Radius := VectorLength(Position);
  Changed;
end;

// 0x000656fc
procedure TColObject.Optimize(Full, ReplaceBounds: Boolean);
var Mesh: Integer;
begin
  if Full then
    for Mesh := 0 to 1 do
    begin
      MergeVertices(Mesh, 0.01);
      RemoveDegenerateFaces(Mesh);
      RemoveUnusedVertices(Mesh);
    end;
  BuildFaceGroups;
  UpdateBounds(ReplaceBounds);
end;

// 0x00065744
function TColObject.CheckDimensions(Mesh: Integer): Boolean;
var I, K: Integer;
begin
  Result := True;
  I := 0;
  while Result and (I < Length(FVertices[Mesh])) do
  begin
    for K := 0 to 2 do
      if (FVertices[Mesh][I].Position[K] < -255.99) or
        (FVertices[Mesh][I].Position[K] > 255.99) then Result := False;
    Inc(I);
  end;
end;

// 0x00063ce0
procedure TColObject.Save(OwnerWindow: HWND; Stream: TStream; FullOptimization: Boolean);
const
  Magics: array[0..2] of Cardinal = ($4C4C4F43, $324C4F43, $334C4F43);
  ModelTag: Cardinal = $32444543; // Native writer emits 'CED2', not the loaded model ID.
  Zero: Cardinal = 0;
var
  SphereCount, BoxCount, VertexCount, GroupCount, FaceCount: Integer;
  ShadowVertexCount, ShadowFaceCount: Integer;
  SphereBytes, BoxBytes, VertexBytes, GroupBytes, FaceBytes: Integer;
  ShadowVertexBytes, ShadowFaceBytes, DataSize, Offset, Value, Start: Integer;
  I: Integer;
  Flags: Cardinal;
  Name: array[0..19] of Char;

  // 0x00063a50
  procedure WriteBounds(const Bounds: TColBounds);
  begin
    if FFormat = 0 then
    begin
      Stream.Write(Bounds.Radius, 4);
      Stream.Write(Bounds.Center, 12);
      Stream.Write(Bounds.Min, 12);
      Stream.Write(Bounds.Max, 12);
    end
    else Stream.Write(Bounds, 40);
  end;

  // 0x00063ac8
  procedure WriteSphere(const Sphere: TColSphere);
  begin
    if FFormat = 0 then
    begin
      Stream.Write(Sphere.Radius, 4);
      Stream.Write(Sphere.Position, 12);
      Stream.Write(Sphere.Surface, 4);
    end
    else Stream.Write(Sphere, 20);
  end;

  // 0x00063b2c
  procedure WriteBox(const Box: TColBox);
  begin
    Stream.Write(Box, 28);
  end;

  // 0x00063b48
  procedure WriteVertex(const Vertex: TColVertex);
  var PackedPosition: array[0..2] of SmallInt; K: Integer; Scaled: Single;
  begin
    if FFormat = 0 then Stream.Write(Vertex.Position, 12)
    else
    begin
      for K := 0 to 2 do
      begin
        Scaled := Vertex.Position[K] * 128;
        // 0x00062b38: FISTP follows the current x87 rounding mode.
        PackedPosition[K] := Round(Scaled);
      end;
      Stream.Write(PackedPosition, 6);
    end;
  end;

  // 0x00063bd8
  procedure WriteFace(const Face: TColFace);
  var K: Integer;
  begin
    if FFormat = 0 then
    begin
      for K := 0 to 2 do
      begin
        Stream.Write(Face.Indices[K], 2);
        Stream.Write(Zero, 2);
      end;
      Stream.Write(Face.Surface, 4);
    end
    else
    begin
      Stream.Write(Face.Indices, 6);
      Stream.Write(Face.Surface[0], 1);
      Stream.Write(Face.Surface[3], 1);
    end;
  end;

  // 0x00063cc4
  procedure WriteGroup(const Group: TColFaceGroup);
  begin
    Stream.Write(Group, 28);
  end;

  procedure WriteOffset(Count: Integer);
  begin
    Value := Ord(Count > 0) * Offset;
    Stream.Write(Value, 4);
  end;

begin
  if (FFormat <> 0) and not CheckDimensions(0) then
  begin
    if MessageBox(OwnerWindow, PChar('The collision mesh for "' + FModelName +
      '"'#13#10'exceeds the dimension limits of +/- 255.99 per axis.'#13#10#13#10 +
      'Do you still want to export it? (in Col 1 format)'),
      'Exporting Problem', MB_YESNO or MB_ICONQUESTION) <> IDYES then Exit;
    SetFormat(0);
  end
  else if (FFormat = 2) and not CheckDimensions(1) then
  begin
    if MessageBox(OwnerWindow, PChar('The shadow mesh for "' + FModelName +
      '"'#13#10'exceeds the dimension limits of +/- 255.99 per axis.'#13#10#13#10 +
      'Do you still want to export the model? (without shadow mesh)'),
      'Exporting Problem', MB_YESNO or MB_ICONQUESTION) <> IDYES then Exit;
    SetFormat(0);
  end;
  if FOptimizeOnSave then Optimize(FullOptimization, False);
  SphereCount := Length(FSpheres);
  SphereBytes := SphereCount * 20 + Ord(FFormat = 0) * 4;
  BoxCount := Length(FBoxes);
  BoxBytes := BoxCount * 28 + Ord(FFormat = 0) * 4;
  VertexCount := Length(FVertices[0]);
  VertexBytes := (Ord(FFormat = 0) + 1) * VertexCount * 6 + Ord(FFormat = 0) * 4;
  if (FFormat <> 0) and (VertexCount * 6 mod 4 > 0) then Inc(VertexBytes, 2);
  GroupCount := Length(FFaceGroups) * Ord(FFormat <> 0);
  GroupBytes := GroupCount * 28 + Ord(GroupCount > 0) * 4;
  FaceCount := Length(FFaces[0]);
  FaceBytes := (Ord(FFormat = 0) + 1) * FaceCount * 8 + Ord(FFormat = 0) * 4;
  ShadowVertexCount := Length(FVertices[1]) * Ord(FFormat = 2);
  ShadowVertexBytes := ShadowVertexCount * 6;
  if ShadowVertexCount * 6 mod 4 > 0 then Inc(ShadowVertexBytes, 2);
  ShadowFaceCount := Length(FFaces[1]) * Ord(FFormat = 2);
  ShadowFaceBytes := ShadowFaceCount * 8;
  DataSize := 64 + Ord(FFormat <> 0) * 36 + Ord(FFormat > 1) * 12 +
    SphereBytes + Ord(FFormat = 0) * 4 + BoxBytes + VertexBytes + GroupBytes +
    FaceBytes + ShadowVertexBytes + ShadowFaceBytes;
  FillChar(Name, SizeOf(Name), 0);
  Move(PChar(FModelName)^, Name, MinInteger(Length(FModelName), 19));
  Start := Stream.Position;
  Stream.Write(Magics[FFormat], 4);
  Stream.Write(DataSize, 4);
  Stream.Write(Name, 20);
  Stream.Write(ModelTag, 4);
  WriteBounds(FBounds);
  if FFormat = 0 then
  begin
    Stream.Write(SphereCount, 4);
    for I := 0 to SphereCount - 1 do WriteSphere(FSpheres[I]);
    Stream.Write(Zero, 4);
    Stream.Write(BoxCount, 4);
    for I := 0 to BoxCount - 1 do WriteBox(FBoxes[I]);
    Stream.Write(VertexCount, 4);
    for I := 0 to VertexCount - 1 do WriteVertex(FVertices[0][I]);
    Stream.Write(FaceCount, 4);
    for I := 0 to FaceCount - 1 do WriteFace(FFaces[0][I]);
  end
  else
  begin
    Stream.Write(SphereCount, 2);
    Stream.Write(BoxCount, 2);
    Stream.Write(FaceCount, 4);
    Flags := 0;
    if ((VertexCount > 0) and (FaceCount > 0)) or (SphereCount > 0) or
      (BoxCount > 0) then
    begin
      Flags := 2;
      if GroupCount > 0 then Flags := Flags or 8;
    end;
    if (ShadowVertexCount > 0) and (ShadowFaceCount > 0) then Flags := Flags or 16;
    Stream.Write(Flags, 4);
    Offset := 104 + Ord(FFormat > 1) * 12;
    WriteOffset(SphereCount);
    Inc(Offset, SphereBytes);
    WriteOffset(BoxCount);
    Inc(Offset, BoxBytes);
    Stream.Write(Zero, 4);
    WriteOffset(VertexCount);
    Inc(Offset, VertexBytes + GroupBytes);
    WriteOffset(FaceCount);
    Inc(Offset, FaceBytes);
    Stream.Write(Zero, 4);
    if FFormat = 2 then
    begin
      Stream.Write(ShadowFaceCount, 4);
      WriteOffset(ShadowVertexCount);
      Inc(Offset, ShadowVertexBytes);
      WriteOffset(ShadowFaceCount);
      Inc(Offset, ShadowFaceBytes);
    end;
    for I := 0 to SphereCount - 1 do WriteSphere(FSpheres[I]);
    for I := 0 to BoxCount - 1 do WriteBox(FBoxes[I]);
    for I := 0 to VertexCount - 1 do WriteVertex(FVertices[0][I]);
    if VertexCount * 6 mod 4 > 0 then Stream.Write(Zero, 2);
    if GroupCount > 0 then
    begin
      for I := 0 to GroupCount - 1 do WriteGroup(FFaceGroups[I]);
      Stream.Write(GroupCount, 4);
    end;
    for I := 0 to FaceCount - 1 do WriteFace(FFaces[0][I]);
    for I := 0 to ShadowVertexCount - 1 do WriteVertex(FVertices[1][I]);
    if ShadowVertexCount * 6 mod 4 > 0 then Stream.Write(Zero, 2);
    for I := 0 to ShadowFaceCount - 1 do WriteFace(FFaces[1][I]);
    if Stream.Position <> Start + 4 + Offset then ShowMessage('problem');
  end;
end;

end.
