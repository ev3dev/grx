{ Converted by Sven Hilscher eMail sven@rufus.lan-ks.de
  from header file grx20.h

  grx20.h ---- GRX 2.x API functions and data structure declarations

  Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
  [e-mail: csaba@vuse.vanderbilt.edu]

  This file is part of the GRX graphics library.

  The GRX graphics library is free software; you can redistribute it
  and/or modify it under some conditions; see the "copying.grx" file
  for details.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. }

{ Define this if you want to use the Linux console version.
  (Ignored on non-Linux systems.) }
{.$define LINUX_CONSOLE}

{$gnu-pascal,I-}
{$ifdef __DJGPP__}
  {$L grx20}
{$elif defined (__MINGW32__)}
  {$L grx20}
{$elif defined (_WIN32)}
  {$L grxW32, vfs.c, user32, gdi32}
{$elif defined (linux) and defined (LINUX_CONSOLE)}
  {$L grx20, vga}
{$else}
  {$L grx20X, X11}
{$endif}

{ Uncomment those of the following libraries that you need }
{.$L tiff}
{.$L jpeg}
{.$L png}
{.$L z}
{.$L socket}

unit GRX;

interface

{$if __GPC_RELEASE__ >= 20030303}
{$define asmname external name}
{$define varasmname external name}
{$else}
{$define varasmname external; asmname}
{$endif}

const
  MaxVarSize = MaxInt div 8;

type
  MemPtr      = ^Char;
{$if __GPC_RELEASE__ >= 20030424}
  GrColor     = Integer attribute (Size = 32);  { color and operation type, must be 32bit }
{$else}
  GrColor     = Integer (32);  { color and operation type, must be 32bit }
{$endif}
  GrColorPtr  = ^GrColor;
  GrColors    = array [0 .. MaxVarSize div SizeOf (GrColor) - 1] of GrColor;
  GrColorsPtr = ^GrColors;

  { This definition is compatible with the grx
    definition 'int pts[][2]' used to define polygons }
  PointType = record
    x, y: Integer;
  end;

