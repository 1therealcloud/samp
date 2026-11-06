//--------------------------------------------------------------------------------------
// File: DXUTgui.h
//
// Desc: 
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef DXUT_GUI_H
#define DXUT_GUI_H

#include <usp10.h>
#include <dimm.h>


//--------------------------------------------------------------------------------------
// Defines and macros 
//--------------------------------------------------------------------------------------
#define EVENT_BUTTON_CLICKED                0x0101
#define EVENT_COMBOBOX_SELECTION_CHANGED    0x0201
#define EVENT_RADIOBUTTON_CHANGED           0x0301
#define EVENT_CHECKBOX_CHANGED              0x0401
#define EVENT_SLIDER_VALUE_CHANGED          0x0501
#define EVENT_EDITBOX_STRING                0x0601
// EVENT_EDITBOX_CHANGE is sent when the listbox content changes
// due to user input.
#define EVENT_EDITBOX_CHANGE                0x0602
#define EVENT_LISTBOX_ITEM_DBLCLK           0x0701
// EVENT_LISTBOX_SELECTION is fired off when the selection changes in
// a single selection list box.
#define EVENT_LISTBOX_SELECTION             0x0702


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
class CDXUTDialogResourceManager;
class CDXUTControl;
class CDXUTButton;
class CDXUTStatic;
class CDXUTCheckBox;
class CDXUTRadioButton;
class CDXUTComboBox;
class CDXUTSlider;
class CDXUTEditBox;
class CDXUTIMEEditBox;
class CDXUTListBox;
class CDXUTScrollBar;
class CDXUTElement;
struct DXUTElementHolder;
struct DXUTTextureNode;
struct DXUTFontNode;
typedef VOID (CALLBACK *PCALLBACKDXUTGUIEVENT) ( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
// 0x000c6af0
void ClientRefreshGuiFonts();


//--------------------------------------------------------------------------------------
// Enums for pre-defined control types
//--------------------------------------------------------------------------------------
enum DXUT_CONTROL_TYPE 
{ 
    DXUT_CONTROL_BUTTON, 
    DXUT_CONTROL_STATIC, 
    DXUT_CONTROL_CHECKBOX,
    DXUT_CONTROL_RADIOBUTTON,
    DXUT_CONTROL_COMBOBOX,
    DXUT_CONTROL_SLIDER,
    DXUT_CONTROL_EDITBOX,
    DXUT_CONTROL_IMEEDITBOX,
    DXUT_CONTROL_LISTBOX,
    DXUT_CONTROL_SCROLLBAR,
};

enum DXUT_CONTROL_STATE
{
    DXUT_STATE_NORMAL = 0,
    DXUT_STATE_DISABLED,
    DXUT_STATE_HIDDEN,
    DXUT_STATE_FOCUS,
    DXUT_STATE_MOUSEOVER,
    DXUT_STATE_PRESSED,
};

#define MAX_CONTROL_STATES 6

struct DXUTBlendColor
{
    // 0x00085E90
    void Init( D3DCOLOR defaultColor, D3DCOLOR disabledColor = D3DCOLOR_ARGB(200, 128, 128, 128), D3DCOLOR hiddenColor = 0 );
    // 0x00085F40
    void Blend( UINT iState, float fElapsedTime, float fRate = 0.7f );
    
    D3DCOLOR  States[ MAX_CONTROL_STATES ]; // Modulate colors for all possible control states
    D3DXCOLOR Current;
};


//-----------------------------------------------------------------------------
// Contains all the display tweakables for a sub-control
//-----------------------------------------------------------------------------
class CDXUTElement
{
public:
    // 0x00086010
    void SetTexture( UINT iTexture, RECT* prcTexture, D3DCOLOR defaultTextureColor = D3DCOLOR_ARGB(255, 255, 255, 255) );
    // 0x00086060
    void SetFont( UINT iFont, D3DCOLOR defaultFontColor = D3DCOLOR_ARGB(255, 255, 255, 255), DWORD dwTextFormat = DT_CENTER | DT_VCENTER );

    // 0x00086090
    
    void Refresh();
    
    UINT iTexture;          // Index of the texture for this Element 
    UINT iFont;             // Index of the font for this Element
    DWORD dwTextFormat;     // The format argument to DrawText 

    RECT rcTexture;         // Bounding rect of this element on the composite texture
    
    DXUTBlendColor TextureColor;
    DXUTBlendColor FontColor;
};


//-----------------------------------------------------------------------------
// All controls must be assigned to a dialog, which handles
// input and rendering for the controls.
//-----------------------------------------------------------------------------
class CDXUTDialog
{
public:
    CDXUTDialog();
    ~CDXUTDialog();

    // Need to call this now
    void Init( CDXUTDialogResourceManager* pManager );

    // Windows message handler
    bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // Control creation
    HRESULT AddStatic( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bIsDefault=false, CDXUTStatic** ppCreated=NULL );
    HRESULT AddButton( int ID, LPCWSTR strText, int x, int y, int width, int height, UINT nHotkey=0, bool bIsDefault=false, CDXUTButton** ppCreated=NULL );
    HRESULT AddCheckBox( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bChecked=false, UINT nHotkey=0, bool bIsDefault=false, CDXUTCheckBox** ppCreated=NULL );
    HRESULT AddRadioButton( int ID, UINT nButtonGroup, LPCWSTR strText, int x, int y, int width, int height, bool bChecked=false, UINT nHotkey=0, bool bIsDefault=false, CDXUTRadioButton** ppCreated=NULL );
    HRESULT AddComboBox( int ID, int x, int y, int width, int height, UINT nHotKey=0, bool bIsDefault=false, CDXUTComboBox** ppCreated=NULL );
    HRESULT AddSlider( int ID, int x, int y, int width, int height, int min=0, int max=100, int value=50, bool bIsDefault=false, CDXUTSlider** ppCreated=NULL );
    HRESULT AddEditBox( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bIsDefault=false, CDXUTEditBox** ppCreated=NULL );
    HRESULT AddIMEEditBox( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bIsDefault=false, CDXUTIMEEditBox** ppCreated=NULL );
    // 0x00090b30
    HRESULT AddIMEEditBox(int ID, const char* strText, int x, int y, int width, int height, bool bIsDefault = false, CDXUTIMEEditBox** ppCreated = NULL);
    HRESULT AddListBox( int ID, int x, int y, int width, int height, DWORD dwStyle=0, CDXUTListBox** ppCreated=NULL );
    HRESULT AddControl( CDXUTControl* pControl );
    HRESULT InitControl( CDXUTControl* pControl );

    // Control retrieval
    CDXUTStatic*      GetStatic( int ID ) { return (CDXUTStatic*) GetControl( ID, DXUT_CONTROL_STATIC ); }
    CDXUTButton*      GetButton( int ID ) { return (CDXUTButton*) GetControl( ID, DXUT_CONTROL_BUTTON ); }
    CDXUTCheckBox*    GetCheckBox( int ID ) { return (CDXUTCheckBox*) GetControl( ID, DXUT_CONTROL_CHECKBOX ); }
    CDXUTRadioButton* GetRadioButton( int ID ) { return (CDXUTRadioButton*) GetControl( ID, DXUT_CONTROL_RADIOBUTTON ); }
    CDXUTComboBox*    GetComboBox( int ID ) { return (CDXUTComboBox*) GetControl( ID, DXUT_CONTROL_COMBOBOX ); }
    // 0x000710b0
    CDXUTSlider*      GetSlider( int ID ) { return (CDXUTSlider*) GetControl( ID, DXUT_CONTROL_SLIDER ); }
    CDXUTEditBox*     GetEditBox( int ID ) { return (CDXUTEditBox*) GetControl( ID, DXUT_CONTROL_EDITBOX ); }
    CDXUTIMEEditBox*  GetIMEEditBox( int ID ) { return (CDXUTIMEEditBox*) GetControl( ID, DXUT_CONTROL_IMEEDITBOX ); }
    CDXUTListBox*     GetListBox( int ID ) { return (CDXUTListBox*) GetControl( ID, DXUT_CONTROL_LISTBOX ); }

    CDXUTControl* GetControl( int ID );
    CDXUTControl* GetControl( int ID, UINT nControlType );
    CDXUTControl* GetControlAtPoint( POINT pt );

    bool GetControlEnabled( int ID );
    void SetControlEnabled( int ID, bool bEnabled );

    void ClearRadioButtonGroup( UINT nGroup );
    void ClearComboBox( int ID );

    // Access the default display Elements used when adding new controls
    HRESULT       SetDefaultElement( UINT nControlType, UINT iElement, CDXUTElement* pElement );
    CDXUTElement* GetDefaultElement( UINT nControlType, UINT iElement );

    // Methods called by controls
    void SendEvent( UINT nEvent, bool bTriggeredByUser, CDXUTControl* pControl );
    void RequestFocus( CDXUTControl* pControl );

    // Render helpers
    HRESULT DrawRect( RECT* pRect, D3DCOLOR color );
    HRESULT DrawPolyLine( POINT* apPoints, UINT nNumPoints, D3DCOLOR color );
    HRESULT DrawSprite( CDXUTElement* pElement, RECT* prcDest );
    HRESULT CalcTextRect( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, int nCount = -1 );
    HRESULT DrawText( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, bool bShadow = false, int nCount = -1 );
    // 0x0008cae0
    HRESULT DrawTextA(const char* text, CDXUTElement* element, RECT* rect, bool shadow = false, int count = -1);

    // Attributes
    // 0x00060d10
    bool GetVisible() { return m_bVisible; }
    // 0x00002cb0
    void SetVisible( bool bVisible ) { m_bVisible = bVisible; }
    bool GetMinimized() { return m_bMinimized; }
    void SetMinimized( bool bMinimized ) { m_bMinimized = bMinimized; }
    void SetBackgroundColors( D3DCOLOR colorAllCorners ) { SetBackgroundColors( colorAllCorners, colorAllCorners, colorAllCorners, colorAllCorners ); }
    void SetBackgroundColors( D3DCOLOR colorTopLeft, D3DCOLOR colorTopRight, D3DCOLOR colorBottomLeft, D3DCOLOR colorBottomRight );
    // 0x0006a100
    void EnableCaption( bool bEnable ) { m_bCaption = bEnable; }
    // 0x0006a110
    int GetCaptionHeight() const { return m_nCaptionHeight; }
    // 0x0006a120
    void SetCaptionHeight( int nHeight ) { m_nCaptionHeight = nHeight; }
    // samp.dll RVA: unknown (SDK wide compatibility facade; no standalone native entry)
    void SetCaptionText( const WCHAR * text ) {
        m_wszCaption[0] = 0;
        WideCharToMultiByte(CP_ACP, 0, text, -1, m_wszCaption, 256, 0, 0);
        m_wszCaption[255] = 0;
    }
    // 0x0006a130
    void SetCaptionText(const char* text);
    void GetLocation( POINT &Pt ) const { Pt.x = m_x; Pt.y = m_y; }
    // 0x00002cc0
    void SetLocation( int x, int y ) { m_x = x; m_y = y; }
    void SetSize( int width, int height ) { m_width = width; m_height = height;  }
    // 0x00002ce0
    int GetWidth() { return m_width; }
    // 0x00002cf0
    int GetHeight() { return m_height; }

    void SetNextDialog( CDXUTDialog* pNextDialog );

    static void SetRefreshTime( float fTime ){ s_fTimeRefresh = fTime; }

    static CDXUTControl* GetNextControl( CDXUTControl* pControl );
    static CDXUTControl* GetPrevControl( CDXUTControl* pControl );

    void RemoveControl( int ID );
    void RemoveAllControls();

    // Sets the callback used to notify the app of control events
    void SetCallback( PCALLBACKDXUTGUIEVENT pCallback, void* pUserContext = NULL );
    void EnableNonUserEvents( bool bEnable ) { m_bNonUserEvents = bEnable; }
    // 0x0006a150
    void EnableKeyboardInput( bool bEnable ) { m_bKeyboardInput = bEnable; }
    // 0x0006a160
    void EnableMouseInput( bool bEnable ) { m_bMouseInput = bEnable; }

    // Device state notification
    void Refresh();
    // 0x00090360
    HRESULT OnRender( float fElapsedTime );    

    // Shared resource access. Indexed fonts and textures are shared among
    // all the controls.
    HRESULT       SetFont( UINT index, LPCWSTR strFaceName, LONG height, LONG weight );
    // 0x0008ca50
    HRESULT SetFont(UINT index, const char* face, LONG height, LONG weight);
    // 0x00086B40
    DXUTFontNode* GetFont( UINT index );

    HRESULT          SetTexture( UINT index, LPCWSTR strFilename );
    // 0x00089890
    HRESULT SetTexture(UINT index, const char* filename);
    DXUTTextureNode* GetTexture( UINT index );

    CDXUTDialogResourceManager* GetManager() { return m_pManager; }

    static void ClearFocus();
    void FocusDefaultControl();

    bool m_bNonUserEvents;
    bool m_bKeyboardInput;
    bool m_bMouseInput;

private:
    int m_nDefaultControlID;

    static double s_fTimeRefresh;
    double m_fTimeLastRefresh;

    // Initialize default Elements
    // 0x0008d1c0
    void InitFont();
    // 0x000c6af0
    friend void ClientRefreshGuiFonts();
    // 0x0008d200
    void InitDefaultElements();

    // Windows message handlers
    void OnMouseMove( POINT pt );
    void OnMouseUp( POINT pt );

    // Control events
    bool OnCycleFocus( bool bForward );

    static CDXUTControl* s_pControlFocus;        // The control which has focus
    static CDXUTControl* s_pControlPressed;      // The control currently pressed

    CDXUTControl* m_pControlMouseOver;           // The control which is hovered over

    bool m_bVisible;
    bool m_bCaption;
    bool m_bMinimized;
    char m_wszCaption[256]; // Native ANSI caption at +0x16.

    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_nCaptionHeight;

    D3DCOLOR m_colorTopLeft;
    D3DCOLOR m_colorTopRight;
    D3DCOLOR m_colorBottomLeft;
    D3DCOLOR m_colorBottomRight;

    CDXUTDialogResourceManager* m_pManager;
    PCALLBACKDXUTGUIEVENT m_pCallbackEvent;
    void* m_pCallbackEventUserContext;

    friend class NativeTextureProbe;

    CGrowableArray< int > m_Textures;   // Index into m_TextureCache;
    friend class NativeDialogProbe;
    friend class NativeFontManagerProbe;
    CGrowableArray< int > m_Fonts;      // Index into m_FontCache;

    CGrowableArray< CDXUTControl* > m_Controls;
    friend class NativeThemeProbe;
    CGrowableArray< DXUTElementHolder* > m_DefaultElements;

    CDXUTElement m_CapElement;  // Element for the caption

    CDXUTDialog* m_pNextDialog;
    CDXUTDialog* m_pPrevDialog;
};


//--------------------------------------------------------------------------------------
// Structs for shared resources
//--------------------------------------------------------------------------------------
struct DXUTTextureNode
{
    char strFilename[MAX_PATH]; // Native texture node +0x104 pointer.
    IDirect3DTexture9* pTexture;
    DWORD dwWidth;
    DWORD dwHeight;
};

struct DXUTFontNode
{
    char strFace[MAX_PATH]; // Native font node is ANSI even with UTF-16 edit buffers.
    ID3DXFont* pFont;
    LONG  nHeight;
    LONG  nWeight;
};


//-----------------------------------------------------------------------------
// Manages shared resources of dialogs
//-----------------------------------------------------------------------------
class CDXUTDialogResourceManager
{
public:
    CDXUTDialogResourceManager();
    ~CDXUTDialogResourceManager();

    HRESULT     OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice );
    HRESULT     OnResetDevice();
    void        OnLostDevice();
    void        OnDestroyDevice();
    bool        MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    
    int AddFont( LPCWSTR strFaceName, LONG height, LONG weight );
    // 0x0008c8f0
    int AddFont(const char* face, LONG height, LONG weight);
    // Descriptive name; the original lexical name is unknown.
    // 0x0008c9e0
    int UpdateFont(UINT index, const char* face, LONG height, LONG weight);
    friend class NativeFontManagerProbe;
    friend class NativeDialogProbe;
    friend class CDXUTDialog;
    int AddTexture( LPCWSTR strFilename );
    // 0x000897d0
    int AddTexture(const char* filename);
    friend class NativeTextureProbe;

    DXUTFontNode*     GetFontNode( int iIndex )     { return m_FontCache.GetAt( iIndex ); };
    DXUTTextureNode*  GetTextureNode( int iIndex )  { return m_TextureCache.GetAt( iIndex ); };
    IDirect3DDevice9* GetD3DDevice()                { return m_pd3dDevice; }

    // Shared between all dialogs
    IDirect3DStateBlock9* m_pStateBlock;
    ID3DXSprite*          m_pSprite;          // Sprite used for drawing

protected:
    CGrowableArray< DXUTTextureNode* > m_TextureCache;   // Shared textures
    CGrowableArray< DXUTFontNode* > m_FontCache;         // Shared fonts
    
    IDirect3DDevice9* m_pd3dDevice;

    // Resource creation helpers
    HRESULT CreateFont( UINT index );
    // 0x000874b0
    HRESULT CreateTexture( UINT index );
};


