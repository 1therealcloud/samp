unit ServerProperties;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Menus;

type
  TfmServerProperties = class(TForm)
    lbAddressLab: TLabel;
    lbPlayersLab: TLabel;
    lbPingLab: TLabel;
    lbModeLab: TLabel;
    lbMapLab: TLabel;
    lbMap: TLabel;
    lbMode: TLabel;
    lbPing: TLabel;
    lbPlayers: TLabel;
    edAddress: TEdit;
    lbHostName: TLabel;
    lbServerPassword: TLabel;
    lbRconPassword: TLabel;
    edServerPassword: TEdit;
    edRconPassword: TEdit;
    bnSave: TButton;
    bnCancel: TButton;
    bnConnect: TButton;
    pmCopy: TPopupMenu;
    piCopy: TMenuItem;
    procedure bnSaveClick(Sender: TObject);
    procedure bnCancelClick(Sender: TObject);
    procedure bnConnectClick(Sender: TObject);
    procedure pmCopyPopup(Sender: TObject);
    procedure piCopyClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmServerProperties: TfmServerProperties;

implementation

uses Main;

{$R *.dfm}

// 0x000cc880
procedure TfmServerProperties.bnSaveClick(Sender: TObject);
var
  Idx: Integer;
begin
  Idx:= StrToInt(fmMain.lbServers.Items.Strings[fmMain.lbServers.ItemIndex]);
  Servers[Idx].ServerPassword:= edServerPassword.Text;
  Servers[Idx].RconPassword:= edRconPassword.Text;
  Close;
end;

// 0x000cc97c
procedure TfmServerProperties.bnCancelClick(Sender: TObject);
begin
  Close;
end;

// 0x000cc994
procedure TfmServerProperties.bnConnectClick(Sender: TObject);
var
  Idx: Integer;
begin
  Idx:= StrToInt(fmMain.lbServers.Items.Strings[fmMain.lbServers.ItemIndex]);
  Servers[Idx].ServerPassword:= edServerPassword.Text;
  Servers[Idx].RconPassword:= edRconPassword.Text;
  fmMain.ConnectClick(fmMain);
  Close;
end;

// 0x000ccaa4
procedure TfmServerProperties.pmCopyPopup(Sender: TObject);
begin
  piCopy.Enabled:= edAddress.Text <> '- - -';
end;

// 0x000ccb24
procedure TfmServerProperties.piCopyClick(Sender: TObject);
begin
  fmMain.SetClipBoardStr(edAddress.Text);
end;

end.