const
  { these are the supported configurations: }
  GRX_Version_TCC_8086_DOS  = 1;  { also works with BCC }
  GRX_Version_GCC_386_GO32  = 2;  { DJGPP }
  GRX_Version_GCC_386_Linux = 3;  { the real stuff }
  GRX_Version_Generic_X11   = 4;  { generic X11 version }
  GRX_Version_Watcom_DOS4GW = 5;  { Watcom C++ 11.0 32 Bit }

  { available video modes (for 'GrSetMode') }

  Gr_Unknown_Mode                      = -1; { initial state }
  { ============= modes which clear the video memory ============= }
  Gr_80_25_Text                        = 0;  { Extra parameters for GrSetMode: }
  Gr_Default_Text                      = 1;
  Gr_Width_Height_Text                 = 2;  { Integer w,Integer h }
  Gr_Biggest_Text                      = 3;
  Gr_320_200_Graphics                  = 4;
  Gr_Default_Graphics                  = 5;
  Gr_Width_Height_Graphics             = 6;  { Integer w,Integer h }
  Gr_Biggest_Noninterlaced_Graphics    = 7;
  Gr_Biggest_Graphics                  = 8;
  Gr_Width_Height_Color_Graphics       = 9;  { Integer w,Integer h,Word nc }
  Gr_Width_Height_Color_Text           = 10; { Integer w,Integer h,Word nc }
  Gr_Custom_Graphics                   = 11; { Integer w,Integer h,Word nc,Integer vx,Integer vy }
  { ==== equivalent modes which do not clear the video memory ==== }
  Gr_NC_80_25_Text                     = 12;
  Gr_NC_Default_Text                   = 13;
  Gr_NC_Width_Height_Text              = 14; { Integer w,Integer h }
  Gr_NC_Biggest_Text                   = 15;
  Gr_NC_320_200_Graphics               = 16;
  Gr_NC_Default_Graphics               = 17;
  Gr_NC_Width_Height_Graphics          = 18; { Integer w,Integer h }
  Gr_NC_Biggest_Noninterlaced_Graphics = 19;
  Gr_NC_Biggest_Graphics               = 20;
  Gr_NC_Width_Height_Color_Graphics    = 21; { Integer w,Integer h,Word nc }
  Gr_NC_Width_Height_Color_Text        = 22; { Integer w,Integer h,Word nc }
  Gr_NC_Custom_Graphics                = 23; { Integer w,Integer h,Word nc,Integer vx,Integer vy }
  { ==== plane instead of color based modes ==== }
  { colors = 1 << bpp  >>> resort enum for GRX3 <<< }
  Gr_Width_Height_BPP_Graphics         = 24; { int w,int h,int bpp }
  Gr_Width_Height_BPP_Text             = 25; { int w,int h,int bpp }
  Gr_Custom_BPP_Graphics               = 26; { int w,int h,int bpp,int vx,int vy }
  Gr_NC_Width_Height_BPP_Graphics      = 27; { int w,int h,int bpp }
  Gr_NC_Width_Height_BPP_Text          = 28; { int w,int h,int bpp }
  Gr_NC_Custom_BPP_Graphics            = 29; { int w,int h,int bpp,int vx,int vy }

  { Available frame modes (video memory layouts) }

  { ====== video frame buffer modes ====== }
  Gr_FrameUndef   =  0;       { undefined }
  Gr_FrameText    =  1;       { text modes }
  Gr_FrameHerc1   =  2;       { Hercules mono }
  Gr_FrameEGAVGA1 =  3;       { EGA VGA mono }
  Gr_FrameEGA4    =  4;       { EGA 16 color }
  Gr_FrameSVGA4   =  5;       { (Super) VGA 16 color }
  Gr_FrameSVGA8   =  6;       { (Super) VGA 256 color }
  Gr_FrameVGA8X   =  7;       { VGA 256 color mode X }
  Gr_FrameSVGA16  =  8;       { Super VGA 32768/65536 color }
  Gr_FrameSVGA24  =  9;       { Super VGA 16M color }
  Gr_FrameSVGA32L = 10;       { Super VGA 16M color padded #1 }
  Gr_FrameSVGA32H = 11;       { Super VGA 16M color padded #2 }
  { ==== modes provided by the X11 driver ===== }
  Gr_FrameXWin1   = Gr_FrameEGAVGA1;
  Gr_FrameXWin4   = Gr_FrameSVGA4;
  Gr_FrameXWin8   = Gr_FrameSVGA8;
  Gr_FrameXWin16  = Gr_FrameSVGA16;
  Gr_FrameXWin24  = Gr_FrameSVGA24;
  Gr_FrameXWin32L = Gr_FrameSVGA32L;
  Gr_FrameXWin32H = Gr_FrameSVGA32H;
  { ==== linear frame buffer modes  ====== }
  Gr_FrameSVGA8_LFB   = 12;   { (Super) VGA 256 color }
  Gr_FrameSVGA16_LFB  = 13;   { Super VGA 32768/65536 color }
  Gr_FrameSVGA24_LFB  = 14;   { Super VGA 16M color }
  Gr_FrameSVGA32L_LFB = 15;   { Super VGA 16M color padded #1 }
  Gr_FrameSVGA32H_LFB = 16;   { Super VGA 16M color padded #2 }
  { ====== system RAM frame buffer modes ====== }
  Gr_FrameRAM1    = 17;       { mono }
  Gr_FrameRAM4    = 18;       { 16 color planar }
  Gr_FrameRAM8    = 19;       { 256 color }
  Gr_FrameRAM16   = 20;       { 32768/65536 color }
  Gr_FrameRAM24   = 21;       { 16M color }
  Gr_FrameRAM32L  = 22;       { 16M color padded #1 }
  Gr_FrameRAM32H  = 23;       { 16M color padded #2 }
  Gr_FrameRAM3x8  = 24;       { 16M color planar (image mode) }
  { ====== markers for scanning modes ====== }
  Gr_FirstTextFrameMode     = Gr_FrameText;
  Gr_LastTextFrameMode      = Gr_FrameText;
  Gr_FirstGraphicsFrameMode = Gr_FrameHerc1;
  Gr_LastGraphicsFrameMode  = Gr_FrameSVGA32H_LFB;
  Gr_FirstRAMframeMode      = Gr_FrameRAM1;
  Gr_LastRAMframeMode       = Gr_FrameRAM3x8;

  { supported video adapter types }

  Gr_Unknown = -1;          { not known (before driver set) }
  Gr_VGA     =  0;          { VGA adapter }
  Gr_EGA     =  1;          { EGA adapter }
  Gr_Herc    =  2;          { Hercules mono adapter }
  Gr_8514A   =  3;          { 8514A or compatible }
  Gr_S3      =  4;          { S3 graphics accelerator }
  Gr_XWin    =  5;          { X11 driver }
  Gr_Mem     =  6;          { memory only driver }

  Gr_Max_Polygon_Pointers = 1000000;
  Gr_Max_Ellipse_Pointers = 1024 + 5;
  Gr_Max_Angle_Value      = 3600;
  Gr_Arc_Style_Open       = 0;
  Gr_Arc_Style_Close1     = 1;
  Gr_Arc_Style_Close2     = 2;

  { bits in the GrVideoDriver.drvflags field: }
  Gr_DriverF_User_Resolution = 1; { set if driver supports user setable arbitrary resolution }

type

  { The video driver descriptor structure }
  { struct _GR_videoDriver }
  GrVideoDriver = record
    Name      : CString;        { driver name }
    Adapter   : Char;           { adapter type }
    Inherit   : ^GrVideoDriver; { inherit video modes from this }
    Modes     : ^GrVideoMode;   { table of supported modes }
    NModes    : Integer;        { number of modes }
    Detect    : Pointer;
    Init      : Pointer;
    Reset     : Pointer;
    SelectMode: Pointer;
    DrvFlags  : Integer;
  end;
  GrVideoDriverPtr = ^GrVideoDriver;

  { Video driver mode descriptor structure }
  { struct _GR_videoMode }
  GrVideoMode = record
    Present     : Char;         { is it really available? }
    BPP         : Char;       { log2 of # of colors }
    Width,Height: ShortInt;    { video mode geometry }
    Mode        : ShortInt;         { BIOS mode number (if any) }
    LineOffset  : Integer;         { scan line length }
    PrivData    : Integer;        { driver can use it for anything }
    ExtInfo     : ^GrVideoModeExt;   { extra info (maybe shared) }
  end;
  GrVideoModePtr = ^GrVideoMode;

  { Video driver mode descriptor extension structure. This is a separate
    structure accessed via a pointer from the main mode descriptor. The
    reason for this is that frequently several modes can share the same
    extended info. }

  { struct _GR_videoModeExt }
  GrVideoModeExt = record
    Mode        : Integer;                     { frame driver for this video mode }
    Drv         : ^GrFrameDriver;              { optional frame driver override }
    Frame       : MemPtr;                      { frame buffer address }
    CPrec       : array [1 .. 3] of ByteCard;  { color component precisions }
    CPos        : array [1 .. 3] of ByteCard;  { color component bit positions }
    Flags       : Integer;                     { mode flag bits; see "grdriver.h" }
    Setup       : Pointer;
    SetVSize    : Pointer;
    Scroll      : Pointer;
    SetBank     : Pointer;
    SetRWBanks  : Pointer;
    LoadColor   : Pointer;
    LFB_Selector: Integer;
  end;

  { The frame driver descriptor structure. }

  { struct _GR_frameDriver }
  GrFrameDriver = record
    Mode              : Integer; { supported frame access mode }
    RMode             : Integer; { matching RAM frame (if video) }
    Is_Video          : Integer; { video RAM frame driver ? }
    Row_Align         : Integer; { scan line size alignment }
    Num_Planes        : Integer; { number of planes }
    Bits_Per_Pixel    : Integer; { bits per pixel }
    Max_Plane_Size    : Integer; { maximum plane size in bytes }
    Init              : Pointer;
    ReadPixel         : Pointer;
    DrawPixel         : Pointer;
    DrawLine          : Pointer;
    DrawHLine         : Pointer;
    DrawVLine         : Pointer;
    DrawBlock         : Pointer;
    DrawBitmap        : Pointer;
    DrawPattern       : Pointer;
    BitBlt            : Pointer;
    BltV2R            : Pointer;
    BltR2V            : Pointer;
    GetIndexedScanline: Pointer;
    PutScanLine       : Pointer;
  end;
  GrFrameDriverPtr = ^GrFrameDriver;

  { driver and mode info structure }

  { extern const struct _GR_driverInfo }
  GrDriverInfoType = record
    VDriver    : ^GrVideoDriver; { the current video driver }
    CurMode    : ^GrVideoMode;   { current video mode pointer }
    ActMode    : GrVideoMode;    { copy of above, resized if virtual }
    FDriver    : GrFrameDriver;  { frame driver for the current context }
    SDriver    : GrFrameDriver;  { frame driver for the screen }
    TDriver    : GrFrameDriver;  { a dummy driver for text modes }
    MCode      : ByteCard;       { code for the current mode }
    DefTW,DefTH: Integer;        { default text mode size }
    DefGW,DefGH: Integer;        { default graphics mode size }
    DefTC,DefGC: GrColor;        { default text and graphics colors }
    VPosX,VPosY: Integer;        { current virtual viewport position }
    ErrsFatal  : Integer;        { if set, exit upon errors }
    ModeRestore: Integer;        { restore startup video mode if set }
    SplitBanks : Integer;        { indicates separate R/W banks }
    CurBank    : Integer;        { currently mapped bank }
    MdSetHook  : Pointer;        { callback for mode set }
    SetBank    : Pointer;        { banking routine }
    SetRWBanks : Pointer;        { split banking routine }
  end;

var
  GrDriverInfo: ^GrDriverInfoType; varasmname 'GrDriverInfo';

{ setup stuff }

function  GrSetDriver(DrvSpec: CString):Integer; asmname 'GrSetDriver';
function  GrSetMode(m, w, h, nc, vx, vy:Integer):Integer; asmname 'GrSetMode';
function  GrSetViewport(XPos, YPos: Integer):Integer; asmname 'GrSetViewport';
procedure GrSetModeHook(HookFunc:Pointer); asmname 'GrSetModeHook';
procedure GrSetModeRestore(RestoreFlag:Boolean); asmname 'GrSetModeRestore';

procedure GrSetErrorHandling(ExitIfError:Boolean); asmname 'GrSetErrorHandling';
procedure GrSetEGAVGAmonoDrawnPlane(Plane:Integer); asmname 'GrSetEGAVGAmonoDrawnPlane';
procedure GrSetEGAVGAmonoShownPlane(Plane:Integer); asmname 'GrSetEGAVGAmonoShownPlane';

function  GrGetLibraryVersion: Integer; asmname 'GrGetLibraryVersion';
function  GrGetLibrarySystem: Integer; asmname 'GrGetLibrarySystem';

{ inquiry stuff ---- many of these can be macros }

function  GrCurrentMode:Integer; asmname 'GrCurrentMode';
function  GrAdapterType:Integer; asmname 'GrAdapterType';
function  GrCurrentFrameMode:Integer; asmname 'GrCurrentFrameMode';
function  GrScreenFrameMode:Integer; asmname 'GrScreenFrameMode';
function  GrCoreFrameMode:Integer; asmname 'GrCoreFrameMode';

function  GrCurrentVideoDriver: GrVideoDriverPtr; asmname 'GrCurrentVideoDriver' ;
function  GrCurrentVideoMode:GrVideoModePtr; asmname 'GrCurrentVideoMode';
function  GrVirtualVideoMode:GrVideoModePtr; asmname 'GrVirtualVideoMode';
function  GrCurrentFrameDriver: GrFrameDriverPtr; asmname 'GrCurrentFrameDriver';
function  GrScreenFrameDriver: GrFrameDriverPtr; asmname 'GrScreenFrameDriver';

function  GrFirstVideoMode(FMode:Integer):GrVideoModePtr; asmname 'GrFirstVideoMode';
function  GrNextVideoMode(Prev:GrVideoModePtr):GrVideoModePtr; asmname 'GrNextVideoMode';

function  GrScreenX:Integer; asmname 'GrScreenX';
function  GrScreenY:Integer; asmname 'GrScreenY';
function  GrVirtualX:Integer; asmname 'GrVirtualX';
function  GrVirtualY:Integer; asmname 'GrVirtualY';
function  GrViewportX:Integer; asmname 'GrViewportX';
function  GrViewportY:Integer; asmname 'GrViewportY';
function  GrScreenIsVirtual:Boolean; asmname 'GrScreenIsVirtual';

{ RAM context geometry and memory allocation inquiry stuff }

function  GrFrameNumPlanes(md:Integer):Integer; asmname 'GrFrameNumPlanes';
function  GrFrameLineOffset(md,Width:Integer):Integer; asmname 'GrFrameLineOffset';
function  GrFramePlaneSize(md,w,h:Integer):Integer; asmname 'GrFramePlaneSize';
function  GrFrameContextSize(md,w,h:Integer):Integer; asmname 'GrFrameContextSize';

function  GrNumPlanes:Integer; asmname 'GrNumPlanes';
function  GrLineOffset(Width:Integer):Integer; asmname 'GrLineOffset';
function  GrPlaneSize(w,h:Integer):Integer; asmname 'GrPlaneSize';
function  GrContextSize(w,h:Integer):Integer; asmname 'GrContextSize';

{ ==================================================================
            FRAME BUFFER, CONTEXT AND CLIPPING STUFF
  ================================================================== }

type
  GrFrameType = record
    gf_BaseAddr  : array [0 .. 3] of MemPtr; { base address of frame memory }
    gf_Selector  : ShortInt;                 { frame memory segment selector }
    gf_OnScreen  : Char;                     { is it in video memory ? }
    gf_MemFlags  : Char;                     { memory allocation flags }
    gf_LineOffset: Integer;                  { offset to next scan line in bytes }
    gf_Driver    : ^GrFrameDriver;           { frame access functions }
  end;

  { struct _GR_context }
  GrContext = record
    gc_Frame    : GrFrameType;  { frame buffer info }
    gc_Root     : ^GrContext;   { context which owns frame }
    gc_XMax     : Integer;      { max X coord (width  - 1) }
    gc_YMax     : Integer;      { max Y coord (height - 1) }
    gc_XOffset  : Integer;      { X offset from root's base }
    gc_YOffset  : Integer;      { Y offset from root's base }
    gc_XClipLo  : Integer;      { low X clipping limit }
    gc_YClipLo  : Integer;      { low Y clipping limit }
    gc_XClipHi  : Integer;      { high X clipping limit }
    gc_YClipHi  : Integer;      { high Y clipping limit }
    gc_UsrXBase : Integer;      { user window min X coordinate }
    gc_UsrYBase : Integer;      { user window min Y coordinate }
    gc_UsrWidth : Integer;      { user window width  }
    gc_UsrHeight: Integer;      { user window height }
  end;
  GrContextPtr = ^GrContext;

  { extern const struct _GR_contextInfo }
  GrContextInfoType = record
    Current: GrContext;  { the current context }
    Screen : GrContext;  { the screen context }
  end;

var
  GrContextInfo: GrContextInfoType; varasmname 'GrContextInfo';

function  GrCreateContext(w, h: Integer; Memory: MemPtr; Where: GrContextPtr): GrContextPtr; asmname 'GrCreateContext';
function  GrCreateFrameContext(md: ByteCard; w, h: Integer; Memory: MemPtr; Where: GrContextPtr): GrContextPtr; asmname 'GrCreateFrameContext';
function  GrCreateSubContext(x1, y1, x2, y2: Integer; Parent, Where: GrContextPtr): GrContextPtr; asmname 'GrCreateSubContext';
function  GrSaveContext(Where: GrContextPtr): GrContextPtr; asmname 'GrSaveContext';
function  GrCurrentContext: GrContextPtr; asmname 'GrCurrentContext';
function  GrScreenContext: GrContextPtr; asmname 'GrScreenContext';

procedure GrDestroyContext(Context: GrContextPtr); asmname 'GrDestroyContext';
procedure GrResizeSubContext(Context: GrContextPtr; x1, y1, x2, y2: Integer); asmname 'GrResizeSubContext';
procedure GrSetContext(Context: GrContextPtr); asmname 'GrSetContext';

procedure GrSetClipBox(x1, y1, x2, y2:Integer); asmname 'GrSetClipBox';
procedure GrGetClipBox(var x1p, y1p, x2p, y2p: Integer); asmname 'GrGetClipBox';
procedure GrResetClipBox; asmname 'GrResetClipBox';

function  GrMaxX: Integer; asmname 'GrMaxX';
function  GrMaxY: Integer; asmname 'GrMaxY';
function  GrSizeX:Integer; asmname 'GrSizeX';
function  GrSizeY:Integer; asmname 'GrSizeY';
function  GrLowX: Integer; asmname 'GrLowX';
function  GrLowY: Integer; asmname 'GrLowY';
function  GrHighX:Integer; asmname 'GrHighX';
function  GrHighY:Integer; asmname 'GrHighY';

{ ==================================================================
                           COLOR STUFF
  ================================================================== }

{ Flags to 'OR' to colors for various operations }

const
  GrWrite      = 0                ;  { write color }
  GrXor        = $01000000        ;  { to "xor" any color to the screen }
  GrOr         = $02000000        ;  { to "or" to the screen }
  GrAnd        = $03000000        ;  { to "and" to the screen }
  GrImage      = $04000000        ;  { BLIT: write, except given color }
  GrCValueMask = $00ffffff        ;  { color value mask }
  GrCModeMask  = $ff000000        ;  { color operation mask }
  GrNoColor    = GrXor or 0       ;  { GrNoColor is used for "no" color }

function  GrColorValue(c:GrColor):GrColor; asmname 'GrColorValue';
function  GrColorMode(c:GrColor):GrColor; asmname 'GrColorMode';
function  GrWriteModeColor(c:GrColor):GrColor; asmname 'GrWriteModeColor';
function  GrXorModeColor(c:GrColor):GrColor; asmname 'GrXorModeColor';
function  GrOrModeColor(c:GrColor):GrColor; asmname 'GrOrModeColor';
function  GrAndModeColor(c:GrColor):GrColor; asmname 'GrAndModeColor';
function  GrImageModeColor(c:GrColor):GrColor; asmname 'GrImageModeColor';

procedure GrResetColors; asmname 'GrResetColors';
procedure GrSetRGBcolorMode; asmname 'GrSetRGBcolorMode';
procedure GrRefreshColors; asmname 'GrRefreshColors';

function  GrNumColors:GrColor; asmname 'GrNumColors';
function  GrNumFreeColors:GrColor; asmname 'GrNumFreeColors';

function  GrBlack:GrColor; asmname 'GrBlack';
function  GrWhite:GrColor; asmname 'GrWhite';

function  GrBuildRGBcolorT(r,g,b:Integer):GrColor; asmname 'GrBuildRGBcolorT';
function  GrBuildRGBcolorR(r,g,b:Integer):GrColor; asmname 'GrBuildRGBcolorR';
function  GrRGBcolorRed(c:GrColor):Integer ; asmname 'GrRGBcolorRed';
function  GrRGBcolorGreen(c:GrColor):Integer ; asmname 'GrRGBcolorGreen';
function  GrRGBcolorBlue(c:GrColor):Integer ; asmname 'GrRGBcolorBlue';

function  GrAllocColor(r,g,b:Integer):GrColor; asmname 'GrAllocColor'; { shared, read-only }
function  GrAllocColorID(r,g,b:Integer):GrColor; asmname 'GrAllocColorID'; { potentially inlined version }
function  GrAllocCell:GrColor; asmname 'GrAllocCell'; { unshared, read-write }

function  GrAllocEgaColors:GrColorsPtr; asmname 'GrAllocEgaColors'; { shared, read-only standard EGA colors }

procedure GrSetColor(c:GrColor; r,g,b:Integer); asmname 'GrSetColor';
procedure GrFreeColor(c:GrColor); asmname 'GrFreeColor';
procedure GrFreeCell(c:GrColor);  asmname 'GrFreeCell';

procedure GrQueryColor(c:GrColor; var r,g,b:Integer); asmname 'GrQueryColor';
procedure GrQueryColorID(c:GrColor; var r,g,b:Integer); asmname 'GrQueryColorID';

function  GrColorSaveBufferSize:Integer ; asmname 'GrColorSaveBufferSize';
procedure GrSaveColors(Buffer:Pointer); asmname 'GrSaveColors';
procedure GrRestoreColors(Buffer:Pointer); asmname 'GrRestoreColors';

type
  { typedef struct } { framed box colors }
  GrFBoxColors = record
    fbx_IntegerColor: Integer;
    fbx_TopColor    : Integer;
    fbx_RightColor  : Integer;
    fbx_BottomColor : Integer;
    fbx_LeftColor   : Integer;
  end;

procedure GrClearScreen(bg:GrColor); asmname 'GrClearScreen';
procedure GrClearClipBox(bg:GrColor); asmname 'GrClearClipBox';
procedure GrPlot(x, y: Integer; c:GrColor); asmname 'GrPlot';
procedure GrLine(x1, y1, x2, y2: Integer; c:GrColor); asmname 'GrLine';
procedure GrHLine(x1, x2, y:Integer; c:GrColor); asmname 'GrHLine';
procedure GrVLine(x, y1, y2: Integer; c: GrColor); asmname 'GrVLine';
procedure GrBox(x1, y1, x2, y2: Integer; c: GrColor); asmname 'GrBox';
procedure GrFilledBox(x1, y1, x2, y2: Integer; c: GrColor); asmname 'GrFilledBox';
procedure GrFramedBox(x1, y1, x2, y2, Wdt: Integer; protected var c: GrFBoxColors); asmname 'GrFramedBox';
function  GrGenerateEllipse(xc, yc, xa, ya:Integer; var Points{ : array of PointType }):Integer; asmname 'GrGenerateEllipse';
function  GrGenerateEllipseArc(xc, yc, xa, ya, Start, Ende: Integer; var Points{ : array of PointType }):Integer; asmname 'GrGenerateEllipseArc';
procedure GrLastArcCoords(var xs, ys, xe, ye, xc, yc: Integer); asmname 'GrLastArcCoords';
procedure GrCircle(xc, yc, r: Integer; c: GrColor); asmname 'GrCircle';
procedure GrEllipse(xc, yc, xa, ya: Integer; c: GrColor); asmname 'GrEllipse';
procedure GrCircleArc(xc, yc, r, Start, Ende, Style: Integer; c: GrColor); asmname 'GrCircleArc';
procedure GrEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; c: GrColor); asmname 'GrEllipseArc';
procedure GrFilledCircle(xc, yc, r: Integer; c: GrColor); asmname 'GrFilledCircle';
procedure GrFilledEllipse(xc, yc, xa, ya: Integer; c: GrColor); asmname 'GrFilledEllipse';
procedure GrFilledCircleArc(xc, yc, r, Start, Ende, Style: Integer; c: GrColor); asmname 'GrFilledCircleArc';
procedure GrFilledEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; c: GrColor); asmname 'GrFilledEllipseArc';
procedure GrPolyLine(NumPts: Integer; var Points{ : array of PointType }; c: GrColor); asmname 'GrPolyLine';
procedure GrPolygon(NumPts: Integer; var Points{ : array of PointType }; c: GrColor); asmname 'GrPolygon';
procedure GrFilledConvexPolygon(NumPts: Integer; var Points{ : array of PointType }; c: GrColor); asmname 'GrFilledConvexPolygon';
procedure GrFilledPolygon(NumPts: Integer; var Points{ : array of PointType }; c: GrColor); asmname 'GrFilledPolygon';
procedure GrBitBlt(Dest: GrContextPtr; x, y: Integer; Src: GrContextPtr; x1, y1, x2, y2: Integer; op: GrColor); asmname 'GrBitBlt';
function  GrPixel(x, y:Integer):GrColor; asmname 'GrPixel';
function  GrPixelC(c: GrContextPtr; x, y: Integer): GrColor; asmname 'GrPixelC';

procedure GrFloodFill(x, y: Integer; Border, c: GrColor); asmname 'GrFloodFill';

function GrGetScanline(x1,x2,yy: Integer): GrColorsPtr; asmname 'GrGetScanline';
function GrGetScanlineC(ctx: GrContextPtr; x1,x2,yy: Integer): GrColorsPtr; asmname 'GrGetScanlineC';
{ Input   ctx: source context, if NULL the current context is used }
{         x1 : first x coordinate read                             }
{         x2 : last  x coordinate read                             }
{         yy : y coordinate                                        }
{ Output  nil : error / no data (clipping occurred)                }
{         else                                                     }
{           p[0..w]: pixel values read                             }
{                      (w = |x2-y1|)                               }
{           Output data is valid until next GRX call !             }

procedure GrPutScanline(x1,x2,yy: Integer;c: GrColorsPtr; op: GrColor); asmname 'GrPutScanline';
{ Input   x1: first x coordinate to be set                         }
{         x2: last  x coordinate to be set                         }
{         yy: y coordinate                                         }
{         c : c[0..(|x2-x1|] hold the pixel data                   }
{         op: Operation (GrWRITE/GrXOR/GrOR/GrAND/GrIMAGE)         }
{                                                                  }
{ Note    c[..] data must fit GrCVALUEMASK otherwise the results   }
{         are implementation dependend.                            }
{         => You can't supply operation code with the pixel data!  }


{ ==================================================================
        NON CLIPPING DRAWING PRIMITIVES
  ================================================================== }

procedure GrPlotNC(x, y: Integer; c: GrColor); asmname 'GrPlotNC';
procedure GrLineNC(x1, y1, x2, y2: Integer; c: GrColor); asmname 'GrLineNC';
procedure GrHLineNC(x1, x2, y: Integer; c: GrColor); asmname 'GrHLineNC';
procedure GrVLineNC(x, y1, y2: Integer; c: GrColor); asmname 'GrVLineNC';
procedure GrBoxNC(x1, y1, x2, y2: Integer; c: GrColor); asmname 'GrBoxNC';
procedure GrFilledBoxNC(x1, y1, x2, y2: Integer; c: GrColor); asmname 'GrFilledBoxNC';
procedure GrFramedBoxNC(x1, y1, x2, y2, Wdt: Integer; protected var c: GrFBoxColors); asmname 'GrFramedBoxNC';
procedure GrBitBltNC(Dest: GrContextPtr; x, y: Integer; Src: GrContextPtr; x1, y1, x2, y2: Integer; op: GrColor); asmname 'GrBitBltNC';
function  GrPixelNC(x, y:Integer):GrColor; asmname 'GrPixelNC';
function  GrPixelCNC(c: GrContextPtr; x, y: Integer): GrColor; asmname 'GrPixelCNC';

{ ==================================================================
           FONTS AND TEXT PRIMITIVES
  ================================================================== }

const
  { text drawing directions }
  Gr_Text_Right      = 0;  { normal }
  Gr_Text_Down       = 1;  { downward }
  Gr_Text_Left       = 2;  { upside down, right to left }
  Gr_Text_Up         = 3;  { upward }
  Gr_Text_Default    = Gr_Text_Right;
  { Gr_Text_Is_Vertical(d) = ((d) and 1); }

  { text alignment options }
  Gr_Align_Left      = 0;  { X only }
  Gr_Align_Top       = 0;  { Y only }
  Gr_Align_Center    = 1;  { X, Y   }
  Gr_Align_Right     = 2;  { X only }
  Gr_Align_Bottom    = 2;  { Y only }
  Gr_Align_Baseline  = 3;  { Y only }
  Gr_Align_Default   = Gr_Align_Left;

  { character types in text strings }
  Gr_Byte_Text    = 0;  { one byte per character }
  Gr_Word_Text    = 1;  { two bytes per character }
  Gr_Attr_Text    = 2;  { chr w/ PC style attribute byte }

  { OR this to the foreground color value for underlined text when
    using Gr_Byte_Text or Gr_Word_Text modes. }
  Gr_Underline_Text  =  GrXor shl 4;

  { Font conversion flags for 'GrLoadConvertedFont'. OR them as desired. }
  Gr_FontCvt_None       =  0;  { no conversion }
  Gr_FontCvt_SkipChars  =  1;  { load only selected characters }
  Gr_FontCvt_Resize     =  2;  { resize the font }
  Gr_FontCvt_Italicize  =  4;  { tilt font for "italic" look }
  Gr_FontCvt_Boldify    =  8;  { make a "bold"(er) font  }
  Gr_FontCvt_Fixify     = 16;  { convert prop. font to fixed wdt }
  Gr_FontCvt_Proportion = 32;  { convert fixed font to prop. wdt }

{ font structures }
type
  { typedef struct _GR_fontHeader }  { font descriptor }
  GrFontHeader = record
    Name        : CString;  { font name }
    Family      : CString;  { font family name }
    Proportional: Char;     { characters have varying width }
    Scalable    : Char;     { derived from a scalable font }
    Preloaded   : Char;     { set when linked Integero program }
    Modified    : Char;     { "tweaked" font (resized, etc..) }
    Width       : Integer;  { width (proportional=>average) }
    Height      : Integer;  { font height }
    Baseline    : Integer;  { baseline pixel pos (from top) }
    ULPos       : Integer;  { underline pixel pos (from top) }
    ULHeight    : Integer;  { underline width }
    MinChar     : Integer;  { lowest character code in font }
    NumChars    : Integer;  { number of characters in font }
  end;

  { typedef struct _GR_fontChrInfo } { character descriptor }
  GrFontChrInfo = record
    Width : Integer;  { width of this character }
    Offset: Integer;  { offset from start of bitmap }
  end;

  { typedef struct _GR_font } { the complete font }
  AuxoffsType = array [1 .. 7] of Integer;
  GrFont = record
    h       : GrFontHeader;  { the font info structure }
    BitMap  : MemPtr;        { character bitmap array }
    AuxMap  : MemPtr;        { map for rotated & underline chrs }
    MinWidth: Integer;       { width of narrowest character }
    MaxWidth: Integer;       { width of widest character }
    AuxSize : Integer;       { allocated size of auxiliary map }
    AuxNext : Integer;       { next free byte in auxiliary map }
    AuxOffs : ^AuxoffsType;  { offsets to completed aux chars }
    ChrInfo : array [1 .. 1] of GrFontChrInfo; { character info (not act. size) }
  end;

  GrFontPtr = ^GrFont;

var
  GrFont_PC6x8 : GrFont; varasmname 'GrFont_PC6x8';
  GrFont_PC8x8 : GrFont; varasmname 'GrFont_PC8x8';
  GrFont_PC8x14: GrFont; varasmname 'GrFont_PC8x14';
  GrFont_PC8x16: GrFont; varasmname 'GrFont_PC8x16';
  GrDefaultFont: GrFont; varasmname 'GrFont_PC8x14';

function GrLoadFont(FontName: CString): GrFontPtr; asmname 'GrLoadFont';
function GrLoadConvertedFont(FontName: CString; cvt, w, h, MinCh, MaxCh: Integer): GrFontPtr; asmname 'GrLoadConvertedFont';
function GrBuildConvertedFont(protected var From: GrFont; cvt, w, h, MinCh, MaxCh: Integer): GrFontPtr; asmname 'GrBuildConvertedFont';

procedure GrUnloadFont(var Font: GrFont); asmname 'GrUnloadFont';
procedure GrDumpFont(protected var f: GrFont; CsymbolName, FileName: CString); asmname 'GrDumpFont';
procedure GrSetFontPath(path_list: CString); asmname 'GrSetFontPath';

function  GrFontCharPresent(protected var Font: GrFont; Chr: Integer): Boolean; asmname 'GrFontCharPresent';
function  GrFontCharWidth(protected var Font: GrFont; Chr: Integer): Integer; asmname 'GrFontCharWidth';
function  GrFontCharHeight(protected var Font: GrFont; Chr: Integer): Integer; asmname 'GrFontCharHeight';
function  GrFontCharBmpRowSize(protected var Font: GrFont; Chr: Integer): Integer; asmname 'GrFontCharBmpRowSize';
function  GrFontCharBitmapSize(protected var Font: GrFont; Chr: Integer): Integer; asmname 'GrFontCharBitmapSize';
function  GrFontStringWidth(protected var Font: GrFont; Text: CString; Len, Typ_e: Integer): Integer; asmname 'GrFontStringWidth';
function  GrFontStringHeight(protected var Font: GrFont; Text: CString; Len, Typ_e: Integer): Integer; asmname 'GrFontStringHeight';
function  GrProportionalTextWidth(protected var Font: GrFont; Text: CString; Len, Typ_e: Integer): Integer; asmname 'GrProportionalTextWidth';
function  GrBuildAuxiliaryBitmap(var Font: GrFont; Chr, Dir, ul: Integer): MemPtr; asmname 'GrBuildAuxiliaryBitmap';
function  GrFontCharBitmap(protected var Font: GrFont; Chr: Integer): MemPtr; asmname 'GrFontCharBitmap';
function  GrFontCharAuxBmp(var Font: GrFont; Chr, Dir, ul: Integer): MemPtr; asmname 'GrFontCharAuxBmp';

type
  GrColorTableP = ^GrColor;

  { typedef union _GR_textColor }   { text color union }
  GrTextColor = record
  case 1..2 of
    1: (v: GrColor);           { color value for "direct" text }
    2: (p: GrColorTableP);     { color table for attribute text }
  end;

  { typedef struct _GR_textOption } { text drawing option structure }
  GrTextOption = record
    txo_Font   : GrFontPtr;    { font to be used }
    txo_FgColor: GrTextColor;  { foreground color }
    txo_BgColor: GrTextColor;  { background color }
    txo_ChrType: ByteCard;     { character type (see above) }
    txo_Direct : ByteCard;     { direction (see above) }
    txo_XAlign : ByteCard;     { X alignment (see above) }
    txo_YAlign : ByteCard;     { Y alignment (see above) }
  end;

  { typedef struct }        { fixed font text window desc. }
  GrTextRegion = record
    txr_Font      : GrTextColor; { font to be used }
    txr_FgColor   : GrTextColor; { foreground color }
    txr_BgColor   : GrTextColor; { background color }
    txr_Buffer    : Pointer;     { pointer to text buffer }
    txr_Backup    : Pointer;     { optional backup buffer }
    txr_Width     : Integer;     { width of area in chars }
    txr_Height    : Integer;     { height of area in chars }
    txr_LineOffset: Integer;     { offset in buffer(s) between rows }
    txr_XPos      : Integer;     { upper left corner X coordinate }
    txr_YPos      : Integer;     { upper left corner Y coordinate }
    txr_ChrType   : Char;        { character type (see above) }
  end;

function  GrCharWidth(Chr: Integer; protected var Opt: GrTextOption): Integer; asmname 'GrCharWidth';
function  GrCharHeight(Chr: Integer; protected var Opt: GrTextOption): Integer; asmname 'GrCharHeight';
procedure GrCharSize(Chr: Integer; protected var Opt: GrTextOption; var w, h: Integer); asmname 'GrCharSize';
function  GrStringWidth(Text: CString; Length: Integer; protected var Opt: GrTextOption): Integer; asmname 'GrStringWidth';
function  GrStringHeight(Text: CString; Length: Integer; protected var Opt: GrTextOption): Integer; asmname 'GrStringHeight';
procedure GrStringSize(Text: CString; Length: Integer; protected var Opt: GrTextOption;var w, h: Integer); asmname 'GrStringSize';

procedure GrDrawChar(Chr, x, y: Integer; protected var Opt: GrTextOption); asmname 'GrDrawChar';
procedure GrDrawString(Text: CString; Length, x, y: Integer; protected var Opt: GrTextOption); asmname 'GrDrawString';
procedure GrTextXY(x, y: Integer; Text: CString; fg, bg: GrColor); asmname 'GrTextXY';

procedure GrDumpChar(Chr, Col, Row: Integer; protected var r: GrTextRegion); asmname 'GrDumpChar';
procedure GrDumpText(Col, Row, Wdt, Hgt: Integer; protected var r: GrTextRegion); asmname 'GrDumpText';
procedure GrDumpTextRegion(protected var r: GrTextRegion); asmname 'GrDumpTextRegion';

{ =================================================================
        THICK AND DASHED LINE DRAWING PRIMITIVES
  ================================================================== }

{ custom line option structure
  zero or one dash pattern length means the line is continuous
  the dash pattern always begins with a drawn section }

type
  GrLineOption = record
   No_Color   : GrColor;   { color used to draw line }
   Lno_Width  : Integer;   { width of the line }
   Lno_PattLen: Integer;   { length of the dash pattern }
   Lno_DashPat: ^ByteCard; { draw/nodraw pattern }
  end;

procedure GrCustomLine(x1, y1, x2, y2: Integer; protected var o: GrLineOption); asmname 'GrCustomLine';
procedure GrCustomBox(x1, y1, x2, y2: Integer; protected var o: GrLineOption); asmname 'GrCustomBox';
procedure GrCustomCircle(xc, yc, r: Integer; protected var o: GrLineOption); asmname 'GrCustomCircle';
procedure GrCustomEllipse(xc, yc, r: Integer; protected var o: GrLineOption); asmname 'GrCustomEllipse';
procedure GrCustomCircleArc(xc, yc, r, Start, Ende, Style: Integer; protected var o: GrLineOption); asmname 'GrCustomCircleArc';
procedure GrCustomEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; protected var o: GrLineOption); asmname 'GrCustomEllipseArc';
procedure GrCustomPolyLine(NumPts: Integer; var Points{ : array of PointType }; protected var o: GrLineOption); asmname 'GrCustomPolyLine';
procedure GrCustomPolygon(NumPts: Integer; var Points{ : array of PointType }; protected var o: GrLineOption); asmname 'GrCustomPolygon';

{ ==================================================================
           PATTERNED DRAWING AND FILLING PRIMITIVES
  ================================================================== }

{ BITMAP: a mode independent way to specify a fill pattern of two
  colors. It is always 8 pixels wide (1 byte per scan line), its
  height is user-defined. SET THE TYPE FLAG TO ZERO!!! }

type
  { typedef struct _GR_bitmap }
  GrBitmap = record
    bmp_IsPixMap: Integer;      { type flag for pattern union }
    bmp_Height  : Integer;      { bitmap height }
    bmp_Data    : ^ByteCard;    { pointer to the bit pattern }
    bmp_FgColor : GrColor;      { foreground color for fill }
    bmp_BgColor : GrColor;      { background color for fill }
    bmp_MemFlags: Integer;      { set if dynamically allocated }
  end;

  { PIXMAP: a fill pattern stored in a layout identical to the video RAM
      for filling using 'bitblt'-s. It is mode dependent, typically one
      of the library functions is used to build it. KEEP THE TYPE FLAG
      NONZERO!!! }

  { typedef struct _GR_pixmap }
  GrPixmap = record
    pxp_IsPixMap: Integer;     { type flag for pattern union }
    pxp_Width   : Integer;     { pixmap width (in pixels)  }
    pxp_Height  : Integer;     { pixmap height (in pixels) }
    pxp_Oper    : GrColor;     { bitblt mode (SET, OR, XOR, AND, IMAGE) }
    pxp_Source  : GrFrameType; { source context for fill }
  end;

  { Fill pattern union -- can either be a bitmap or a pixmap }

  { typedef union _GR_pattern }
  GrPattern = record
    gp_IsPixMap: Integer;   { nonzero for pixmaps }
    gp_BitMap  : GrBitmap;  { fill bitmap }
    gp_PixMap  : GrPixmap;  { fill pixmap }
  end;
  GrPatternPtr = ^GrPattern;

  { Draw pattern for line drawings -- specifies both the:
      (1) fill pattern, and the
      (2) custom line drawing option }
  GrLinePattern = record
    lnp_Pattern: GrPatternPtr;   { fill pattern }
    lnp_Option : ^GrLineOption;  { width + dash pattern }
  end;
  GrLinePatternPtr = GrLinePattern;

function  GrBuildPixmap(protected var Pixels; w, h: Integer; protected var Colors { : array of GrColor }): GrPatternPtr; asmname 'GrBuildPixmap';
function  GrBuildPixmapFromBits(protected var Bits; w, h: Integer; fgc, bgc: GrColor): GrPatternPtr; asmname 'GrBuildPixmapFromBits';
function  GrConvertToPixmap(Src: GrContextPtr): GrPatternPtr; asmname 'GrConvertToPixmap';

procedure GrDestroyPattern(p: GrPatternPtr); asmname 'GrDestroyPattern';

procedure GrPatternedLine(x1, y1, x2, y2: Integer; lp: GrLinePatternPtr); asmname 'GrPatternedLine';
procedure GrPatternedBox(x1, y1, x2, y2: Integer; lp: GrLinePatternPtr); asmname 'GrPatternedBox';
procedure GrPatternedCircle(xc, yc, r: Integer; lp: GrLinePatternPtr); asmname 'GrPatternedCircle';
procedure GrPatternedEllipse(xc, yc, xa, ya: Integer; lp: GrLinePatternPtr); asmname 'GrPatternedEllipse';
procedure GrPatternedCircleArc(xc, yc, r, Start, Ende, Style: Integer; lp: GrLinePatternPtr); asmname 'GrPatternedCircleArc';
procedure GrPatternedEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; lp: GrLinePatternPtr); asmname 'GrPatternedEllipseArc';
procedure GrPatternedPolyLine(NumPts: Integer; var Points{ : array of PointType }; lp: GrLinePatternPtr); asmname 'GrPatternedPolyLine';
procedure GrPatternedPolygon(NumPts: Integer; var Points{ : array of PointType }; lp: GrLinePatternPtr); asmname 'GrPatternedPolygon';

procedure GrPatternFilledPlot(x, y: Integer; p: GrPatternPtr); asmname 'GrPatternFilledPlot';
procedure GrPatternFilledLine(x1, y1, x2, y2: Integer; p: GrPatternPtr); asmname 'GrPatternFilledLine';
procedure GrPatternFilledBox(x1, y1, x2, y2: Integer; p: GrPatternPtr); asmname 'GrPatternFilledBox';
procedure GrPatternFilledCircle(xc, yc, r: Integer; p: GrPatternPtr); asmname 'GrPatternFilledCircle';
procedure GrPatternFilledEllipse(xc, yc, xa, ya: Integer; p: GrPatternPtr); asmname 'GrPatternFilledEllipse';
procedure GrPatternFilledCircleArc(xc, yc, r, Start, Ende, Style: Integer; p: GrPatternPtr); asmname 'GrPatternFilledCircleArc';
procedure GrPatternFilledEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; p: GrPatternPtr); asmname 'GrPatternFilledEllipseArc';
procedure GrPatternFilledConvexPolygon(NumPts: Integer; var Points{ : array of PointType }; p: GrPatternPtr); asmname 'GrPatternFilledConvexPolygon';
procedure GrPatternFilledPolygon(NumPts: Integer; var Points{ : array of PointType }; p: GrPatternPtr); asmname 'GrPatternFilledPolygon';
procedure GrPatternFloodFill(x, y: Integer; Border: GrColor; p: GrPatternPtr); asmname 'GrPatternFloodFill';

procedure GrPatternDrawChar(Chr, x, y: Integer; protected var Opt: GrTextOption; p: GrPatternPtr); asmname 'GrPatternDrawChar';
procedure GrPatternDrawString(Text: CString; Length, x, y: Integer; protected var Opt: GrTextOption; p: GrPatternPtr); asmname 'GrPatternDrawString';
procedure GrPatternDrawStringExt(Text: CString; Length, x, y: Integer; protected var Opt: GrTextOption; p: GrPatternPtr); asmname 'GrPatternDrawStringExt';


{ <image.h>   - Image Utility
                by Michal Stencl Copyright (c) 1998 for GRX
  <e-mail>    - [stenclpmd@ba.telecom.sk] }

type
  { Original GRX decl:
    GrImage = GrPixmap;
    GrImagePtr = ^GrImage;
       Pascal isn't case sensitive so GrImage conficts with GrIMAGE above.
       Since we only need GrImagePtr we take a short cut here: }
  GrImagePtr = ^GrPixmap;

{ Flags for GrImageInverse() }
const
  Gr_Image_Inverse_LR = 1;  { inverse left right }
  Gr_Image_Inverse_TD = 2;  { inverse top down   }

function  GrImageBuild(protected var Pixels; w, h: Integer; protected var Colors { : array of GrColor }): GrImagePtr; asmname 'GrImageBuild';
procedure GrImageDestroy(i: GrImagePtr); asmname 'GrImageDestroy';
procedure GrImageDisplay(x,y: Integer; i: GrImagePtr); asmname 'GrImageDisplay';
procedure GrImageDisplayExt(x1,y1,x2,y2: Integer; i: GrImagePtr); asmname 'GrImageDisplayExt';
procedure GrImageFilledBoxAlign(xo,yo,x1,y1,x2,y2: Integer; p: GrImagePtr); asmname 'GrImageFilledBoxAlign';
procedure GrImageHLineAlign(xo,yo,x,y,Width: Integer; p: GrImagePtr); asmname 'GrImageHLineAlign';
procedure GrImagePlotAlign(xo,yo,x,y: Integer; p: GrImagePtr); asmname 'GrImagePlotAlign';

function  GrImageInverse(p: GrImagePtr; Flag: Integer): GrImagePtr; asmname 'GrImageInverse';
function  GrImageStretch(p: GrImagePtr; NWidth, NHeight: Integer): GrImagePtr; asmname 'GrImageStretch';

function  GrImageFromPattern(p: GrPatternPtr): GrImagePtr; asmname 'GrImageFromPattern';
function  GrImageFromContext(c: GrContextPtr): GrImagePtr; asmname 'GrImageFromContext';
function  GrImageBuildUsedAsPattern(protected var Pixels; w, h: Integer; protected var Colors { : array of GrColor }): GrImagePtr; asmname 'GrImageBuildUsedAsPattern';

function  GrPatternFromImage(i: GrImagePtr): GrPatternPtr; asmname 'GrPatternFromImage';

{ ==================================================================
                 DRAWING IN USER WINDOW COORDINATES
  ================================================================== }

procedure GrSetUserWindow(x1, y1, x2, y2: Integer); asmname 'GrSetUserWindow';
procedure GrGetUserWindow(var x1, y1, x2, y2: Integer); asmname 'GrGetUserWindow';
procedure GrGetScreenCoord(var x, y: Integer); asmname 'GrGetScreenCoord';
procedure GrGetUserCoord(var x, y: Integer); asmname 'GrGetUserCoord';

procedure GrUsrPlot(x, y: Integer; c:GrColor); asmname 'GrUsrPlot';
procedure GrUsrLine(x1, y1, x2, y2: Integer; c:GrColor); asmname 'GrUsrLine';
procedure GrUsrHLine(x1, x2, y:Integer; c:GrColor); asmname 'GrUsrHLine';
procedure GrUsrVLine(x, y1, y2: Integer; c: GrColor); asmname 'GrUsrVLine';
procedure GrUsrBox(x1, y1, x2, y2: Integer; c: GrColor); asmname 'GrUsrBox';
procedure GrUsrFilledBox(x1, y1, x2, y2: Integer; c: GrColor); asmname 'GrUsrFilledBox';
procedure GrUsrFramedBox(x1, y1, x2, y2, Wdt: Integer; c: GrFBoxColors); asmname 'GrUsrFramedBox';

procedure GrUsrCircle(xc, yc, r: Integer; c: GrColor); asmname 'GrUsrCircle';
procedure GrUsrEllipse(xc, yc, xa, ya: Integer; c: GrColor); asmname 'GrUsrEllipse';
procedure GrUsrCircleArc(xc, yc, r, Start, Ende, Style: Integer; c: GrColor); asmname 'GrUsrCircleArc';
procedure GrUsrEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; c: GrColor); asmname 'GrUsrEllipseArc';
procedure GrUsrFilledCircle(xc, yc, r: Integer; c: GrColor); asmname 'GrUsrFilledCircle';
procedure GrUsrFilledEllipse(xc, yc, xa, ya: Integer; c: GrColor); asmname 'GrUsrFilledEllipse';
procedure GrUsrFilledCircleArc(xc, yc, r, Start, Ende, Style: Integer; c: GrColor); asmname 'GrUsrFilledCircleArc';
procedure GrUsrFilledEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; c: GrColor); asmname 'GrUsrFilledEllipseArc';
procedure GrUsrPolyLine(NumPts: Integer; var Points{ : array of PointType }; c: GrColor); asmname 'GrUsrPolyLine';
procedure GrUsrPolygon(NumPts: Integer; var Points{ : array of PointType }; c: GrColor); asmname 'GrUsrPolygon';
procedure GrUsrFilledConvexPolygon(NumPts: Integer; var Points{ : array of PointType }; c: GrColor); asmname 'GrUsrFilledConvexPolygon';
procedure GrUsrFilledPolygon(NumPts: Integer; var Points{ : array of PointType }; c: GrColor); asmname 'GrUsrFilledPolygon';
procedure GrUsrFloodFill(x, y: Integer; Border, c: GrColor); asmname 'GrUsrFloodFill';

function  GrUsrPixel(x, y:Integer):GrColor; asmname 'GrUsrPixel';
function  GrUsrPixelC(c: GrContextPtr; x, y: Integer):GrColor; asmname 'GrUsrPixelC';

procedure GrUsrCustomLine(x1, y1, x2, y2: Integer; protected var o: GrLineOption); asmname 'GrUsrCustomLine';
procedure GrUsrCustomBox(x1, y1, x2, y2: Integer; protected var o: GrLineOption); asmname 'GrUsrCustomBox';
procedure GrUsrCustomCircle(xc, yc, r: Integer; protected var o: GrLineOption); asmname 'GrUsrCustomCircle';
procedure GrUsrCustomEllipse(xc, yc, r: Integer; protected var o: GrLineOption); asmname 'GrUsrCustomEllipse';
procedure GrUsrCustomCircleArc(xc, yc, r, Start, Ende, Style: Integer; protected var o: GrLineOption); asmname 'GrUsrCustomCircleArc';
procedure GrUsrCustomEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; protected var o: GrLineOption); asmname 'GrUsrCustomEllipseArc';
procedure GrUsrCustomPolyLine(NumPts: Integer; var Points{ : array of PointType }; protected var o: GrLineOption); asmname 'GrUsrCustomPolyLine';
procedure GrUsrCustomPolygon(NumPts: Integer; var Points{ : array of PointType }; protected var o: GrLineOption); asmname 'GrUsrCustomPolygon';

procedure GrUsrPatternedLine(x1, y1, x2, y2: Integer; lp: GrLinePatternPtr); asmname 'GrUsrPatternedLine';
procedure GrUsrPatternedBox(x1, y1, x2, y2: Integer; lp: GrLinePatternPtr); asmname 'GrUsrPatternedBox';
procedure GrUsrPatternedCircle(xc, yc, r: Integer; lp: GrLinePatternPtr); asmname 'GrUsrPatternedCircle';
procedure GrUsrPatternedEllipse(xc, yc, xa, ya: Integer; lp: GrLinePatternPtr); asmname 'GrUsrPatternedEllipse';
procedure GrUsrPatternedCircleArc(xc, yc, r, Start, Ende, Style: Integer; lp: GrLinePatternPtr); asmname 'GrUsrPatternedCircleArc';
procedure GrUsrPatternedEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; lp: GrLinePatternPtr); asmname 'GrUsrPatternedEllipseArc';
procedure GrUsrPatternedPolyLine(NumPts: Integer; var Points{ : array of PointType }; lp: GrLinePatternPtr); asmname 'GrUsrPatternedPolyLine';
procedure GrUsrPatternedPolygon(NumPts: Integer; var Points{ : array of PointType }; lp: GrLinePatternPtr); asmname 'GrUsrPatternedPolygon';

procedure GrUsrPatternFilledPlot(x, y: Integer; p: GrPatternPtr); asmname 'GrPatternFilledPlot';
procedure GrUsrPatternFilledLine(x1, y1, x2, y2: Integer; p: GrPatternPtr); asmname 'GrUsrPatternFilledLine';
procedure GrUsrPatternFilledBox(x1, y1, x2, y2: Integer; p: GrPatternPtr); asmname 'GrUsrPatternFilledBox';
procedure GrUsrPatternFilledCircle(xc, yc, r: Integer; p: GrPatternPtr); asmname 'GrUsrPatternFilledCircle';
procedure GrUsrPatternFilledEllipse(xc, yc, xa, ya: Integer; p: GrPatternPtr); asmname 'GrUsrPatternFilledEllipse';
procedure GrUsrPatternFilledCircleArc(xc, yc, r, Start, Ende, Style: Integer; p: GrPatternPtr); asmname 'GrUsrPatternFilledCircleArc';
procedure GrUsrPatternFilledEllipseArc(xc, yc, xa, ya, Start, Ende, Style: Integer; p: GrPatternPtr); asmname 'GrUsrPatternFilledEllipseArc';
procedure GrUsrPatternFilledConvexPolygon(NumPts: Integer; var Points{ : array of PointType }; p: GrPatternPtr); asmname 'GrUsrPatternFilledConvexPolygon';
procedure GrUsrPatternFilledPolygon(NumPts: Integer; var Points{ : array of PointType }; p: GrPatternPtr); asmname 'GrUsrPatternFilledPolygon';
procedure GrUsrPatternFloodFill(x, y: Integer; Border: GrColor; p: GrPatternPtr); asmname 'GrUsrPatternFloodFill';

procedure GrUsrDrawChar(Chr, x, y: Integer; protected var Opt: GrTextOption); asmname 'GrUsrDrawChar';
procedure GrUsrDrawString(Text: CString; Length, x, y: Integer; protected var Opt: GrTextOption); asmname 'GrUsrDrawString';
procedure GrUsrTextXY(x, y: Integer; Text: CString; fg, bg: Integer); asmname 'GrUsrTextXY';

{ ==================================================================
                  GRAPHICS CURSOR UTILITIES
  ================================================================== }

type
  { typedef struct _GR_cursor }
  GrCursor = record
    Work       : GrContext;  { work areas (4) }
    XCord,YCord: Integer;    { cursor position on screen }
    XSize,YSize: Integer;    { cursor size }
    XOffs,YOffs: Integer;    { LU corner to hot point offset }
    XWork,YWork: Integer;    { save/work area sizes }
    XWPos,YWPos: Integer;    { save/work area position on screen }
    Displayed  : Integer;    { set if displayed }
  end;
  GrCursorPtr = ^GrCursor;

function  GrBuildCursor(Pixels: Pointer; Pitch, w, h, xo, yo: Integer; protected var Colors { : array of GrColor }): GrCursorPtr; asmname 'GrBuildCursor';
procedure GrDestroyCursor(Cursor: GrCursorPtr); asmname 'GrDestroyCursor';
procedure GrDisplayCursor(Cursor: GrCursorPtr); asmname 'GrDisplayCursor';
procedure GrEraseCursor(Cursor: GrCursorPtr); asmname 'GrEraseCursor';
procedure GrMoveCursor(Cursor: GrCursorPtr; x, y: Integer); asmname 'GrMoveCursor';

{ ==================================================================
       MOUSE AND KEYBOARD INPUT UTILITIES
  ================================================================== }

const
  Gr_M_Motion        = $0001;      { mouse event flag bits }
  Gr_M_Left_Down     = $0002;
  Gr_M_Left_Up       = $0004;
  Gr_M_Right_Down    = $0008;
  Gr_M_Right_Up      = $0010;
  Gr_M_Middle_Down   = $0020;
  Gr_M_Middle_Up     = $0040;
  Gr_M_Button_Down   = Gr_M_Left_Down or Gr_M_Middle_Down or Gr_M_Right_Down;
  Gr_M_Button_Up     = Gr_M_Left_Up   or Gr_M_Middle_Up   or Gr_M_Right_Up;
  Gr_M_Button_Change = Gr_M_Button_Up or Gr_M_Button_Down;

  Gr_M_Left          = 1;          { mouse button index bits }
  Gr_M_Right         = 2;
  Gr_M_Middle        = 4;

  Gr_M_KeyPress      = $0080;      { other event flag bits }
  Gr_M_Poll          = $0100;
  Gr_M_NoPaint       = $0200;
  Gr_Size_Changed    = $0400;
  Gr_M_Event         = Gr_M_Motion or Gr_M_KeyPress or Gr_M_Button_Down or Gr_M_Button_Up or Gr_Size_Changed;

  Gr_KB_RightShift   = $01;    { Keybd states: right shift key depressed }
  Gr_KB_LeftShift    = $02;    { left shift key depressed }
  Gr_KB_Ctrl         = $04;    { CTRL depressed }
  Gr_KB_Alt          = $08;    { ALT depressed }
  Gr_KB_ScrolLock    = $10;    { SCROLL LOCK active }
  Gr_KB_NumLock      = $20;    { NUM LOCK active }
  Gr_KB_CapsLock     = $40;    { CAPS LOCK active }
  Gr_KB_Insert       = $80;    { INSERT state active }
  Gr_KB_Shift        = Gr_KB_LeftShift or Gr_KB_RightShift;

  Gr_M_Cur_Normal    = 0;       { MOUSE CURSOR modes: just the cursor }
  Gr_M_Cur_Rubber    = 1;       { rectangular rubber band (XOR-d to the screen) }
  Gr_M_Cur_Line      = 2;       { line attached to the cursor }
  Gr_M_Cur_Box       = 3;       { rectangular box dragged by the cursor }
  Gr_M_Cur_Cross     = 4;       { crosshair }

  Gr_M_Queue_Size    = 128;     { default queue size }

type
  { typedef struct _GR_mouseEvent }         { mouse event buffer structure }
  GrMouseEvent = record
    Flags  : Integer;           { event type flags (see above) }
    x, y   : Integer;           { mouse coordinates }
    Buttons: Integer;           { mouse button state }
    Key    : Integer;           { key code from keyboard }
    KBStat : Integer;           { keybd status (ALT, CTRL, etc..) }
    DTime  : Integer;           { time since last event (msec) }
  end;
  GrMouseEventPtr = ^GrMouseEvent;

function  GrMouseDetect: Boolean; asmname 'GrMouseDetect';
procedure GrMouseEventMode(Dummy: Integer); asmname 'GrMouseEventMode';
procedure GrMouseInit; asmname 'GrMouseInit';
procedure GrMouseInitN(Queue_Size: Integer); asmname 'GrMouseInitN';
procedure GrMouseUnInit; asmname 'GrMouseUnInit';
procedure GrMouseSetSpeed(SPMult, SPDiv: Integer); asmname 'GrMouseSetSpeed';
procedure GrMouseSetAccel(Thresh, Accel: Integer); asmname 'GrMouseSetAccel';
procedure GrMouseSetLimits(x1, y1, x2, y2: Integer); asmname 'GrMouseSetLimits';
procedure GrMouseGetLimits(var x1, y1, x2, y2: Integer); asmname 'GrMouseGetLimits';
procedure GrMouseWarp(x, y: Integer); asmname 'GrMouseWarp';
procedure GrMouseEventEnable(Enable_KB, Enable_MS: Boolean); asmname 'GrMouseEventEnable';
procedure GrMouseGetEvent(Flags: Integer; Event: GrMouseEventPtr); asmname 'GrMouseGetEvent';
procedure GrMouseGetEventT(Flags: Integer; Event: GrMouseEventPtr; timout_msecs: Integer); asmname 'GrMouseGetEventT';
function  GrMousePendingEvent: Integer; asmname 'GrMousePendingEvent';

function  GrMouseGetCursor: GrCursorPtr; asmname 'GrMouseGetCursor';
procedure GrMouseSetCursor(Cursor: GrCursorPtr); asmname 'GrMouseSetCursor';
procedure GrMouseSetColors(fg,bg: GrColor); asmname 'GrMouseSetColors';
procedure GrMouseSetCursorMode(Mode: Integer; x1, y1, x2, y2: Integer; c: Integer); asmname 'GrMouseSetCursorMode';
procedure GrMouseDisplayCursor; asmname 'GrMouseDisplayCursor';
procedure GrMouseEraseCursor; asmname 'GrMouseEraseCursor';
procedure GrMouseUpdateCursor; asmname 'GrMouseUpdateCursor';
function  GrMouseCursorIsDisplayed: Boolean; asmname 'GrMouseCursorIsDisplayed';

function  GrMouseBlock(c: GrContextPtr; x1, y1, x2, y2: Integer): Integer; asmname 'GrMouseBlock';
procedure GrMouseUnBlock(Return_Value_From_GrMouseBlock: Integer); asmname 'GrMouseUnBlock';

{ ==================================================================
                         KEYBOARD INTERFACE
  ================================================================== }

const
  GrKey_NoKey              = $0000;  { no key available }
  GrKey_OutsideValidRange  = $0100;  { key typed but code outside 1 .. GrKey_LastDefinedKeycode }

  { standard ASCII key codes }
  GrKey_Control_A          = $0001;
  GrKey_Control_B          = $0002;
  GrKey_Control_C          = $0003;
  GrKey_Control_D          = $0004;
  GrKey_Control_E          = $0005;
  GrKey_Control_F          = $0006;
  GrKey_Control_G          = $0007;
  GrKey_Control_H          = $0008;
  GrKey_Control_I          = $0009;
  GrKey_Control_J          = $000a;
  GrKey_Control_K          = $000b;
  GrKey_Control_L          = $000c;
  GrKey_Control_M          = $000d;
  GrKey_Control_N          = $000e;
  GrKey_Control_O          = $000f;
  GrKey_Control_P          = $0010;
  GrKey_Control_Q          = $0011;
  GrKey_Control_R          = $0012;
  GrKey_Control_S          = $0013;
  GrKey_Control_T          = $0014;
  GrKey_Control_U          = $0015;
  GrKey_Control_V          = $0016;
  GrKey_Control_W          = $0017;
  GrKey_Control_X          = $0018;
  GrKey_Control_Y          = $0019;
  GrKey_Control_Z          = $001a;
  GrKey_Control_LBracket   = $001b;
  GrKey_Control_BackSlash  = $001c;
  GrKey_Control_RBracket   = $001d;
  GrKey_Control_Caret      = $001e;
  GrKey_Control_Underscore = $001f;
  GrKey_Space              = $0020;
  GrKey_ExclamationPoint   = $0021;
  GrKey_DoubleQuote        = $0022;
  GrKey_Hash               = $0023;
  GrKey_Dollar             = $0024;
  GrKey_Percent            = $0025;
  GrKey_Ampersand          = $0026;
  GrKey_Quote              = $0027;
  GrKey_LParen             = $0028;
  GrKey_RParen             = $0029;
  GrKey_Star               = $002a;
  GrKey_Plus               = $002b;
  GrKey_Comma              = $002c;
  GrKey_Dash               = $002d;
  GrKey_Period             = $002e;
  GrKey_Slash              = $002f;
  GrKey_0                  = $0030;
  GrKey_1                  = $0031;
  GrKey_2                  = $0032;
  GrKey_3                  = $0033;
  GrKey_4                  = $0034;
  GrKey_5                  = $0035;
  GrKey_6                  = $0036;
  GrKey_7                  = $0037;
  GrKey_8                  = $0038;
  GrKey_9                  = $0039;
  GrKey_Colon              = $003a;
  GrKey_SemiColon          = $003b;
  GrKey_LAngle             = $003c;
  GrKey_Equals             = $003d;
  GrKey_RAngle             = $003e;
  GrKey_QuestionMark       = $003f;
  GrKey_At                 = $0040;
  GrKey_A                  = $0041;
  GrKey_B                  = $0042;
  GrKey_C                  = $0043;
  GrKey_D                  = $0044;
  GrKey_E                  = $0045;
  GrKey_F                  = $0046;
  GrKey_G                  = $0047;
  GrKey_H                  = $0048;
  GrKey_I                  = $0049;
  GrKey_J                  = $004a;
  GrKey_K                  = $004b;
  GrKey_L                  = $004c;
  GrKey_M                  = $004d;
  GrKey_N                  = $004e;
  GrKey_O                  = $004f;
  GrKey_P                  = $0050;
  GrKey_Q                  = $0051;
  GrKey_R                  = $0052;
  GrKey_S                  = $0053;
  GrKey_T                  = $0054;
  GrKey_U                  = $0055;
  GrKey_V                  = $0056;
  GrKey_W                  = $0057;
  GrKey_X                  = $0058;
  GrKey_Y                  = $0059;
  GrKey_Z                  = $005a;
  GrKey_LBracket           = $005b;
  GrKey_BackSlash          = $005c;
  GrKey_RBracket           = $005d;
  GrKey_Caret              = $005e;
  GrKey_UnderScore         = $005f;
  GrKey_BackQuote          = $0060;
  GrKey_Small_a            = $0061;
  GrKey_Small_b            = $0062;
  GrKey_Small_c            = $0063;
  GrKey_Small_d            = $0064;
  GrKey_Small_e            = $0065;
  GrKey_Small_f            = $0066;
  GrKey_Small_g            = $0067;
  GrKey_Small_h            = $0068;
  GrKey_Small_i            = $0069;
  GrKey_Small_j            = $006a;
  GrKey_Small_k            = $006b;
  GrKey_Small_l            = $006c;
  GrKey_Small_m            = $006d;
  GrKey_Small_n            = $006e;
  GrKey_Small_o            = $006f;
  GrKey_Small_p            = $0070;
  GrKey_Small_q            = $0071;
  GrKey_Small_r            = $0072;
  GrKey_Small_s            = $0073;
  GrKey_Small_t            = $0074;
  GrKey_Small_u            = $0075;
  GrKey_Small_v            = $0076;
  GrKey_Small_w            = $0077;
  GrKey_Small_x            = $0078;
  GrKey_Small_y            = $0079;
  GrKey_Small_z            = $007a;
  GrKey_LBrace             = $007b;
  GrKey_Pipe               = $007c;
  GrKey_RBrace             = $007d;
  GrKey_Tilde              = $007e;
  GrKey_Control_Backspace  = $007f;

  { extended key codes as defined in DJGPP }
  GrKey_Alt_Escape         = $0101;
  GrKey_Control_At         = $0103;
  GrKey_Alt_Backspace      = $010e;
  GrKey_BackTab            = $010f;
  GrKey_Alt_Q              = $0110;
  GrKey_Alt_W              = $0111;
  GrKey_Alt_E              = $0112;
  GrKey_Alt_R              = $0113;
  GrKey_Alt_T              = $0114;
  GrKey_Alt_Y              = $0115;
  GrKey_Alt_U              = $0116;
  GrKey_Alt_I              = $0117;
  GrKey_Alt_O              = $0118;
  GrKey_Alt_P              = $0119;
  GrKey_Alt_LBracket       = $011a;
  GrKey_Alt_RBracket       = $011b;
  GrKey_Alt_Return         = $011c;
  GrKey_Alt_A              = $011e;
  GrKey_Alt_S              = $011f;
  GrKey_Alt_D              = $0120;
  GrKey_Alt_F              = $0121;
  GrKey_Alt_G              = $0122;
  GrKey_Alt_H              = $0123;
  GrKey_Alt_J              = $0124;
  GrKey_Alt_K              = $0125;
  GrKey_Alt_L              = $0126;
  GrKey_Alt_Semicolon      = $0127;
  GrKey_Alt_Quote          = $0128;
  GrKey_Alt_Backquote      = $0129;
  GrKey_Alt_Backslash      = $012b;
  GrKey_Alt_Z              = $012c;
  GrKey_Alt_X              = $012d;
  GrKey_Alt_C              = $012e;
  GrKey_Alt_V              = $012f;
  GrKey_Alt_B              = $0130;
  GrKey_Alt_N              = $0131;
  GrKey_Alt_M              = $0132;
  GrKey_Alt_Comma          = $0133;
  GrKey_Alt_Period         = $0134;
  GrKey_Alt_Slash          = $0135;
  GrKey_Alt_KPStar         = $0137;
  GrKey_F1                 = $013b;
  GrKey_F2                 = $013c;
  GrKey_F3                 = $013d;
  GrKey_F4                 = $013e;
  GrKey_F5                 = $013f;
  GrKey_F6                 = $0140;
  GrKey_F7                 = $0141;
  GrKey_F8                 = $0142;
  GrKey_F9                 = $0143;
  GrKey_F10                = $0144;
  GrKey_Home               = $0147;
  GrKey_Up                 = $0148;
  GrKey_PageUp             = $0149;
  GrKey_Alt_KPMinus        = $014a;
  GrKey_Left               = $014b;
  GrKey_Center             = $014c;
  GrKey_Right              = $014d;
  GrKey_Alt_KPPlus         = $014e;
  GrKey_End                = $014f;
  GrKey_Down               = $0150;
  GrKey_PageDown           = $0151;
  GrKey_Insert             = $0152;
  GrKey_Delete             = $0153;
  GrKey_Shift_F1           = $0154;
  GrKey_Shift_F2           = $0155;
  GrKey_Shift_F3           = $0156;
  GrKey_Shift_F4           = $0157;
  GrKey_Shift_F5           = $0158;
  GrKey_Shift_F6           = $0159;
  GrKey_Shift_F7           = $015a;
  GrKey_Shift_F8           = $015b;
  GrKey_Shift_F9           = $015c;
  GrKey_Shift_F10          = $015d;
  GrKey_Control_F1         = $015e;
  GrKey_Control_F2         = $015f;
  GrKey_Control_F3         = $0160;
  GrKey_Control_F4         = $0161;
  GrKey_Control_F5         = $0162;
  GrKey_Control_F6         = $0163;
  GrKey_Control_F7         = $0164;
  GrKey_Control_F8         = $0165;
  GrKey_Control_F9         = $0166;
  GrKey_Control_F10        = $0167;
  GrKey_Alt_F1             = $0168;
  GrKey_Alt_F2             = $0169;
  GrKey_Alt_F3             = $016a;
  GrKey_Alt_F4             = $016b;
  GrKey_Alt_F5             = $016c;
  GrKey_Alt_F6             = $016d;
  GrKey_Alt_F7             = $016e;
  GrKey_Alt_F8             = $016f;
  GrKey_Alt_F9             = $0170;
  GrKey_Alt_F10            = $0171;
  GrKey_Control_Print      = $0172;
  GrKey_Control_Left       = $0173;
  GrKey_Control_Right      = $0174;
  GrKey_Control_End        = $0175;
  GrKey_Control_PageDown   = $0176;
  GrKey_Control_Home       = $0177;
  GrKey_Alt_1              = $0178;
  GrKey_Alt_2              = $0179;
  GrKey_Alt_3              = $017a;
  GrKey_Alt_4              = $017b;
  GrKey_Alt_5              = $017c;
  GrKey_Alt_6              = $017d;
  GrKey_Alt_7              = $017e;
  GrKey_Alt_8              = $017f;
  GrKey_Alt_9              = $0180;
  GrKey_Alt_0              = $0181;
  GrKey_Alt_Dash           = $0182;
  GrKey_Alt_Equals         = $0183;
  GrKey_Control_PageUp     = $0184;
  GrKey_F11                = $0185;
  GrKey_F12                = $0186;
  GrKey_Shift_F11          = $0187;
  GrKey_Shift_F12          = $0188;
  GrKey_Control_F11        = $0189;
  GrKey_Control_F12        = $018a;
  GrKey_Alt_F11            = $018b;
  GrKey_Alt_F12            = $018c;
  GrKey_Control_Up         = $018d;
  GrKey_Control_KPDash     = $018e;
  GrKey_Control_Center     = $018f;
  GrKey_Control_KPPlus     = $0190;
  GrKey_Control_Down       = $0191;
  GrKey_Control_Insert     = $0192;
  GrKey_Control_Delete     = $0193;
  GrKey_Control_Tab        = $0194;
  GrKey_Control_KPSlash    = $0195;
  GrKey_Control_KPStar     = $0196;
  GrKey_Alt_KPSlash        = $01a4;
  GrKey_Alt_Tab            = $01a5;
  GrKey_Alt_Enter          = $01a6;

  { some additional codes not in DJGPP }
  GrKey_Alt_LAngle         = $01b0;
  GrKey_Alt_RAngle         = $01b1;
  GrKey_Alt_At             = $01b2;
  GrKey_Alt_LBrace         = $01b3;
  GrKey_Alt_Pipe           = $01b4;
  GrKey_Alt_RBrace         = $01b5;
  GrKey_Print              = $01b6;
  GrKey_Shift_Insert       = $01b7;
  GrKey_Shift_Home         = $01b8;
  GrKey_Shift_End          = $01b9;
  GrKey_Shift_PageUp       = $01ba;
  GrKey_Shift_PageDown     = $01bb;
  GrKey_Alt_Up             = $01bc;
  GrKey_Alt_Left           = $01bd;
  GrKey_Alt_Center         = $01be;
  GrKey_Alt_Right          = $01c0;
  GrKey_Alt_Down           = $01c1;
  GrKey_Alt_Insert         = $01c2;
  GrKey_Alt_Delete         = $01c3;
  GrKey_Alt_Home           = $01c4;
  GrKey_Alt_End            = $01c5;
  GrKey_Alt_PageUp         = $01c6;
  GrKey_Alt_PageDown       = $01c7;
  GrKey_Shift_Up           = $01c8;
  GrKey_Shift_Down         = $01c9;
  GrKey_Shift_Right        = $01ca;
  GrKey_Shift_Left         = $01cb;

  { this may be usefull for table allocation ... }
  GrKey_LastDefinedKeycode = GrKey_Shift_Left;

  { some well known synomyms }
  GrKey_BackSpace          = GrKey_Control_H;
  GrKey_Tab                = GrKey_Control_I;
  GrKey_LineFeed           = GrKey_Control_J;
  GrKey_Escape             = GrKey_Control_LBracket;
  GrKey_Return             = GrKey_Control_M;

type
  GrKeyType = ShortInt;

{ new functions to replace the old style
    kbhit / getch / getkey / getxkey / getkbstat
  keyboard interface }
function GrKeyPressed: Integer; asmname 'GrKeyPressed';
function GrKeyRead: GrKeyType; asmname 'GrKeyRead';
function GrKeyStat: Integer; asmname 'GrKeyStat';

{ ==================================================================
                MISCELLANEOUS UTILITIY FUNCTIONS
  ================================================================== }

procedure GrResizeGrayMap (var Map; Pitch, ow, oh, nw, nh: Integer); asmname 'GrResizeGrayMap';
function  GrMatchString (Pattern, Strg: CString): Integer; asmname 'GrMatchString';
procedure GrSetWindowTitle (Title: CString); asmname 'GrSetWindowTitle';
procedure GrSleep (MSec: Integer); asmname 'GrSleep';

{ ==================================================================
                            PNM FUNCTIONS
  ================================================================== }

{ The PNM formats, grx support load/save of binaries formats (4,5,6) only }

const
  PlainPBMFormat = 1;
  PlainPGMFormat = 2;
  PlainPPMFormat = 3;
  PBMFormat      = 4;
  PGMFormat      = 5;
  PPMFormat      = 6;

{ The PNM functions }

function GrSaveContextToPbm (grc: GrContextPtr; FileName, Comment: CString): Integer; asmname 'GrSaveContextToPbm';
function GrSaveContextToPgm (grc: GrContextPtr; FileName, Comment: CString): Integer; asmname 'GrSaveContextToPgm';
function GrSaveContextToPpm (grc: GrContextPtr; FileName, Comment: CString): Integer; asmname 'GrSaveContextToPpm';
function GrLoadContextFromPnm (grc: GrContextPtr; FileName: CString): Integer; asmname 'GrLoadContextFromPnm';
function GrQueryPnm (FileName: CString; var Width, Height, MaxVal: Integer): Integer; asmname 'GrQueryPnm';
function GrLoadContextFromPnmBuffer (grc: GrContextPtr; protected var Buffer): Integer; asmname 'GrLoadContextFromPnmBuffer';
function GrQueryPnmBuffer (protected var Buffer; var Width, Height, MaxVal: Integer): Integer; asmname 'GrQueryPnmBuffer';

{ ==================================================================
                             ADDON FUNCTIONS
   these functions may not be installed or available on all systems
  ================================================================== }

function GrPngSupport: Integer; asmname 'GrPngSupport';
function GrSaveContextToPng (grc: GrContextPtr; FileName: CString): Integer; asmname 'GrSaveContextToPng';
function GrLoadContextFromPng (grc: GrContextPtr; FileName: CString; UseAlpha: Integer): Integer; asmname 'GrLoadContextFromPng';
function GrQueryPng (FileName: CString; var Width, Height: Integer): Integer; asmname 'GrQueryPng';

{ SaveContextToTiff - Dump a context in a TIFF file

  Arguments:
    cxt:   Context to be saved (nil -> use current context)
    tiffn: Name of tiff file
    compr: Compression method (see tiff.h), 0: automatic selection
    docn:  string saved in the tiff file (DOCUMENTNAME tag)

   Returns  0 on success
           -1 on error

  requires tifflib by  Sam Leffler (sam@engr.sgi.com)
         available at  ftp://ftp.sgi.com/graphics/tiff }
function SaveContextToTiff(Cxt: GrContextPtr; TiffN: CString; Compr: Integer; DocN: CString): Integer; asmname 'SaveContextToTiff';

{ GrSaveContextToJpeg - Dump a context in a JPEG file

  Arguments:
    grc:      Context to be saved (nil -> use current context)
    Filename: Name of the jpeg file
    accuracy: Accuracy percentage (100 for no loss of quality)

   Returns  0 on success
           -1 on error

  requires jpeg-6a by  IJG (Independent JPEG Group)
         available at  ftp.uu.net as graphics/jpeg/jpegsrc.v6a.tar.gz }
function GrSaveContextToJpeg(grc: GrContextPtr; FileName: CString; Accuracy: Integer): Integer; asmname 'GrSaveContextToJpeg';
function GrSaveContextToGrayJpeg(grc: GrContextPtr; FileName: CString; Accuracy: Integer): Integer; asmname 'GrSaveContextToGrayJpeg';
function GrJpegSupport: Integer; asmname 'GrJpegSupport';
function GrLoadContextFromJpeg(grc: GrContextPtr; FileName: CString; Scale: Integer): Integer; asmname 'GrLoadContextFromJpeg';
function GrQueryJpeg(FileName: CString; var Width, Height: Integer): Integer; asmname 'GrQueryJpeg';

implementation

end.