//-----------------------------------------------------------------------------
// Base class for controls
//-----------------------------------------------------------------------------
class CDXUTControl
{
public:
    // 0x0008DCC0
    CDXUTControl( CDXUTDialog *pDialog = NULL );
    // 0x0008DE00
    virtual ~CDXUTControl();

    // 0x0008DD10

    virtual HRESULT OnInit() { return S_OK; }
    // 0x00087560
    virtual void Refresh();
    // 0x0008DD20
    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime ) { };

    // Windows message handler
    // 0x0008DD30
    virtual bool MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) { return false; }

    // 0x0008DD40

    virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ) { return false; }
    // 0x0008DD50
    virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam ) { return false; }

    // 0x0008DD60

    virtual bool CanHaveFocus() { return false; }
    // 0x000840D0
    virtual void OnFocusIn() { m_bHasFocus = true; }
    // 0x000840E0
    virtual void OnFocusOut() { m_bHasFocus = false; }
    // 0x0008DD70
    virtual void OnMouseEnter() { m_bMouseOver = true; }
    // 0x0008DD80
    virtual void OnMouseLeave() { m_bMouseOver = false; }
    // 0x0008DD90
    virtual void OnHotkey() {}

    // 0x0008DDA0

    virtual BOOL ContainsPoint( POINT pt ) { return PtInRect( &m_rcBoundingBox, pt ); }

    // 0x0008DDC0

    virtual void SetEnabled( bool bEnabled ) { m_bEnabled = bEnabled; }
    // 0x0008DDD0
    virtual bool GetEnabled() { return m_bEnabled; }
    // 0x0008DDE0
    virtual void SetVisible( bool bVisible ) { m_bVisible = bVisible; }
    // 0x0008DDF0
    virtual bool GetVisible() { return m_bVisible; }

    // samp.dll RVA: unknown (inline field accessor; standalone DL type binding not established)

    UINT GetType() const { return m_Type; }

    // samp.dll RVA: unknown (inline field accessor; standalone DL type binding not established)

    int  GetID() const { return m_ID; }
    // samp.dll RVA: unknown (identical entries 0x00049ED0/0x00084110; typed native entry not uniquely established)
    void SetID( int ID ) { m_ID = ID; }

    // 0x000848A0
    // 0x00066B00

    void SetLocation( int x, int y ) { m_x = x; m_y = y; UpdateRects(); }
    // 0x00066B20
    void SetSize( int width, int height ) { m_width = width; m_height = height; UpdateRects(); }

    // 0x00084120

    void SetHotkey( UINT nHotkey ) { m_nHotkey = nHotkey; }
    // samp.dll RVA: unknown (inline field accessor; standalone DL type binding not established)
    UINT GetHotkey() { return m_nHotkey; }

    // samp.dll RVA: unknown (inline SDK accessor; standalone native entry not established)

    void SetUserData( void *pUserData ) { m_pUserData = pUserData; }
    // samp.dll RVA: unknown (inline SDK accessor; standalone native entry not established)
    void *GetUserData() const { return m_pUserData; }

    // 0x00087540

    virtual void SetTextColor( D3DCOLOR Color );
    // 0x00069270
    CDXUTElement* GetElement( UINT iElement ) { return m_Elements.GetAt( iElement ); }
    // 0x00089D50
    HRESULT SetElement( UINT iElement, CDXUTElement* pElement);

    bool m_bVisible;                // Shown/hidden flag
    bool m_bMouseOver;              // Mouse pointer is above control
    bool m_bHasFocus;               // Control has input focus
    bool m_bIsDefault;              // Is the default control

    // Size, scale, and positioning members
    int m_x, m_y;
    int m_width, m_height;

    // These members are set by the container
    CDXUTDialog* m_pDialog;    // Parent container
    UINT m_Index;              // Index within the control list
    
    CGrowableArray< CDXUTElement* > m_Elements;  // All display elements

