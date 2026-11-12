program Project1;

uses
  Forms,
  NewColProj in 'NewColProj.pas' {Form1};

{$R *.res}

// 0x00074878
begin
  Application.Initialize;
  Application.Title := 'kdff gui';
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
