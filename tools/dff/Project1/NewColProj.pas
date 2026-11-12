unit NewColProj;

// Recovery of the original Project1 main unit.

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, ComCtrls, Buttons, OpenGL12, RenderWareDFF, rwtxd, CameraClass,
  ColObject;

type
  TForm1 = class(TForm)
    Edit1: TEdit;
    Edit2: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    OpenDffFIle: TOpenDialog;
    SaveDffFile: TSaveDialog;
    OpenColFile: TOpenDialog;
    SaveColFile: TSaveDialog;
    GLPanel: TPanel;
    Edit3: TEdit;
    Label3: TLabel;
    OpenTxdFile: TOpenDialog;
    ComboRenderMode: TComboBox;
    RadioGroup1: TRadioGroup;
    RadioMakeColEmpty: TRadioButton;
    RadioMakeColBox: TRadioButton;
    RadioMakeColFaces: TRadioButton;
    Button3: TButton;
    chkMakeColOptimize: TCheckBox;
    Button1: TBitBtn;
    Button5: TBitBtn;
    Button2: TBitBtn;
    Label4: TLabel;
    ButtonImpNV: TBitBtn;
    ButtonRefreshTXD: TBitBtn;
    Button4: TBitBtn;
    ImageNoCol: TImage;
    ImageHasCol: TImage;
    Memo1: TMemo;
    ButtonEasyColors: TBitBtn;
    Panel1: TPanel;
    GroupBox1: TGroupBox;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    TrackBar1: TTrackBar;
    TrackBar2: TTrackBar;
    TrackBar3: TTrackBar;
    GroupBox2: TGroupBox;
    Label12: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    TrackBar4: TTrackBar;
    TrackBar5: TTrackBar;
    TrackBar6: TTrackBar;
    BitBtn1: TBitBtn;
    Shape1: TShape;
    Shape2: TShape;
    GroupBox3: TGroupBox;
    TrackBar7: TTrackBar;
    TrackBar8: TTrackBar;
    // 0x00072fe8
    procedure Button1Click(Sender: TObject);
    // 0x0007290c
    procedure FormCreate(Sender: TObject);
    // 0x00073084
    procedure Button2Click(Sender: TObject);
    // 0x0007315c
    procedure Button3Click(Sender: TObject);
    // 0x00073428
    procedure Button4Click(Sender: TObject);
    // 0x000727c4
    procedure GLPanelResize(Sender: TObject);
    // 0x00073960
    procedure FormMouseWheel(Sender: TObject; Shift: TShiftState; WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    // 0x000735c4
    procedure Button5Click(Sender: TObject);
    // 0x000739cc
    procedure ComboRenderModeChange(Sender: TObject);
    // 0x000739e4
    procedure FormResize(Sender: TObject);
    // 0x00073ad8
    procedure GlPanelMouseDown(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    // 0x00073b18
    procedure GlPanelMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
    // 0x00073be0
    procedure GlPanelMouseUp(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    // 0x00072aec
    procedure UpdateAllOpenSaveDialogsToFilePath(FilePath: string);
    // 0x00072c10
    procedure SetPedModelExportType;
    // 0x00072cf4
    procedure LoadDffFile(FilePath: string);
    // 0x00073bf0
    procedure ButtonRefreshTXDClick(Sender: TObject);
    // 0x00073c64
    procedure ButtonImpNVClick(Sender: TObject);
    // 0x00073f20
    procedure ButtonEasyColorsClick(Sender: TObject);
    // 0x00074048
    procedure UpdateEditVertexColors;
    // 0x000740fc
    procedure UpdateEditNightColors;
    // 0x000741b0
    procedure Shape1MouseUp(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    // 0x0007422c
    procedure Shape2MouseUp(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    // 0x00073f94
    function SliderToNormalVal(Value: Integer): Single;
    // 0x00073fc8
    function MinSliderToNormalVal(Value: Integer): Single;
    // 0x00074008
    function MaxSliderToNormalVal(Value: Integer): Single;
    // 0x000742a8
    procedure TrackBar1Change(Sender: TObject);
    // 0x000742b0
    procedure TrackBar2Change(Sender: TObject);
    // 0x000742b8
    procedure TrackBar3Change(Sender: TObject);
    // 0x000742c0
    procedure TrackBar4Change(Sender: TObject);
    // 0x000742c8
    procedure TrackBar5Change(Sender: TObject);
    // 0x000742d0
    procedure TrackBar6Change(Sender: TObject);
    // 0x000742d8
    procedure BitBtn1Click(Sender: TObject);
    // 0x00074578
    procedure TrackBar7Change(Sender: TObject);
    // 0x0007458c
    procedure TrackBar8Change(Sender: TObject);
  private
    // Descriptive field names; storage offsets are confirmed by FormCreate.
    FDeviceContext: HDC;           // +0x3c4
    FRenderingContext: HGLRC;       // +0x3c8
    // 0x000736a8; name descriptive, native method is not published.
    procedure RenderIdle(Sender: TObject; var Done: Boolean);
  end;

var
  // 0x00078aa4
  Form1: TForm1;

implementation

{$R *.dfm}

// Descriptive global names; original spelling is not available.
var
  // 0x00075fc0
  ViewAngleX: Single = -90.0;
  // 0x00075fc4
  ViewAngleY: Single = 350.0;
  // 0x00075fc8
  ViewOffsetX: Single = 0.0;
  // 0x00075fcc
  ViewOffsetY: Single = 0.0;
  // 0x00075fd0
  ViewDistance: Single = 50.0;
  // 0x00075fdc
  ModelRadius: Single = 0.0;
  // 0x00075fe4
  IsPedModel: Boolean = False;
  // 0x00075fe8
  PedExportType: Integer = 0;
  // 0x00075ff0
  VertexCount: Integer = 0;
  // 0x00075ff4
  FaceCount: Integer = 0;
  // 0x00075ff8
  NightVertexCount: Integer = 0;
  // 0x00078af4
  DffModel: TDffLoader;
  // 0x00078afc
  TxdModel: Ttxdloader;
  // 0x00078b00
  NightTxdModel: Ttxdloader;
  // 0x00078af8
  NightDffModel: TDffLoader;
  // 0x00075fd4
  ViewZoom: Single = 0.0;
  // 0x00075fd8
  MouseDragMode: Integer = 0;
  // 0x00075fe0
  ViewDragScale: Single = 1.0;
  // 0x00075fec
  HasEmbeddedCol: Boolean = False;
  // 0x00075ffc
  RenderMode: Integer = 0;
  // 0x00078aa8
  Camera: TCamera;
  // 0x00078b04
  LastMouseX: Integer;
  // 0x00078b08
  LastMouseY: Integer;
  // 0x00078b0c
  EditColorDialog: TColorDialog;

// 0x00071ebc (import thunk)
function GenerateColFromDff(SourceDff: PChar; Mode: Integer; DestinationCol: PChar): Integer;
  cdecl; external 'kdff.dll' name 'GenerateColFromDff';

// 0x00071ec4 (import thunk)
function AppendColToDff(SourceDff, SourceCol, DestinationDff: PChar): Integer;
  cdecl; external 'kdff.dll' name 'AppendColToDff';

// 0x00071ecc (import thunk)
function ImportNVCFromDff(SourceDff, NightDff, DestinationDff: PChar): Integer;
  cdecl; external 'kdff.dll' name 'ImportNVCFromDff';

// 0x00071ed4 (import thunk)
function EasyVertexColors(SourceDff: PChar; DayColor, NightColor: Cardinal;
  DayX, DayY, DayZ, NightX, NightY, NightZ, Minimum, Maximum: Single;
  DestinationDff: PChar): Integer;
  cdecl; external 'kdff.dll' name 'EasyVertexColors';

// 0x00072aec
procedure TForm1.UpdateAllOpenSaveDialogsToFilePath(FilePath: string);
begin
  OpenDffFIle.FileName := ChangeFileExt(FilePath, '.dff');
  OpenTxdFile.FileName := ChangeFileExt(FilePath, '.txd');
  OpenColFile.FileName := ChangeFileExt(FilePath, '.col');
  SaveDffFile.FileName := ChangeFileExt(FilePath, '.dff');
  SaveColFile.FileName := ChangeFileExt(FilePath, '.col');
end;

// 0x00073084
procedure TForm1.Button2Click(Sender: TObject);
begin
  if OpenColFile.Execute then
  begin
    Edit2.Text := OpenColFile.FileName;
    if not HasEmbeddedCol then
      Button4.Enabled := True;
    Memo1.Lines.Add('Opened col: ' + Edit2.Text);
  end;
end;

// 0x00073428
procedure TForm1.Button4Click(Sender: TObject);
var
  ErrorCode: Integer;
begin
  if SaveDffFile.Execute then
  begin
    ErrorCode := AppendColToDff(PChar(Edit1.Text), PChar(Edit2.Text),
      PChar(SaveDffFile.FileName));
    if ErrorCode = 0 then
    begin
      UpdateAllOpenSaveDialogsToFilePath(SaveDffFile.FileName);
      Memo1.Lines.Add('Saved dff with SA-MP col: ' + SaveDffFile.FileName);
    end
    else
      Memo1.Lines.Add('Append col to dff error: ' + IntToStr(ErrorCode));
  end;
end;

// 0x000727c4
procedure TForm1.GLPanelResize(Sender: TObject);
begin
  glViewport(0, 0, GLPanel.Width, GLPanel.Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity;
  if Camera.Orthographic then
    glOrtho(-GLPanel.Width / 10, GLPanel.Width / 10,
      -GLPanel.Height / 10, GLPanel.Height / 10, Camera.NearClip, Camera.FarClip)
  else
    gluPerspective(Camera.Fov, GLPanel.Width / GLPanel.Height,
      Camera.NearClip, Camera.FarClip);
  glMatrixMode(GL_MODELVIEW);
end;

// 0x00073960
procedure TForm1.FormMouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
var
  PanelPoint: TPoint;
begin
  PanelPoint := GLPanel.ScreenToClient(MousePos);
  if (PanelPoint.X >= 0) and (PanelPoint.Y >= 0) and
     (PanelPoint.X <= GLPanel.Width) and (PanelPoint.Y <= GLPanel.Height) then
  begin
    Handled := True;
    ViewZoom := ViewZoom + WheelDelta / 80;
  end;
end;

// 0x000739cc
procedure TForm1.ComboRenderModeChange(Sender: TObject);
begin
  RenderMode := ComboRenderMode.ItemIndex;
end;

// 0x000739e4
procedure TForm1.FormResize(Sender: TObject);
begin
  Memo1.Left := 1;
  Memo1.Top := Height - 140;
  Memo1.Height := 90;
  Memo1.Width := Width - 20;
  GLPanel.Left := 1;
  GLPanel.Width := Width - 20;
  GLPanel.Height := Memo1.Top - GLPanel.Top - 5;
  if Panel1.Visible then
  begin
    Panel1.Top := GLPanel.Top;
    Panel1.Left := Width - Panel1.Width - 20;
    GLPanel.Width := Width - Panel1.Width - 20;
    Panel1.Height := GLPanel.Height;
  end;
  GLPanelResize(Sender);
end;

// 0x00073ad8
procedure TForm1.GlPanelMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if Button = mbLeft then
  begin
    MouseDragMode := 1;
    LastMouseX := X;
    LastMouseY := Y;
  end;
  if Button = mbRight then
  begin
    MouseDragMode := 2;
    LastMouseX := X;
    LastMouseY := Y;
  end;
end;

// 0x00073b18
procedure TForm1.GlPanelMouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
  if MouseDragMode <> 0 then
  begin
    if MouseDragMode = 1 then
    begin
      ViewAngleX := ViewAngleX + (LastMouseX - X);
      ViewAngleY := ViewAngleY + (LastMouseY - Y);
    end
    else
    begin
      ViewOffsetX := ViewOffsetX + (X - LastMouseX) * 0.1 * ViewDragScale;
      ViewOffsetY := ViewOffsetY + (LastMouseY - Y) * 0.1 * ViewDragScale;
    end;
    LastMouseX := X;
    LastMouseY := Y;
  end;
end;

// 0x00073be0
procedure TForm1.GlPanelMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  MouseDragMode := 0;
end;

// 0x00073f94
function TForm1.SliderToNormalVal(Value: Integer): Single;
const
  Scale: Single = 0.25;
  Offset: Single = -1.0;
var
  Scaled: Single;
begin
  Scaled := Value * Scale;
  Result := Offset + Scaled;
end;

// 0x00073fc8
function TForm1.MinSliderToNormalVal(Value: Integer): Single;
const
  Offset: Single = 0.0;
var
  Scaled: Single;
begin
  Scaled := Value * 0.1;
  Result := Offset + Scaled;
end;

// 0x00074008
function TForm1.MaxSliderToNormalVal(Value: Integer): Single;
const
  Offset: Single = 1.0;
var
  Scaled: Single;
begin
  Scaled := Value * 0.1;
  Result := Offset - Scaled;
end;

// 0x00072c10
procedure TForm1.SetPedModelExportType;
var
  Rotation: TDFFRotation;
begin
  if IsPedModel then
  begin
    Rotation := DffModel.Clumps[0].FrameList.Data.Frames[1].Rotation;
    PedExportType := 0;
    if (Rotation[0, 1] = 1) and (Rotation[1, 2] = 1) and
       (Rotation[2, 0] = 1) then
      PedExportType := 1;
    if (Rotation[0, 1] = 1) and (Rotation[1, 0] = -1) and
       (Rotation[2, 2] = 1) then
      PedExportType := 2;
  end;
end;

// 0x00072cf4
procedure TForm1.LoadDffFile(FilePath: string);
begin
  DffModel.LoadFromFile(FilePath);
  ModelRadius := DffModel.Clumps[0].Radius;
  ViewDragScale := ModelRadius / 30;
  ViewDistance := ModelRadius * 3;
  ViewZoom := 0;
  ViewOffsetX := 0;
  ViewOffsetY := 0;
  ViewAngleX := -90;
  ViewAngleY := 350;
  VertexCount := DffModel.Clumps[0].GeometryList.Geometries[0].Data.VertexCount;
  FaceCount := DffModel.Clumps[0].GeometryList.Geometries[0].Data.TriangleCount;
  Memo1.Lines.Add('Opened dff: ' + OpenDffFIle.FileName + ' Frame count: ' +
    IntToStr(Int64(DffModel.Clumps[0].FrameList.Data.Count)) + ' radius: ' +
    FloatToStr(ModelRadius) + ' vertices: ' + IntToStr(VertexCount) +
    ' faces: ' + IntToStr(FaceCount));
  if Panel1.Visible then
  begin
    Panel1.Visible := False;
    FormResize(Form1);
  end;
  if DffModel.Clumps[0].FrameList.Data.Count > 30 then
  begin
    IsPedModel := True;
    SetPedModelExportType;
    Button3.Enabled := False;
    ButtonImpNV.Enabled := False;
    ButtonEasyColors.Enabled := False;
  end
  else
  begin
    IsPedModel := False;
    Button3.Enabled := True;
    ButtonImpNV.Enabled := True;
    ButtonEasyColors.Enabled := True;
  end;
  if DffModel.Clumps[0].HasCol then
  begin
    ImageNoCol.Visible := False;
    ImageHasCol.Visible := True;
    HasEmbeddedCol := True;
    Button4.Enabled := False;
  end
  else
  begin
    ImageNoCol.Visible := True;
    ImageHasCol.Visible := False;
    HasEmbeddedCol := False;
  end;
end;

// 0x00072fe8
procedure TForm1.Button1Click(Sender: TObject);
begin
  if OpenDffFIle.Execute then
  begin
    Edit1.Text := OpenDffFIle.FileName;
    LoadDffFile(OpenDffFIle.FileName);
    UpdateAllOpenSaveDialogsToFilePath(OpenDffFIle.FileName);
  end;
end;

// 0x00073c64
procedure TForm1.ButtonImpNVClick(Sender: TObject);
var
  ErrorCode: Integer;
begin
  if OpenDffFIle.Execute then
  begin
    NightDffModel.LoadFromFile(OpenDffFIle.FileName);
    NightVertexCount := NightDffModel.Clumps[0].GeometryList.Geometries[0].Data.VertexCount;
    if NightVertexCount <> VertexCount then
      Application.MessageBox(PChar('Models have different vertex counts: (' +
        IntToStr(VertexCount) + ' vs ' + IntToStr(NightVertexCount) + ')'),
        'Warning', MB_ICONWARNING);
    if SaveDffFile.Execute then
    begin
      ErrorCode := ImportNVCFromDff(PChar(Edit1.Text),
        PChar(OpenDffFIle.FileName), PChar(SaveDffFile.FileName));
      if ErrorCode = 0 then
      begin
        Memo1.Lines.Add('Imported NVC from file: ' + OpenDffFIle.FileName);
        LoadDffFile(SaveDffFile.FileName);
        Edit1.Text := SaveDffFile.FileName;
      end
      else
        Memo1.Lines.Add('Failed to import NVC: ' + IntToStr(ErrorCode));
    end;
  end;
end;

// 0x000742d8
procedure TForm1.BitBtn1Click(Sender: TObject);
var
  DayColor, NightColor: Cardinal;
  DayX, DayY, DayZ, NightX, NightY, NightZ, Minimum, Maximum: Single;
  ErrorCode: Integer;
begin
  DayColor := Cardinal(Shape1.Brush.Color);
  NightColor := Cardinal(Shape2.Brush.Color);
  DayX := SliderToNormalVal(TrackBar1.Position);
  DayY := SliderToNormalVal(TrackBar2.Position);
  DayZ := SliderToNormalVal(TrackBar3.Position);
  NightX := SliderToNormalVal(TrackBar4.Position);
  NightY := SliderToNormalVal(TrackBar5.Position);
  NightZ := SliderToNormalVal(TrackBar6.Position);
  Minimum := MinSliderToNormalVal(TrackBar7.Position);
  Maximum := MaxSliderToNormalVal(TrackBar8.Position);
  if SaveDffFile.Execute then
  begin
    ErrorCode := EasyVertexColors(PChar(Edit1.Text), DayColor, NightColor,
      DayX, DayY, DayZ, NightX, NightY, NightZ, Minimum, Maximum,
      PChar(SaveDffFile.FileName));
    if ErrorCode = 0 then
    begin
      Memo1.Lines.Add('Saved with easy vertex colors: ' + SaveDffFile.FileName);
      LoadDffFile(SaveDffFile.FileName);
      Edit1.Text := SaveDffFile.FileName;
    end
    else
      Memo1.Lines.Add('Easy Colors Failed: ' + IntToStr(ErrorCode));
  end;
end;

// 0x00074048
procedure TForm1.UpdateEditVertexColors;
begin
  if VertexCount > 0 then
    DffModel.SetEditColors(Cardinal(Shape1.Brush.Color), 0,
      SliderToNormalVal(TrackBar1.Position), SliderToNormalVal(TrackBar2.Position),
      SliderToNormalVal(TrackBar3.Position), MinSliderToNormalVal(TrackBar7.Position),
      MaxSliderToNormalVal(TrackBar8.Position));
end;

// 0x000740fc
procedure TForm1.UpdateEditNightColors;
begin
  if VertexCount > 0 then
    DffModel.SetEditNightColors(Cardinal(Shape2.Brush.Color), 0,
      SliderToNormalVal(TrackBar4.Position), SliderToNormalVal(TrackBar5.Position),
      SliderToNormalVal(TrackBar6.Position), MinSliderToNormalVal(TrackBar7.Position),
      MaxSliderToNormalVal(TrackBar8.Position));
end;

// 0x000741b0
procedure TForm1.Shape1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  EditColorDialog := TColorDialog.Create(Form1);
  EditColorDialog.Options := [cdFullOpen, cdAnyColor];
  EditColorDialog.Color := Shape1.Brush.Color;
  if EditColorDialog.Execute then
  begin
    Shape1.Brush.Color := EditColorDialog.Color;
    UpdateEditVertexColors;
  end;
end;

// 0x0007422c
procedure TForm1.Shape2MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  EditColorDialog := TColorDialog.Create(Form1);
  EditColorDialog.Options := [cdFullOpen, cdAnyColor];
  EditColorDialog.Color := Shape2.Brush.Color;
  if EditColorDialog.Execute then
  begin
    Shape2.Brush.Color := EditColorDialog.Color;
    UpdateEditNightColors;
  end;
end;

// 0x00073f20
procedure TForm1.ButtonEasyColorsClick(Sender: TObject);
begin
  if Panel1.Visible then
  begin
    Panel1.Visible := False;
    FormResize(Sender);
    Exit;
  end;
  Panel1.Visible := True;
  Panel1.Left := Form1.Width - Panel1.Width - 10;
  Panel1.Height := GLPanel.Height;
  UpdateEditVertexColors;
  UpdateEditNightColors;
  FormResize(Sender);
end;

// 0x000742a8
procedure TForm1.TrackBar1Change(Sender: TObject);
begin
  UpdateEditVertexColors;
end;

// 0x000742b0
procedure TForm1.TrackBar2Change(Sender: TObject);
begin
  UpdateEditVertexColors;
end;

// 0x000742b8
procedure TForm1.TrackBar3Change(Sender: TObject);
begin
  UpdateEditVertexColors;
end;

// 0x000742c0
procedure TForm1.TrackBar4Change(Sender: TObject);
begin
  UpdateEditNightColors;
end;

// 0x000742c8
procedure TForm1.TrackBar5Change(Sender: TObject);
begin
  UpdateEditNightColors;
end;

// 0x000742d0
procedure TForm1.TrackBar6Change(Sender: TObject);
begin
  UpdateEditNightColors;
end;

// 0x00074578
procedure TForm1.TrackBar7Change(Sender: TObject);
begin
  UpdateEditNightColors;
  UpdateEditVertexColors;
end;

// 0x0007458c
procedure TForm1.TrackBar8Change(Sender: TObject);
begin
  UpdateEditNightColors;
  UpdateEditVertexColors;
end;

// 0x000735c4
procedure TForm1.Button5Click(Sender: TObject);
begin
  if OpenTxdFile.Execute then
  begin
    Edit3.Text := OpenTxdFile.FileName;
    Memo1.Lines.Add('Opened txd: ' + OpenTxdFile.FileName);
    TxdModel.LoadFromFile(OpenTxdFile.FileName);
  end;
end;

// 0x00073bf0
procedure TForm1.ButtonRefreshTXDClick(Sender: TObject);
begin
  if Length(Edit3.Text) > 0 then
    TxdModel.LoadFromFile(Edit3.Text);
end;

// Descriptive name; the original free-procedure name is not in RTTI.
// 0x000726e8
procedure InitializeGL;
begin
  glClearColor(0, 0, 0, 0);
  glShadeModel(GL_SMOOTH);
  glClearDepth(1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDisable(GL_DITHER);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  Camera.Position[0] := 366.15;
  Camera.Position[1] := 1762.83;
  Camera.Position[2] := 158.91;
  Camera.Target[0] := 365.31;
  Camera.Target[1] := 1763.38;
  Camera.Target[2] := 158.06;
  Camera.Up[0] := 0;
  Camera.Up[1] := 0;
  Camera.Up[2] := 1;
  Camera.Orthographic := False;
  Camera.Fov := 40;
end;

// 0x0007290c
procedure TForm1.FormCreate(Sender: TObject);
var
  PixelFormat: TPixelFormatDescriptor;
begin
  Memo1.Clear;
  Camera.Init;
  DecimalSeparator := '.';
  InitOpenGL;
  FDeviceContext := GetDC(GLPanel.Handle);
  // The original assigns these five PFD fields without clearing the local record.
  PixelFormat.nSize := SizeOf(PixelFormat);
  PixelFormat.nVersion := 1;
  PixelFormat.dwFlags := PFD_DRAW_TO_WINDOW or PFD_SUPPORT_OPENGL or PFD_DOUBLEBUFFER;
  PixelFormat.iPixelType := PFD_TYPE_RGBA;
  PixelFormat.cColorBits := 32;
  SetPixelFormat(FDeviceContext, ChoosePixelFormat(FDeviceContext, @PixelFormat),
    @PixelFormat);
  FRenderingContext := wglCreateContext(FDeviceContext);
  wglMakeCurrent(FDeviceContext, FRenderingContext);
  GetDeviceCaps(GetDC(GetDesktopWindow), BITSPIXEL);
  InitializeGL;
  GLPanelResize(Sender);
  ReadExtensions;
  ReadWGLExtensions;
  Application.OnIdle := RenderIdle;
  ViewWindow := GLPanel.Handle;
  DffModel := TDffLoader.Create;
  TxdModel := Ttxdloader.Create;
  NightTxdModel := Ttxdloader.Create;
  NightDffModel := TDffLoader.Create;
  TrackBar1.Position := TrackBar1.Max div 2;
  TrackBar2.Position := TrackBar2.Max div 2;
  TrackBar3.Position := TrackBar3.Max div 2;
  TrackBar4.Position := TrackBar4.Max div 2;
  TrackBar5.Position := TrackBar5.Max div 2;
  TrackBar6.Position := TrackBar6.Max div 2;
end;

// 0x000736a8
procedure TForm1.RenderIdle(Sender: TObject; var Done: Boolean);
var
  Night: Boolean;
  ViewWidth, ViewHeight: Integer;
begin
  Night := RenderMode = 1;
  ViewWidth := GLPanel.Width;
  ViewHeight := GLPanel.Height;
  if Night then glClearColor(0.1, 0.1, 0.19, 1)
  else glClearColor(0.56, 0.56, 0.59, 1);
  glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
  glLoadIdentity;
  glTranslatef(ViewOffsetX, ViewOffsetY, -ViewDistance + ViewZoom * ViewDragScale);
  glScalef(-1, 1, 1);
  glRotatef(-ViewAngleY, 1, 0, 0);
  glRotatef(ViewAngleX, 0, 1, 0);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, ViewWidth, ViewHeight);
  glColor4f(1, 1, 1, 1);
  glScalef(-1, 1, 1);
  if not IsPedModel then
  begin
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 0, 1, 0);
  end
  else
  begin
    if PedExportType = 0 then glRotatef(-90, 0, 1, 0);
    if PedExportType = 1 then glRotatef(90, 0, 0, 1);
    if PedExportType = 2 then
    begin
      glRotatef(90, 0, 0, 1);
      glRotatef(90, 0, 1, 0);
    end;
  end;
  glDisable(GL_CULL_FACE);
  if not Panel1.Visible then
  begin
    if Night then DffModel.Draw(TxdModel, TxdModel, False, 0, 1, True)
    else DffModel.Draw(TxdModel, TxdModel, False, 0, 0, True);
  end;
  if Panel1.Visible then
  begin
    if Night then DffModel.Draw(TxdModel, TxdModel, False, 0, 3, True)
    else DffModel.Draw(TxdModel, TxdModel, False, 0, 2, True);
  end;
  SwapBuffers(FDeviceContext);
  // Native code leaves the caller's Done flag unchanged.
end;

// 0x0007315c
procedure TForm1.Button3Click(Sender: TObject);
var
  Mode, ErrorCode: Integer;
  Col: TColObject;
  Stream: TFileStream;
begin
  if SaveColFile.Execute then
  begin
    Edit2.Text := SaveColFile.FileName;
    Mode := Ord(RadioMakeColBox.Checked);
    if RadioMakeColFaces.Checked then Mode := 2;
    ErrorCode := GenerateColFromDff(PChar(Edit1.Text), Mode, PChar(Edit2.Text));
    if ErrorCode = 0 then
    begin
      if not HasEmbeddedCol then Button4.Enabled := True;
      if (Mode = 2) and chkMakeColOptimize.Checked then
      begin
        Memo1.Lines.Add('Performing col optimization');
        Col := TColObject.Create(nil);
        Col.LoadFromFile(Edit2.Text);
        Col.Optimize(True, True);
        Stream := TFileStream.Create(Edit2.Text, fmCreate);
        Col.Save(0, Stream, False);
        Stream.Free;
        // Native handler does not free the TColObject instance.
      end;
      UpdateAllOpenSaveDialogsToFilePath(SaveColFile.FileName);
      Memo1.Lines.Add('Saved col: ' + Edit2.Text);
    end
    else
    begin
      Memo1.Lines.Add('Make Col error: ' + IntToStr(ErrorCode));
      Edit2.Text := '';
    end;
  end;
end;

end.