protected:
    virtual void UpdateRects();

    int  m_ID;                 // ID number
    DXUT_CONTROL_TYPE m_Type;  // Control type, set once in constructor  
    UINT m_nHotkey;            // Virtual key code for this control's hotkey
    void *m_pUserData;         // Data associated with this control that is set by user.
    
    bool m_bEnabled;           // Enabled/disabled flag
    
    RECT m_rcBoundingBox;      // Rectangle defining the active region of the control
};


//-----------------------------------------------------------------------------
// Contains all the display information for a given control type
//-----------------------------------------------------------------------------
struct DXUTElementHolder
{
    UINT nControlType;
    UINT iElement;

    CDXUTElement Element;
};


//-----------------------------------------------------------------------------
// Static control
//-----------------------------------------------------------------------------
class CDXUTStatic : public CDXUTControl
{
public:
    CDXUTStatic( CDXUTDialog *pDialog = NULL );

    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );
    virtual BOOL ContainsPoint( POINT pt ) { return false; }

    HRESULT GetTextCopy( LPWSTR strDest, UINT bufferCount );
    LPCWSTR GetText() { return m_strText; }
    HRESULT SetText( LPCWSTR strText );


protected:
    WCHAR m_strText[MAX_PATH];      // Window text  
};


//-----------------------------------------------------------------------------
// Button control
//-----------------------------------------------------------------------------
class CDXUTButton : public CDXUTStatic
{
public:
    CDXUTButton( CDXUTDialog *pDialog = NULL );

    virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
    virtual void OnHotkey() { m_pDialog->RequestFocus( this ); m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this ); }
    
    virtual BOOL ContainsPoint( POINT pt ) { return PtInRect( &m_rcBoundingBox, pt ); }
    virtual bool CanHaveFocus() { return (m_bVisible && m_bEnabled); }

    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );

protected:
    bool m_bPressed;
};


//-----------------------------------------------------------------------------
// CheckBox control
//-----------------------------------------------------------------------------
class CDXUTCheckBox : public CDXUTButton
{
public:
    CDXUTCheckBox( CDXUTDialog *pDialog = NULL );

    virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
    virtual void OnHotkey() { m_pDialog->RequestFocus( this ); SetCheckedInternal( !m_bChecked, true ); }

    virtual BOOL ContainsPoint( POINT pt ); 
    virtual void UpdateRects(); 

    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );

    bool GetChecked() { return m_bChecked; }
    void SetChecked( bool bChecked ) { SetCheckedInternal( bChecked, false ); }
    
protected:
    virtual void SetCheckedInternal( bool bChecked, bool bFromInput );

    bool m_bChecked;
    RECT m_rcButton;
    RECT m_rcText;
};


//-----------------------------------------------------------------------------
// RadioButton control
//-----------------------------------------------------------------------------
class CDXUTRadioButton : public CDXUTCheckBox
{
public:
    CDXUTRadioButton( CDXUTDialog *pDialog = NULL );

    virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
    virtual void OnHotkey() { m_pDialog->RequestFocus( this ); SetCheckedInternal( true, true, true ); }
    
    void SetChecked( bool bChecked, bool bClearGroup=true ) { SetCheckedInternal( bChecked, bClearGroup, false ); }
    void SetButtonGroup( UINT nButtonGroup ) { m_nButtonGroup = nButtonGroup; }
    UINT GetButtonGroup() { return m_nButtonGroup; }
    
protected:
    virtual void SetCheckedInternal( bool bChecked, bool bClearGroup, bool bFromInput );
    UINT m_nButtonGroup;
};


