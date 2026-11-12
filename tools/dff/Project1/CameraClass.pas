unit CameraClass;

interface

uses Windows, VectorGeometry;

type
  // Native old-style object: 73 bytes, with FOV at +0x28.
  // Field names are descriptive; the original unit name is in PACKAGEINFO.
  TCamera = object
    Position, Target, Up: TAffineVector;
    Fov, Aspect, NearClip, FarClip: Double;
    Orthographic: Boolean;
    // 0x00071de4
    constructor Init;
  end;

var
  // 0x00078a94; descriptive name for the native viewer-window global.
  ViewWindow: HWND;

implementation

// 0x00071de4
constructor TCamera.Init;
begin
  Fov := 45;
  NearClip := 0.2;
  FarClip := 10000;
end;

end.