//-----------------------------------------------------------------------------
// Scrollbar control
//-----------------------------------------------------------------------------
class CDXUTScrollBar : public CDXUTControl
{
    friend class CDXUTListBoxDL;
public:
    // 0x0008E750
    CDXUTScrollBar( CDXUTDialog *pDialog = NULL );
    // 0x0008E800
    virtual ~CDXUTScrollBar();

    // 0x00084E70

    virtual bool    HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
    // 0x00088250
    virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );

    // 0x000884D0

    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );
    // 0x000881B0
    virtual void    UpdateRects();

    // 0x000886D0

    void SetTrackRange( int nStart, int nEnd );
    // 0x00066B40
    int GetTrackPos() { return m_nPosition; }
    // 0x00066B50
    // 0x00084DA0
    // 0x00084E80
    void SetTrackPos( int nPosition ) { m_nPosition = nPosition; Cap(); UpdateThumbRect(); }
    // 0x00084190
    int GetPageSize() { return m_nPageSize; }
    // 0x00066B70
    void SetPageSize( int nPageSize ) { m_nPageSize = nPageSize; Cap(); UpdateThumbRect(); }

    // 0x00088230

    void Scroll( int nDelta );    // Scroll by nDelta items (plus or minus)
    // 0x00084E30
    void ShowItem( int nIndex );  // Ensure that item nIndex is displayed, scroll if necessary

protected:
    // ARROWSTATE indicates the state of the arrow buttons.
    // CLEAR            No arrow is down.
    // CLICKED_UP       Up arrow is clicked.
    // CLICKED_DOWN     Down arrow is clicked.
    // HELD_UP          Up arrow is held down for sustained period.
    // HELD_DOWN        Down arrow is held down for sustained period.
    enum ARROWSTATE { CLEAR, CLICKED_UP, CLICKED_DOWN, HELD_UP, HELD_DOWN };

    void UpdateThumbRect();
    void Cap();  // Clips position at boundaries. Ensures it stays within legal range.

    bool m_bShowThumb;
    RECT m_rcUpButton;
    RECT m_rcDownButton;
    RECT m_rcTrack;
    RECT m_rcThumb;
    int m_nPosition;  // Position of the first displayed item
    int m_nPageSize;  // How many items are displayable in one page
    int m_nStart;     // First item
    int m_nEnd;       // The index after the last item
    POINT m_LastMouse;// Last mouse position
    ARROWSTATE m_Arrow; // State of the arrows
    double m_dArrowTS;  // Timestamp of last arrow event.
};


//-----------------------------------------------------------------------------
// ListBox control
//-----------------------------------------------------------------------------
struct DXUTListBoxItem
{
    WCHAR strText[256];
    void*  pData;

    RECT  rcActive;
    bool  bSelected;
};

class CDXUTListBox : public CDXUTControl
{
public:
    CDXUTListBox( CDXUTDialog *pDialog = NULL );
    virtual ~CDXUTListBox();

    virtual HRESULT OnInit() { return m_pDialog->InitControl( &m_ScrollBar ); }
    virtual bool    CanHaveFocus() { return (m_bVisible && m_bEnabled); }
    virtual bool    HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );

    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );
    virtual void    UpdateRects();

    DWORD GetStyle() const { return m_dwStyle; }
    int GetSize() const { return m_Items.GetSize(); }
    void SetStyle( DWORD dwStyle ) { m_dwStyle = dwStyle; }
    void SetScrollBarWidth( int nWidth ) { m_nSBWidth = nWidth; UpdateRects(); }
    void SetBorder( int nBorder, int nMargin ) { m_nBorder = nBorder; m_nMargin = nMargin; }
    HRESULT AddItem( const WCHAR *wszText, void *pData );
    HRESULT InsertItem( int nIndex, const WCHAR *wszText, void *pData );
    void RemoveItem( int nIndex );
    void RemoveItemByText( WCHAR *wszText );
    void RemoveItemByData( void *pData );
    void RemoveAllItems();

    DXUTListBoxItem *GetItem( int nIndex );
    int GetSelectedIndex( int nPreviousSelected = -1 );
    DXUTListBoxItem *GetSelectedItem( int nPreviousSelected = -1 ) { return GetItem( GetSelectedIndex( nPreviousSelected ) ); }
    void SelectItem( int nNewIndex );

    enum STYLE { MULTISELECTION = 1 };

protected:
    RECT m_rcText;      // Text rendering bound
    RECT m_rcSelection; // Selection box bound
    CDXUTScrollBar m_ScrollBar;
    int m_nSBWidth;
    int m_nBorder;
    int m_nMargin;
    int m_nTextHeight;  // Height of a single line of text
    DWORD m_dwStyle;    // List box style
    int m_nSelected;    // Index of the selected item for single selection list box
    int m_nSelStart;    // Index of the item where selection starts (for handling multi-selection)
    bool m_bDrag;       // Whether the user is dragging the mouse to select

    CGrowableArray< DXUTListBoxItem* > m_Items;
};


//-----------------------------------------------------------------------------
// ComboBox control
//-----------------------------------------------------------------------------
struct DXUTComboBoxItem
{
    WCHAR strText[256];
    void*  pData;

    RECT  rcActive;
    bool  bVisible;
};


class CDXUTComboBox : public CDXUTButton
{
public:
    CDXUTComboBox( CDXUTDialog *pDialog = NULL );
    virtual ~CDXUTComboBox();
    
    virtual void SetTextColor( D3DCOLOR Color );
    virtual HRESULT OnInit() { return m_pDialog->InitControl( &m_ScrollBar ); }

    virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
    virtual void OnHotkey();

    virtual bool CanHaveFocus() { return (m_bVisible && m_bEnabled); }
    virtual void OnFocusOut();
    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );

    virtual void UpdateRects(); 

    HRESULT AddItem( const WCHAR* strText, void* pData );
    void    RemoveAllItems();
    void    RemoveItem( UINT index );
    bool    ContainsItem( const WCHAR* strText, UINT iStart=0 );
    int     FindItem( const WCHAR* strText, UINT iStart=0 );
    void*   GetItemData( const WCHAR* strText );
    void*   GetItemData( int nIndex );
    void    SetDropHeight( UINT nHeight ) { m_nDropHeight = nHeight; UpdateRects(); }
    void    SetScrollBarWidth( int nWidth ) { m_nSBWidth = nWidth; UpdateRects(); }

    void*   GetSelectedData();
    DXUTComboBoxItem* GetSelectedItem();

    UINT    GetNumItems() { return m_Items.GetSize(); }
    DXUTComboBoxItem* GetItem( UINT index ) { return m_Items.GetAt( index ); }

    HRESULT SetSelectedByIndex( UINT index );
    HRESULT SetSelectedByText( const WCHAR* strText );
    HRESULT SetSelectedByData( void* pData );  

protected:
    int     m_iSelected;
    int     m_iFocused;
    int     m_nDropHeight;
    CDXUTScrollBar m_ScrollBar;
    int     m_nSBWidth;

    bool    m_bOpened;

    RECT m_rcText;
    RECT m_rcButton;
    RECT m_rcDropdown;
    RECT m_rcDropdownText;

    
    CGrowableArray< DXUTComboBoxItem* > m_Items;
};


//-----------------------------------------------------------------------------
// Slider control
//-----------------------------------------------------------------------------
class CDXUTSlider : public CDXUTControl
{
public:
    CDXUTSlider( CDXUTDialog *pDialog = NULL );

    virtual BOOL ContainsPoint( POINT pt );
    // 0x0008e730 
    virtual bool CanHaveFocus() { return (m_bVisible && m_bEnabled); }
    // 0x00087e80
    virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
    
    virtual void UpdateRects(); 

    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );

    void SetValue( int nValue ) { SetValueInternal( nValue, false ); }
    int  GetValue() const { return m_nValue; };

    void GetRange( int &nMin, int &nMax ) const { nMin = m_nMin; nMax = m_nMax; }
    void SetRange( int nMin, int nMax );

protected:
    void SetValueInternal( int nValue, bool bFromInput );
    int  ValueFromPos( int x ); 

    int m_nValue;

    int m_nMin;
    int m_nMax;

    int m_nDragX;      // Mouse position at start of drag
    int m_nDragOffset; // Drag offset from the center of the button
    int m_nButtonX;

    bool m_bPressed;
    RECT m_rcButton;
};


//-----------------------------------------------------------------------------
// CUniBuffer class for the edit control
//-----------------------------------------------------------------------------
class CUniBuffer
{
public:
    // 0x0009bbb0
    CUniBuffer( int nInitialSize = 1 );
    // 0x0009b440
    ~CUniBuffer();

    // 0x0009ba60

    static void Initialize();
    // 0x0009b1d0
    static void Uninitialize();

    // samp.dll RVA: unknown (inline SDK accessor; native standalone entry not established)

    int  GetBufferSize() { return m_nBufferSize; }
    // 0x0009b240
    bool SetBufferSize( int nSize );
    // samp.dll RVA: unknown (inline SDK accessor; native standalone entry not established)
    int  GetTextSize()  { return lstrlenW( m_pwszBuffer ); }
    // samp.dll RVA: unknown (inline SDK accessor; native standalone entry not established)
    const WCHAR* GetBuffer() { return m_pwszBuffer; }
    // samp.dll RVA: unknown (inline SDK accessor; native standalone entry not established)
    const WCHAR& operator[]( int n ) const { return m_pwszBuffer[n]; }
    // 0x0009b470
    WCHAR& operator[]( int n );
    // samp.dll RVA: unknown (inline SDK accessor; native standalone entry not established)
    DXUTFontNode* GetFontNode() { return m_pFontNode; }
    // samp.dll RVA: unknown (inline SDK accessor; native standalone entry not established)
    void SetFontNode( DXUTFontNode *pFontNode ) { m_pFontNode = pFontNode; }
    // 0x0009b480
    void Clear();

    // 0x0009b490

    bool InsertChar( int nIndex, WCHAR wChar ); // Inserts the char at specified index. If nIndex == -1, insert to the end.
    // Native ANSI/DBCS path: only the first lead byte returns false.
    // 0x0009b520
    bool InsertChar(int nIndex, char ch);
    // Preserves native whole-object assignment in the single-byte path.
    // 0x0009bbf0
    bool OverwriteAnsiChar(int nIndex, char ch);
    // 0x0009b5d0
    bool RemoveChar( int nIndex );  // Removes the char at specified index. If nIndex == -1, remove the last char.
    // 0x0009b630
    bool InsertString( int nIndex, const WCHAR *pStr, int nCount = -1 );  // Inserts the first nCount characters of the string pStr at specified index.  If nCount == -1, the entire string is inserted. If nIndex == -1, insert to the end.
    // 0x0009b9a0
    bool SetText( LPCWSTR wszText );
    // 0x0009ba00
    bool SetText(const char* text);

    // Uniscribe
    // 0x0009b770
    HRESULT CPtoX( int nCP, BOOL bTrail, int *pX );
    // 0x0009b7b0
    HRESULT XtoCP( int nX, int *pCP, int *pnTrail );
    // 0x0009b840
    void GetPriorItemPos( int nCP, int *pPrior );
    // 0x0009b8d0
    void GetNextItemPos( int nCP, int *pPrior );

private:
    // 0x0009b310
    HRESULT Analyse();      // Uniscribe -- Analyse() analyses the string in the buffer

    WCHAR* m_pwszBuffer;    // Buffer to hold text
    int    m_nBufferSize;   // Size of the buffer allocated, in characters

    // Uniscribe-specific
    BYTE m_pendingLeadByte;    // Native +8 caches the first ANSI/DBCS byte, not a bool.
    DXUTFontNode* m_pFontNode;          // Font node for the font that this buffer uses
    bool m_bAnalyseRequired;            // True if the string has changed since last analysis.
    SCRIPT_STRING_ANALYSIS m_Analysis;
    bool m_nativeMaskText; // Native field +18; selects masked analysis text.
    friend class CDXUTEditBox;
    friend class NativeDxutLayoutProbe;  // Analysis for the current string

private:
    // Empty implementation of the Uniscribe API
    // 0x000841e0
    static HRESULT WINAPI Dummy_ScriptApplyDigitSubstitution( const SCRIPT_DIGITSUBSTITUTE*, SCRIPT_CONTROL*, SCRIPT_STATE* ) { return E_NOTIMPL; }
    // 0x000841f0
    static HRESULT WINAPI Dummy_ScriptStringAnalyse( HDC, const void *, int, int, int, DWORD, int, SCRIPT_CONTROL*, SCRIPT_STATE*, const int*, SCRIPT_TABDEF*, const BYTE*, SCRIPT_STRING_ANALYSIS* ) { return E_NOTIMPL; }
    // 0x00084200
    static HRESULT WINAPI Dummy_ScriptStringCPtoX( SCRIPT_STRING_ANALYSIS, int, BOOL, int* ) { return E_NOTIMPL; }
    // 0x00084210
    static HRESULT WINAPI Dummy_ScriptStringXtoCP( SCRIPT_STRING_ANALYSIS, int, int*, int* ) { return E_NOTIMPL; }
    // 0x00084220
    static HRESULT WINAPI Dummy_ScriptStringFree( SCRIPT_STRING_ANALYSIS* ) { return E_NOTIMPL; }
    // 0x00084230
    static const SCRIPT_LOGATTR* WINAPI Dummy_ScriptString_pLogAttr( SCRIPT_STRING_ANALYSIS ) { return NULL; }
    // 0x00084240
    static const int* WINAPI Dummy_ScriptString_pcOutChars( SCRIPT_STRING_ANALYSIS ) { return NULL; }

    // Function pointers
    // 0x00140bfc
    static HRESULT (WINAPI *_ScriptApplyDigitSubstitution)( const SCRIPT_DIGITSUBSTITUTE*, SCRIPT_CONTROL*, SCRIPT_STATE* );
    // 0x00140c00
    static HRESULT (WINAPI *_ScriptStringAnalyse)( HDC, const void *, int, int, int, DWORD, int, SCRIPT_CONTROL*, SCRIPT_STATE*, const int*, SCRIPT_TABDEF*, const BYTE*, SCRIPT_STRING_ANALYSIS* );
    // 0x00140c04
    static HRESULT (WINAPI *_ScriptStringCPtoX)( SCRIPT_STRING_ANALYSIS, int, BOOL, int* );
    // 0x00140c08
    static HRESULT (WINAPI *_ScriptStringXtoCP)( SCRIPT_STRING_ANALYSIS, int, int*, int* );
    // 0x00140c0c
    static HRESULT (WINAPI *_ScriptStringFree)( SCRIPT_STRING_ANALYSIS* );
    // 0x00140c10
    static const SCRIPT_LOGATTR* (WINAPI *_ScriptString_pLogAttr)( SCRIPT_STRING_ANALYSIS );
    // 0x00140c14
    static const int* (WINAPI *_ScriptString_pcOutChars)( SCRIPT_STRING_ANALYSIS );

    // 0x00180618

    static HINSTANCE s_hDll;  // Uniscribe DLL handle

};

typedef char NativeUniBufferExtent[(sizeof(CUniBuffer) == 0x13) ? 1 : -1];

//-----------------------------------------------------------------------------
// EditBox control
//-----------------------------------------------------------------------------
class CDXUTEditBox : public CDXUTControl
{
public:
    // 0x0008ed00
    CDXUTEditBox( CDXUTDialog *pDialog = NULL );
    // 0x0008ee30
    virtual ~CDXUTEditBox();

    // 0x00088970

    virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
    // 0x00088be0
    virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
    // 0x00088da0
    virtual bool MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
    // 0x00085130
    virtual void UpdateRects();
    // 0x0008ee00
    virtual bool CanHaveFocus() { return (m_bVisible && m_bEnabled); }
    // 0x0008ee80
    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );
    // 0x00088d90
    virtual void OnFocusIn();

    // samp.dll RVA: unknown (SDK compatibility method; standalone native entry not established)

    void SetText( LPCWSTR wszText, bool bSelected = false );
    // 0x00085000
    void SetText(const char* text, bool bSelected = false);
    // 0x000850d0
    const char* GetText();
    // Updates both native mask flags, without invalidating cached analysis.
    // 0x0006eeb0
    void SetPasswordMode(bool enabled);
    // samp.dll RVA: unknown (inline SDK accessor; native standalone entry not established)
    int GetTextLength() { return m_Buffer.GetTextSize(); }  // Returns text length in chars excluding NULL.
    // 0x00088950
    HRESULT GetTextCopy( LPWSTR strDest, UINT bufferCount );
    // 0x00084fd0
    void ClearText();
    // 0x0008ee20
    virtual void SetTextColor( D3DCOLOR Color ) { m_TextColor = Color; }  // Text color
    // 0x00068dd0
    void SetSelectedTextColor( D3DCOLOR Color ) { m_SelTextColor = Color; }  // Selected text color
    // 0x00068de0
    void SetSelectedBackColor( D3DCOLOR Color ) { m_SelBkColor = Color; }  // Selected background color
    // 0x00068df0
    void SetCaretColor( D3DCOLOR Color ) { m_CaretColor = Color; }  // Caret color
    // samp.dll RVA: unknown (inline SDK accessor; native standalone entry not established)
    void SetBorderWidth( int nBorder ) { m_nBorder = nBorder; UpdateRects(); }  // Border of the window
    // samp.dll RVA: unknown (inline SDK accessor; native standalone entry not established)
    void SetSpacing( int nSpacing ) { m_nSpacing = nSpacing; UpdateRects(); }
    // 0x00089000
    void ParseFloatArray( float *pNumbers, int nCount );
    // samp.dll RVA: unknown (SDK compatibility method; standalone native entry not established)
    void SetTextFloatArray( const float *pNumbers, int nCount );

protected:
    // 0x00084ef0
    void PlaceCaret( int nCP );
    // 0x000850e0
    void DeleteSelectionText();
    // 0x00085cf0
    void ResetCaretBlink();
    // 0x00085260
    void CopyToClipboard();
    // 0x00085320
    void PasteFromClipboard();

    CUniBuffer m_Buffer;     // Buffer to hold text
    int      m_nBorder;      // Border of the window
    int      m_nSpacing;     // Spacing between the text and the edge of border
    RECT     m_rcText;       // Bounding rectangle for the text
    RECT     m_rcRender[9];  // Convenient rectangles for rendering elements
    double   m_dfBlink;      // Caret blink time in milliseconds
    double   m_dfLastBlink;  // Last timestamp of caret blink
    bool     m_bCaretOn;     // Flag to indicate whether caret is currently visible
    int      m_nCaret;       // Caret position, in characters
    bool     m_bInsertMode;  // If true, control is in insert mode. Else, overwrite mode.
    int      m_nSelStart;    // Starting position of the selection. The caret marks the end.
    int      m_nFirstVisible;// First visible character in the edit control
    bool m_nativeFlag126;    // Native constructor writes false before the colors.
    D3DCOLOR m_TextColor;    // Text color
    D3DCOLOR m_SelTextColor; // Selected text color
    D3DCOLOR m_SelBkColor;   // Selected background color
    D3DCOLOR m_CaretColor;   // Caret color

    // Mouse-specific
    bool m_bMouseDrag;       // True to indicate drag in progress

    // Static
    // 0x0017f694
    static bool s_bHideCaret;   // If true, we don't render the caret.
};


//-----------------------------------------------------------------------------
// IME-enabled EditBox control
//-----------------------------------------------------------------------------
#define MAX_CANDLIST 10
#define MAX_COMPSTRING_SIZE 256

class CDXUTIMEEditBox : public CDXUTEditBox
{
protected:
    // Empty implementation of the IMM32 API
    // 0x00084250
    static INPUTCONTEXT* WINAPI Dummy_ImmLockIMC( HIMC ) { return NULL; }
    // 0x00084260
    static BOOL WINAPI Dummy_ImmUnlockIMC( HIMC ) { return FALSE; }
    // 0x00084270
    static LPVOID WINAPI Dummy_ImmLockIMCC( HIMCC ) { return NULL; }
    // 0x00084280
    static BOOL WINAPI Dummy_ImmUnlockIMCC( HIMCC ) { return FALSE; }
    // 0x00084290
    static BOOL WINAPI Dummy_ImmDisableTextFrameService( DWORD ) { return TRUE; }
    // 0x000842a0
    static LONG WINAPI Dummy_ImmGetCompositionStringW( HIMC, DWORD, LPVOID, DWORD ) { return IMM_ERROR_GENERAL; }
    // 0x000842b0
    static DWORD WINAPI Dummy_ImmGetCandidateListW( HIMC, DWORD, LPCANDIDATELIST, DWORD ) { return 0; }
    // 0x000842c0
    static HIMC WINAPI Dummy_ImmGetContext( HWND ) { return NULL; }
    // 0x000842d0
    static BOOL WINAPI Dummy_ImmReleaseContext( HWND, HIMC ) { return FALSE; }
    // 0x000842e0
    static HIMC WINAPI Dummy_ImmAssociateContext( HWND, HIMC ) { return NULL; }
    // 0x000842f0
    static BOOL WINAPI Dummy_ImmGetOpenStatus( HIMC ) { return 0; }
    // 0x00084300
    static BOOL WINAPI Dummy_ImmSetOpenStatus( HIMC, BOOL ) { return 0; }
    // 0x00084310
    static BOOL WINAPI Dummy_ImmGetConversionStatus( HIMC, LPDWORD, LPDWORD ) { return 0; }
    // 0x00084320
    static HWND WINAPI Dummy_ImmGetDefaultIMEWnd( HWND ) { return NULL; }
    // 0x00084330
    static UINT WINAPI Dummy_ImmGetIMEFileNameA( HKL, LPSTR, UINT ) { return 0; }
    // 0x00084340
    static UINT WINAPI Dummy_ImmGetVirtualKey( HWND ) { return 0; }
    // 0x00084350
    static BOOL WINAPI Dummy_ImmNotifyIME( HIMC, DWORD, DWORD, DWORD ) { return FALSE; }
    // 0x00084360
    static BOOL WINAPI Dummy_ImmSetConversionStatus( HIMC, DWORD, DWORD ) { return FALSE; }
    // 0x00084370
    static BOOL WINAPI Dummy_ImmSimulateHotKey( HWND, DWORD ) { return FALSE; }
    // 0x00084380
    static BOOL WINAPI Dummy_ImmIsIME( HKL ) { return FALSE; }

    // Traditional Chinese IME
    // 0x00084390
    static UINT WINAPI Dummy_GetReadingString( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT ) { return 0; }
    // 0x000843a0
    static BOOL WINAPI Dummy_ShowReadingWindow( HIMC, BOOL ) { return FALSE; }

    // Verion library imports
    // 0x000843b0
    static BOOL APIENTRY Dummy_VerQueryValueA( const LPVOID, LPSTR, LPVOID *, PUINT ) { return 0; }
    // 0x000843c0
    static BOOL APIENTRY Dummy_GetFileVersionInfoA( LPSTR, DWORD, DWORD, LPVOID ) { return 0; }
    // 0x000843d0
    static DWORD APIENTRY Dummy_GetFileVersionInfoSizeA( LPSTR, LPDWORD ) { return 0; }

    // Function pointers: IMM32
    // 0x00140c18
    static INPUTCONTEXT* (WINAPI * _ImmLockIMC)( HIMC );
    // 0x00140c1c
    static BOOL (WINAPI * _ImmUnlockIMC)( HIMC );
    // 0x00140c20
    static LPVOID (WINAPI * _ImmLockIMCC)( HIMCC );
    // 0x00140c24
    static BOOL (WINAPI * _ImmUnlockIMCC)( HIMCC );
    // 0x00140c28
    static BOOL (WINAPI * _ImmDisableTextFrameService)( DWORD );
    // 0x00140c2c
    static LONG (WINAPI * _ImmGetCompositionStringW)( HIMC, DWORD, LPVOID, DWORD );
    // 0x00140c30
    static DWORD (WINAPI * _ImmGetCandidateListW)( HIMC, DWORD, LPCANDIDATELIST, DWORD );
    // 0x00140c34
    static HIMC (WINAPI * _ImmGetContext)( HWND );
    // 0x00140c38
    static BOOL (WINAPI * _ImmReleaseContext)( HWND, HIMC );
    // 0x00140c3c
    static HIMC (WINAPI * _ImmAssociateContext)( HWND, HIMC );
    // 0x00140c40
    static BOOL (WINAPI * _ImmGetOpenStatus)( HIMC );
    // 0x00140c44
    static BOOL (WINAPI * _ImmSetOpenStatus)( HIMC, BOOL );
    // 0x00140c48
    static BOOL (WINAPI * _ImmGetConversionStatus)( HIMC, LPDWORD, LPDWORD );
    // 0x00140c4c
    static HWND (WINAPI * _ImmGetDefaultIMEWnd)( HWND );
    // 0x00140c50
    static UINT (WINAPI * _ImmGetIMEFileNameA)( HKL, LPSTR, UINT );
    // 0x00140c54
    static UINT (WINAPI * _ImmGetVirtualKey)( HWND );
    // 0x00140c58
    static BOOL (WINAPI * _ImmNotifyIME)( HIMC, DWORD, DWORD, DWORD );
    // 0x00140c5c
    static BOOL (WINAPI * _ImmSetConversionStatus)( HIMC, DWORD, DWORD );
    // 0x00140c60
    static BOOL (WINAPI * _ImmSimulateHotKey)( HWND, DWORD );
    // 0x00140c64
    static BOOL (WINAPI * _ImmIsIME)( HKL );

    // Function pointers: Traditional Chinese IME
    // 0x00140c68
    static UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
    // 0x00140c6c
    static BOOL (WINAPI * _ShowReadingWindow)( HIMC, BOOL );

    // Function pointers: Verion library imports
    // 0x00140c70
    static BOOL (APIENTRY * _VerQueryValueA)( const LPVOID, LPSTR, LPVOID *, PUINT );
    // 0x00140c74
    static BOOL (APIENTRY * _GetFileVersionInfoA)( LPSTR, DWORD, DWORD, LPVOID );
    // 0x00140c78
    static DWORD (APIENTRY * _GetFileVersionInfoSizeA)( LPSTR, LPDWORD );

public:
    // 0x0008F370
    CDXUTIMEEditBox( CDXUTDialog *pDialog = NULL );
    // 0x0008F470
    virtual ~CDXUTIMEEditBox();

    // 0x0008bcc0

    static void Initialize();
    // 0x00085d10
    static void Uninitialize();

    // 0x00085460

    static  HRESULT StaticOnCreateDevice();
    // 0x0008C2C0
    static  bool StaticMsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 0x00085940

    static  void EnableImeSystem( bool bEnable );

    // 0x00090230

    virtual void Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );
    // 0x0008B300
    virtual bool MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
    // 0x00089110
    virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
    // 0x00085490
    virtual void UpdateRects();
    // 0x000890B0
    virtual void OnFocusIn();
    // 0x00085CC0
    virtual void OnFocusOut();

    // 0x00085C40

    void PumpMessage();

    // 0x0008F480

    virtual void RenderCandidateReadingWindow( IDirect3DDevice9* pd3dDevice, float fElapsedTime, bool bReading );
    // 0x0008FAA0
    virtual void RenderComposition( IDirect3DDevice9* pd3dDevice, float fElapsedTime );
    // 0x000900E0
    virtual void RenderIndicator( IDirect3DDevice9* pd3dDevice, float fElapsedTime );

protected:
    // samp.dll RVA: unknown (inline language helper; no standalone native entry established)
    static WORD GetLanguage() { return LOWORD( s_hklCurrent ); }
    // samp.dll RVA: unknown (inline language helper; no standalone native entry established)
    static WORD GetPrimaryLanguage() { return PRIMARYLANGID( LOWORD( s_hklCurrent ) ); }
    // samp.dll RVA: unknown (inline language helper; no standalone native entry established)
    static WORD GetSubLanguage() { return SUBLANGID( LOWORD( s_hklCurrent ) ); }
    // 0x00085430
    static void SendKey( BYTE nVirtKey );
    // 0x00085500
    static DWORD GetImeId( UINT uIndex = 0 );
    // 0x00085760
    static void CheckInputLocale();
    // 0x00085860
    static void CheckToggleState();
    // 0x00085950
    static void SetupImeApi();
    // 0x000859E0
    static void ResetCompositionString();
    // 0x00085A10
    void TruncateCompString( bool bUseBackSpace = true, int iNewStrLen = 0 );
    // 0x00085B70
    void FinalizeString( bool bSend );
    // 0x0008AC70
    static void GetReadingWindowOrientation( DWORD dwId );
    // 0x0008ADD0
    static void GetPrivateReadingString();

    // 0x00085B20

    void SendCompString();

protected:
    enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };
    enum IMESTATE { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };

    struct CCandList
    {
        WCHAR awszCandidate[MAX_CANDLIST][256];
        CUniBuffer HoriCand; // Candidate list string (for horizontal candidate window)
        int   nFirstSelected; // First character position of the selected string in HoriCand
        int   nHoriSelectedLen; // Length of the selected string in HoriCand
        DWORD dwCount;       // Number of valid entries in the candidate list
        DWORD dwSelection;   // Currently selected candidate entry relative to page top
        DWORD dwPageSize;
        int   nReadingError; // Index of the error character
        bool  bShowWindow;   // Whether the candidate list window is visible
        RECT  rcCandidate;   // Candidate rectangle computed and filled each time before rendered
    };

    struct CInputLocale
    {
        HKL   m_hKL;            // Keyboard layout
        WCHAR m_wszLangAbb[3];  // Language abbreviation
        WCHAR m_wszLang[64];    // Localized language name
    };

    // Application-wide data
    // 0x00180604
    static HINSTANCE s_hDllImm32;         // IMM32 DLL handle
    // 0x0017f6a4
    static HINSTANCE s_hDllVer;           // Version DLL handle
    // 0x0017F6A8
    static HIMC      s_hImcDef;           // Default input context

    // 0x00180600

    static HKL     s_hklCurrent;          // Current keyboard layout of the process
    // 0x0017F6B1
    static bool    s_bVerticalCand;       // Indicates that the candidates are listed vertically
    // 0x00140C9C
    static LPWSTR  s_wszCurrIndicator;    // Points to an indicator string that corresponds to current input locale
    // 0x00140C7C
    static WCHAR   s_aszIndicator[5][3];  // String to draw to indicate current input locale
    // 0x0017F695
    static bool    s_bInsertOnType;       // Insert the character as soon as a key is pressed (Korean behavior)
    // 0x0017f6b4
    static HINSTANCE s_hDllIme;           // Instance handle of the current IME module
    // 0x0018061C
    static IMESTATE  s_ImeState;          // IME global state
    // 0x0017F6B0
    static bool    s_bEnableImeSystem;    // Whether the IME system is active
    // 0x0017F69C
    static POINT   s_ptCompString;        // Composition string position. Updated every frame.
    // 0x0017F690
    static int     s_nCompCaret;          // Caret position of the composition string
    // 0x0017F6AC
    static int     s_nFirstTargetConv;    // Index of the first target converted char in comp string.  If none, -1.
    // 0x0018062c
    static CUniBuffer s_CompString;       // Buffer to hold the composition string (we fix its length)
    // 0x0017F6B8
    static BYTE    s_abCompStringAttr[MAX_COMPSTRING_SIZE];
    // 0x0017F7B8
    static DWORD   s_adwCompStringClause[MAX_COMPSTRING_SIZE];
    // 0x001803C0
    static WCHAR   s_wszReadingString[32];// Used only with horizontal reading window (why?)
    // 0x00180640
    static CCandList s_CandList;          // Data relevant to the candidate list
    // 0x0017F696
    static bool    s_bShowReadingWindow;  // Indicates whether reading window is visible
    // 0x001803BA
    static bool    s_bHorizontalReading;  // Indicates whether the reading window is vertical or horizontal
    // 0x001803B9
    static bool    s_bChineseIME;
    // 0x00180620
    static CGrowableArray< CInputLocale > s_Locale; // Array of loaded keyboard layout on system

    // Color of various IME elements
    D3DCOLOR       m_ReadingColor;        // Reading string color
    D3DCOLOR       m_ReadingWinColor;     // Reading window color
    D3DCOLOR       m_ReadingSelColor;     // Selected character in reading string
    D3DCOLOR       m_ReadingSelBkColor;   // Background color for selected char in reading str
    D3DCOLOR       m_CandidateColor;      // Candidate string color
    D3DCOLOR       m_CandidateWinColor;   // Candidate window color
    D3DCOLOR       m_CandidateSelColor;   // Selected candidate string color
    D3DCOLOR       m_CandidateSelBkColor; // Selected candidate background color
    D3DCOLOR       m_CompColor;           // Composition string color
    D3DCOLOR       m_CompWinColor;        // Composition string window color
    D3DCOLOR       m_CompCaretColor;      // Composition string caret color
    D3DCOLOR       m_CompTargetColor;     // Composition string target converted color
    D3DCOLOR       m_CompTargetBkColor;   // Composition string target converted background
    D3DCOLOR       m_CompTargetNonColor;  // Composition string target non-converted color
    D3DCOLOR       m_CompTargetNonBkColor;// Composition string target non-converted background
    D3DCOLOR       m_IndicatorImeColor;   // Indicator text color for IME
    D3DCOLOR       m_IndicatorEngColor;   // Indicator text color for English
    D3DCOLOR       m_IndicatorBkColor;    // Indicator text background color

    // Edit-control-specific data
    int            m_nIndicatorWidth;     // Width of the indicator symbol
    RECT           m_rcIndicator;         // Rectangle for drawing the indicator button

#if defined(DEBUG) | defined(_DEBUG)
    static bool    m_bIMEStaticMsgProcCalled;
#endif
};

// 0x0017fbb8
extern char g_inputAnsiText[2049];
// 0x00085060
const char* InputWideToAnsi(const wchar_t*);
// 0x0009b700
void InputBufferAnsiToWide(const char*, wchar_t*, int);

// 0x00180400
extern wchar_t g_inputMaskedText[256];
// 0x000853a0
const wchar_t* InputMaskWideText(const wchar_t*);

// 0x0017f698
extern DWORD g_inputImeActivityTick;
// 0x00085e60
bool ChatImeActive();
// samp.dll RVA: unknown (extracted signed tick arithmetic; no standalone native entry)
inline bool InputImeRecent(DWORD now, DWORD start)
{
    return (LONG)(now - start) < 300;
}

// 0x00085940
void InputEnableIme(bool);
// 0x00085460
void InputResetIme();

#include <DXUT.h>
#include <DXUTmisc.h>
#include <d3dx9.h>
#include <stddef.h>
#include <windows.h>
class CDXUTDialog;
#pragma pack(push, 1)
class CDXUTDialogDL;
class CDXUTButtonDL;
// ANSI factory view of the same parent, not a separately constructed dialog.
class CDXUTDialogDL
{
public:
    // 0x0006ee60
    CDXUTButtonDL* GetButton(int id);
    // 0x00090790
    HRESULT AddButton(int id, const char* text, int x, int y, int width, int height, UINT hotkey = 0, bool isDefault = false, CDXUTButtonDL** created = 0);
};
// Native ANSI controls share the established packed SDK base, not its wide text.
struct CDXUTStaticDL : CDXUTControl
{
    char text[260];
    // 0x0008de50
    CDXUTStaticDL(CDXUTDialog* parent);
    // 0x0008dec0
    virtual ~CDXUTStaticDL();
    // 0x0008ded0
    virtual void Render(IDirect3DDevice9* device, float elapsed);
    // 0x0008deb0
    virtual BOOL ContainsPoint(POINT point);
    // 0x000875c0
    HRESULT SetText(const char* text);
};
class CDXUTButtonDL : public CDXUTStaticDL
{
    friend class CDXUTDialogDL;
    // 0x0008df20
    CDXUTButtonDL(CDXUTDialog* parent);

public:
    bool pressed;
    // 0x0008dfc0
    virtual ~CDXUTButtonDL();
    // 0x0008dfd0
    virtual void Render(IDirect3DDevice9* device, float elapsed);
    // 0x000848d0
    virtual bool HandleKeyboard(UINT message, WPARAM wparam, LPARAM lparam);
    // 0x00084930
    virtual bool HandleMouse(UINT message, POINT point, WPARAM wparam, LPARAM lparam);
    // 0x0008df80
    virtual bool CanHaveFocus();
    // 0x0008dfa0
    virtual void OnHotkey();
    // 0x0008df60
    virtual BOOL ContainsPoint(POINT point);
};
// Observed native storage views. Construction, virtual interfaces and rendering
// remain separate recovery work; these are not the SDK Unicode widget types.
struct DXUTListBoxItemDL
{
    char text[257];
    char subitems[3][129];
    void* data;
    RECT activeRect;
    bool selected;
    DWORD color;
    bool flag;
};

// These public SDK aliases resolve to its original function-local storage.
// 0x00181a8d
extern bool g_dxutScrollDragging;
// 0x00181a90
extern int g_dxutScrollThumbOffsetY;
// 0x00181a95
extern bool g_dxutListScrollInitialized;
class CDXUTListBoxDL : public CDXUTControl
{
    typedef char ScrollPositionOffset[(offsetof(CDXUTScrollBar, m_nPosition) == 0x8e) ? 1 : -1];
    typedef char ScrollArrowOffset[(offsetof(CDXUTScrollBar, m_dArrowTS) == 0xaa) ? 1 : -1];
    friend class CDialog;

public:
    int columnCount;
    int columnWidths[3];
    CDXUTScrollBar scrollBar;
    RECT textRect, selectionRect;
    int scrollBarWidth, border, margin, textHeight;
    DWORD style;
    int selected, selectionStart;
    bool drag;
    CGrowableArray<DXUTListBoxItemDL*> items;
    // Native column accessors are unchecked, including negative indices.
    // 0x0006a170
    void SetColumnCount(int value);
    // 0x0006ee80
    int GetColumnCount();
    // 0x0006a180
    void SetColumnWidth(int column, int value);
    // 0x0006ee90
    int GetColumnWidth(int column);
    // 0x0008e810
    CDXUTListBoxDL(CDXUTDialog* parent);
    // 0x0008e8d0
    virtual ~CDXUTListBoxDL();
    // 0x0008e900
    virtual void Render(IDirect3DDevice9* device, float elapsed);
    // 0x0008a500
    virtual bool HandleKeyboard(UINT message, WPARAM wparam, LPARAM lparam);
    // 0x0008a6e0
    virtual bool HandleMouse(UINT message, POINT point, WPARAM wparam, LPARAM lparam);
    // 0x00088700
    virtual void UpdateRects();
    // 0x0008e8c0
    virtual HRESULT OnInit();
    // 0x0008e8a0
    virtual bool CanHaveFocus();
    // 0x0008a270
    HRESULT AddItem(const char* text, void* data, int row);
    // Unlike AddItem, native insertion leaves subitems and the final flag untouched.
    // 0x0008a370
    HRESULT InsertItem(int index, const char* text, void* data, DWORD color);
    // 0x00088880
    void SetSubItemText(int row, int column, const char* text);
    // 0x0008a440
    DXUTListBoxItemDL* GetItem(int row);
    // 0x0008c270
    void RemoveAllItems();
    // 0x00088800
    void RemoveItem(int index);
    // 0x0008a470
    void SelectItem(int index);
    // 0x000888f0
    int GetSelectedIndex(int previous = -1);
    // 0x0006e240
    DXUTListBoxItemDL* GetSelectedItem(int previous = -1);
    // 0x0006e260
    CDXUTScrollBar* GetScrollBar();
};
#pragma pack(pop)
typedef char DLListItemExtent[(sizeof(DXUTListBoxItemDL) == 0x29e) ? 1 : -1];
typedef char DLControlExtent[(sizeof(CDXUTControl) == 0x4d) ? 1 : -1];
typedef char DLStaticExtent[(sizeof(CDXUTStaticDL) == 0x151) ? 1 : -1];
typedef char DLButtonExtent[(sizeof(CDXUTButtonDL) == 0x152) ? 1 : -1];
typedef char DLListItemData[(offsetof(DXUTListBoxItemDL, data) == 0x284) ? 1 : -1];
typedef char DLScrollExtent[(sizeof(CDXUTScrollBar) == 0xb2) ? 1 : -1];
typedef char DLListExtent[(sizeof(CDXUTListBoxDL) == 0x158) ? 1 : -1];
typedef char DLListScroll[(offsetof(CDXUTListBoxDL, scrollBar) == 0x5d) ? 1 : -1];
typedef char DLListTextRect[(offsetof(CDXUTListBoxDL, textRect) == 0x10f) ? 1 : -1];
typedef char DLListSelectionRect[(offsetof(CDXUTListBoxDL, selectionRect) == 0x11f) ? 1 : -1];
typedef char DLListArrayOffset[(offsetof(CDXUTListBoxDL, items) == 0x14c) ? 1 : -1];



#endif // DXUT_GUI_H