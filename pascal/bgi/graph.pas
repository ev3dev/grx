{ Graph  -  Interfacing BGI based graphics programs to LIBGRX
  Copyright (C) 96 by Sven Hilscher

  This file is part of the GRX graphics library.

  The GRX graphics library is free software; you can redistribute it
  and/or modify it under some conditions; see the "copying.grx" file
  for details.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  Author: Sven Hilscher
  e-mail: sven@rufus.lan-ks.de

  Version 1.0 12/96

  22/02/01 patches by Maurice Lombardi <Maurice.Lombardi@ujf-grenoble.fr>
  Necessary to correct the drawpoly and fillpoly declarations.
  Some changes to make this file compile with more recent versions of
  gpc (change in declaration of external variables, removal of now
  invalid __soso__  declarations). }

{ Define this if you don't want this unit to declare a KeyPressed
  and a ReadKey function. }
{.$define NO_GRAPH_KEY_FUNCTIONS}

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

unit Graph;

interface

{$if __GPC_RELEASE__ >= 20030303}
{$define asmname external name}
{$define varasmname external name}
{$else}
{$define varasmname external; asmname}
{$endif}

type
  PByte     = ^Byte;
  WrkString = String[255];

const
  GrOk             =  0;
  GrNoInitGraph    = -1;
  GrNotDetected    = -2;
  GrFileNotFound   = -3;
  GrInvalidDriver  = -4;
  GrNoLoadMem      = -5;
  GrNoScanMem      = -6;
  GrNoFloodMem     = -7;
  GrFontNotFound   = -8;
  GrNoFontMem      = -9;
  GrInvalidMode    = -10;
  GrError          = -11;
  GrIOError        = -12;
  GrInvalidFont    = -13;
  GrInvalidFontNum = -14;
  GrInvalidVersion = -18;

  Detect           = 0;
  Native_GRX       = -3;
  CurrentDriver    = -128;
  VGA              = 9;
  EGA              = 3;
  IBM8514          = 6;
  HercMono         = 7;
  EGA64            = 4;
  EGAMono          = 5;
  CGA              = 1;
  MCGA             = 2;
  ATT400           = 8;
  PC3270           = 10;

  { driver definitions from BC++ 4.5: }
  DetectX          = Detect;
  VGA256           = 11;
  ATTDEB           = 12;
  Toshiba          = 13;
  SVGA16           = 14;
  SVGA256          = 15;
  SVGA32K          = 16;
  SVGA64K          = 17;
  VESA16           = 18;
  VESA256          = 19;
  VESA32K          = 20;
  VESA64K          = 21;
  VESA16M          = 22;
  ATI16            = 23;
  ATI256           = 24;
  ATI32K           = 25;
  Compaq           = 26;
  Tseng316         = 27;
  Tseng3256        = 28;
  Tseng416         = 29;
  Tseng4256        = 30;
  Tseng432K        = 31;
  Genoa5           = 32;
  Genoa6           = 33;
  OAK              = 34;
  Paradis16        = 35;
  Paradis256       = 36;
  Tecmar           = 37;
  Trident16        = 38;
  Trident256       = 39;
  Video7           = 40;
  Video7ii         = 41;
  S3               = 42;
  ATIGUP           = 43;

  VGALo            =  0;
  VGAMed           =  1;
  VGAHi            =  2;
  IBM8514Lo        =  0;
  IBM8514Hi        =  1;
  HercMonoHi       =  0;
  CGAC0            =  0;
  CGAC1            =  1;
  CGAC2            =  2;
  CGAC3            =  3;
  CGAHi            =  4;
  MCGAC0           =  CGAC0;
  MCGAC1           =  CGAC1;
  MCGAC2           =  CGAC2;
  MCGAC3           =  CGAC3;
  MCGAMed          =  CGAHi;
  MCGAHi           =  5;
  ATT400C0         =  MCGAC0;
  ATT400C1         =  MCGAC1;
  ATT400C2         =  MCGAC2;
  ATT400C3         =  MCGAC3;
  ATT400Med        =  MCGAMed;
  ATT400Hi         =  MCGAHi;
  EGA64Lo          =  0;
  EGA64Hi          =  1;
  EGALo            =  0;
  EGAHi            =  1;
  EGAMonoHi        =  3;
  PC3270Hi         =  0;

  { mode definitions from BC++ 4.5: }
  Res640x350       =  0;
  Res640x480       =  1;
  Res800x600       =  2;
  Res1024x768      =  3;
  Res1280x1024     =  4;

  { NATIVE_GRX modes: }
  GRX_Default_Graphics               = 0;
  GRX_Biggest_Noninterlaced_Graphics = 1;
  GRX_Biggest_Graphics               = 2;
  GRX_BGI_Emulation                  = 3;
  First_Driver_Specific_Mode         = 4;

  EGABlack        =  0;
  EGABlue         =  1;
  EGAGreen        =  2;
  EGACyan         =  3;
  EGARed          =  4;
  EGAMagenta      =  5;
  EGABrown        = 20;
  EGALightGray    =  7;
  EGADarkGray     = 56;
  EGALightBlue    = 57;
  EGALightGreen   = 58;
  EGALightCyan    = 59;
  EGALightRed     = 60;
  EGALightMagenta = 61;
  EGAYellow       = 62;
  EGAWhite        = 63;

  SolidLn         =  0;
  DottedLn        =  1;
  CenterLn        =  2;
  DashedLn        =  3;
  UserBitLn       =  4;

  NormWidth       =  1;
  ThickWidth      =  3;

  DefaultFont     =  0;  { 8x8 bit mapped font }
  TriplexFont     =  1;
  SmallFont       =  2;
  SansSerifFont   =  3;
  GothicFont      =  4;
  ScriptFont      =  5;
  SimplexFont     =  6;
  TriplexScrFont  =  7;
  ComplexFont     =  8;
  EuropeanFont    =  9;
  BoldFont        =  10;

  HorizDir        =  0;  { left to right }
  VertDir         =  1;  { bottom to top }

  UserCharSize    =  0;  { user-defined char size }

  NormalPut       =  0;  { copy }
  CopyPut         =  0;  { copy }
  XORPut          =  1;  { xor }
  OrPut           =  2;  { or  }
  AndPut          =  3;  { and }
  NotPut          =  4;  { not }

  LeftText        =  0;
  CenterText      =  1;
  RightText       =  2;
  BottomText      =  0;
  TopText         =  2;

  MaxColors       = 15;

  { Clipping }
  ClipOn          = True;
  ClipOff         = False;

  { Bar3D }
  TopOn           = True;
  TopOff          = False;

  { FillStyles }
  EmptyFill       =  0;  { fills area in background color }
  SolidFill       =  1;  { fills area in solid fill color }
  LineFill        =  2;  { --- fill }
  LtSlashFill     =  3;  { /// fill }
  SlashFill       =  4;  { /// fill with thick lines }
  BkSlashFill     =  5;  { \\\ fill with thick lines }
  LtBkSlashFill   =  6;  { \\\ fill }
  HatchFill       =  7;  { light hatch fill }
  XHatchFill      =  8;  { heavy cross hatch fill }
  InterleaveFill  =  9;  { interleaving line fill }
  WideDotFill     = 10;  { Widely spaced dot fill }
  CloseDotFill    = 11;  { Closely spaced dot fill }
  UserFill        = 12;  { user defined fill }

type
  PaletteType = record
    Size  : Byte;
    Colors: array [0 .. MaxColors] of Byte;
  end;

  LineSettingsType = record
    LineStyle: Integer;
    UPattern : ShortCard;
    Thickness: Integer;
  end;

  TextSettingsType = record
    Font     : Integer;
    Direction: Integer;
    CharSize : Integer;
    Horiz    : Integer;
    Vert     : Integer;
   end;

  FillSettingsType = record
    Pattern: Integer;
    Color  : Integer;
  end;

  FillPatternType = array [1 .. 8] of Byte;

  { This definition is compatible with the grx
    definition `int pts[][2]' used to define polygons }
  PointType = record
    x, y: Integer;
  end;

  ViewPortType = record
    x1, y1, x2, y2: Integer;
    Clip: WordBool;
  end;

  ArcCoordsType = record
    x, y:Integer;
    XStart, YStart, XEnd, YEnd: Integer;
  end;

var
  GraphGetMemPtr : Pointer;  { Dummy! }
  GraphFreeMemPtr: Pointer;  { Dummy! }

{ BGI - API definitions }

procedure DetectGraph(var GraphDriver, GraphMode: Integer); asmname 'detectgraph';
procedure InitGraph(var GraphDriver, GraphMode: Integer; PathToDriver: CString); asmname 'initgraph';
procedure SetGraphMode(Mode: Integer); asmname 'setgraphmode';
function  GetModeName(ModeNumber: Integer): WrkString;
procedure GraphDefaults; asmname 'graphdefaults';
function  GetDriverName: WrkString;
function  GraphErrorMsg(ErrorCode: Integer): WrkString;
function  GetMaxX: Integer; asmname 'getmaxx';
function  GetMaxY: Integer; asmname 'getmaxy';
function  GetMaxColor: Integer; asmname 'getmaxcolor';
procedure GetViewSettings(var ViewPort: ViewPortType); asmname 'getviewsettings';
procedure SetViewPort(Left, Top, Right, Bottom: Integer; Clip: Boolean); asmname 'setviewport';
procedure GetLineSettings(var LineInfo: LineSettingsType); asmname 'getlinesettings';
procedure SetLineStyle(LineStyle: Integer; Pattern: Integer; Thickness: Integer); asmname 'setlinestyle';
procedure ClearViewPort; asmname 'clearviewport';
function  GetPixel(x,y: Integer): Integer; asmname 'getpixel';
procedure PutPixel(x, y: Integer; Pixel: Integer); asmname 'putpixel';
procedure Bar3D(Left, Top, Right, Bottom: Integer; Depth: Integer; TopFlag: Boolean); asmname 'bar';
procedure Rectangle(Left, Top, Right, Bottom: Integer); asmname 'rectangle';
procedure FillPoly(NumPoints: Word; var PolyPoints { : array of PointType }); asmname 'fillpoly';
procedure FillEllipse(x, y: Integer; XRadius, YRadius: Integer); asmname 'fillellipse';
procedure GetArcCoords(var ArcCoords: ArcCoordsType); asmname 'getarccoords';
procedure FloodFill(x, y: Integer; Border: Integer); asmname 'floodfill';
procedure SetFillPattern(UPattern: FillPatternType; Color: Integer); asmname 'setfillpattern';
procedure SetFillStyle(Pattern: Integer; Color: Integer); asmname 'setfillstyle';
procedure GetImage(Left, Top, Right, Bottom: Integer; var BitMap); asmname 'getimage';
procedure PutImage(Left, Top: Integer; var BitMap; Op: Integer); asmname 'putimage';
function  ImageSize(Left, Top, Right, Bottom: Integer): Integer; asmname 'imagesize';
procedure GetTextSettings(var TextTypeInfo: TextSettingsType); asmname 'gettextsettings';
procedure SetTextJustify(Horiz, Vert: Integer); asmname 'settextjustify';
procedure SetTextStyle(Font, Direction: Integer; CharSize: Integer); asmname 'settextstyle';
procedure SetRGBPalette(Color, Red, Green, Blue: Integer); asmname 'setrgbpalette';
procedure SetUserCharSize(MultX, DivX, MultY, DivY: Integer); asmname 'setusercharsize';
procedure SetWriteMode(Mode: Integer); asmname 'setwritemode';
procedure OutText(TextString: CString); asmname 'outtext';
procedure OutTextXY(x, y: Integer; TextString: CString); asmname 'outtextxy';
function  TextHeight(TextString: CString): Integer; asmname 'textheight';
function  TextWidth(TextString: CString): Integer; asmname 'textwidth';

function  RegisterBGIfont(Font: Pointer): Integer; asmname 'registerbgifont';
function  InstallUserFont(FontName: CString): Integer; asmname 'installuserfont';

function  GetPaletteSize: Integer; asmname 'getpalettesize';
procedure GetPalette(var Palette: PaletteType); asmname 'getpalette';
procedure SetPalette(ColorNum: Word; Color: Integer); asmname '__gr_setpalette';
procedure SetAllPalette(protected var Palette: PaletteType); asmname 'setallpalette';

procedure RestoreCrtMode; asmname '__gr_restorecrtmode';
procedure CloseGraph; asmname '__gr_closegraph';
procedure SetColor(Color: Integer); asmname '__gr_setcolor';
procedure SetBkColor(Color: Integer); asmname '__gr_setbkcolor';
procedure Bar(Left, Top, Right, Bottom: Integer); asmname '__gr_bar';
function  GraphResult:Integer; asmname '__gr_graphresult';
procedure Line(x1, y1, x2, y2: Integer); asmname '__gr_line';
procedure MoveTo(x, y: Integer); asmname '__gr_moveto';
procedure Arc(x, y: Integer; StAngle, EndAngle, Radius: Word); asmname '__gr_arc';
procedure Circle(x, y: Integer; Radius: Word); asmname '__gr_circle';
procedure ClearDevice; asmname '__gr_cleardevice';
procedure DrawPoly(NumPoints: Word; var PolyPoints { : array of PointType }); asmname '__gr_drawpoly';
procedure Ellipse(x, y: Integer; StAngle, EndAngle: Word; XRadius, YRadius: Word); asmname '__gr_ellipse';
procedure GetAspectRatio(var XAsp, YAsp: Integer); asmname '__gr_getaspectratio';
function  GetBkColor: Word; asmname '__gr_getbkcolor';
function  GetColor: Word; asmname '__gr_getcolor';
procedure GetFillPattern(var FillPattern: FillPatternType); asmname '__gr_getfillpattern';
procedure GetFillSettings(var FillInfo: FillSettingsType); asmname '__gr_getfillsettings';
function  GetMaxMode: Integer; asmname '__gr_getmaxmode';
function  GetGraphMode: Integer; asmname '__gr_getgraphmode';
function  GetX: Integer; asmname '__gr_getx';
function  GetY: Integer; asmname '__gr_gety';
function  InstallUserDriver(const DriverName: String; AutoDetectPtr: Pointer):Integer;
procedure LineRel(dx, dy: Integer); asmname '__gr_linerel';
procedure LineTo(x, y: Integer); asmname '__gr_lineto';
procedure MoveRel(dx, dy: Integer); asmname '__gr_moverel';
procedure PieSlice(x, y: Integer; StAngle, EndAngle, Radius: Word); asmname '__gr_pieslice';
function  RegisterBGIdriver(Driver: Pointer): Integer;
procedure Sector(x, y: Integer; StAngle,EndAngle, XRadius, YRadius: Word); asmname '__gr_sector';
procedure SetAspectRatio(XAsp, YAsp: Integer); asmname '__gr_setaspectratio';
procedure SetGraphBufSize(BufSize: Word); asmname '__gr_setgraphbufsize';
procedure SetActivePage(Page: Word); asmname '__gr_setactivepage';
procedure SetVisualPage(Page: Word); asmname '__gr_setvisualpage';
procedure GetDefaultPalette(var Palette: PaletteType);
procedure GetModeRange(GraphDriver:Integer; var LoMode, HiMode:Integer); asmname '__gr_getmoderange';

function  Black       : Integer;
function  Blue        : Integer;
function  Green       : Integer;
function  Cyan        : Integer;
function  Red         : Integer;
function  Magenta     : Integer;
function  Brown       : Integer;
function  LightGray   : Integer;
function  DarkGray    : Integer;
function  LightBlue   : Integer;
function  LightGreen  : Integer;
function  LightCyan   : Integer;
function  LightRed    : Integer;
function  LightMagenta: Integer;
function  Yellow      : Integer;
function  White       : Integer;

{ BGI - API extensions }

{ Linkable font files }
var
  Bold_Font: PByte; varasmname '_bold_font';
  Euro_Font: PByte; varasmname '_euro_font';
  Goth_Font: PByte; varasmname '_goth_font';
  Lcom_Font: PByte; varasmname '_lcom_font';
  Litt_Font: PByte; varasmname '_litt_font';
  Sans_Font: PByte; varasmname '_sans_font';
  Scri_Font: PByte; varasmname '_scri_font';
  Simp_Font: PByte; varasmname '_simp_font';
  Trip_Font: PByte; varasmname '_trip_font';
  Tscr_Font: PByte; varasmname '_tscr_font';


{ Translates BGI driver/mode into a driver/mode pair for this unit
  for usage with InitGraph / SetGraphMode }
procedure SetBGImode(var GDrv, GMode: Integer); asmname 'set_BGI_mode';

{ Determines a driver/mode pair for InitGraph that will set
  up a graphics mode with the desired resolution and colors }
procedure SetBGImodeWHC(var GDrv, GMode: Integer; Width, Height, Colors: Integer); asmname '__gr_set_BGI_mode_whc';

{ enable multiple graphics pages by
    SetBGIModePages (2);
    InitGraph (gd, gm, '');
    if (GraphResult = grOk) and (GetBGIModePages=2) then PlayWithPages; }
procedure SetBGIModePages(p: Integer); asmname '__gr_set_BGI_mode_pages';
function  GetBGIModePages: Integer; asmname '__gr_get_BGI_mode_pages';

{ like GetMaxColor, GetMaxX, GetMaxY but for any available mode }
function  GetModeMaxColor(Mode: Integer): Integer; asmname '__gr_getmodemaxcolor';
function  GetModeMaxX(Mode: Integer): Integer; asmname '__gr_getmodemaxx';
function  GetModeMaxY(Mode: Integer): Integer; asmname '__gr_getmodemaxy';

{ Set the actual drawing color to r/g/b.
  Functional in HiColor/TrueColor modes only }
procedure SetRGBColor(Red, Green, Blue: Word); asmname '__gr_setrgbcolor';

{ Returns the actual RGB palette. Functional in
  256 color modes only. Won't work on Linux systems }
procedure GetRGBPalette(Color: Integer; var Red, Green, Blue: Integer); asmname '__getrgbpalette';

{ Transforms the predefined 16 EGA colors (RED, BLUE, MAGENTA, ...)
  into the correct color value for the active graphics mode }
function EGAColor(EGACol: Integer): Integer; asmname '_ega_color';

{ Enable/disable linestyle drawing of vector fonts.
  Currently only functional with GRX v1.x }
procedure TextLineStyle(On: Boolean); asmname '__gr_textlinestyle';

{ Counterparts of SetActivePage/SetVisualPage }
function  GetActivePage: Integer; asmname '__gr_getactivepage';
function  GetVisualPage: Integer; asmname '__gr_getvisualpage';

{$ifndef NO_GRAPH_KEY_FUNCTIONS}
function KeyPressed: Boolean; asmname 'kbhit';
function ReadKey: Char; asmname 'getch';
{$endif}

implementation

function gem(ec: Integer): CString; asmname 'grapherrormsg';
function GraphErrorMsg(ErrorCode: Integer): WrkString;
begin
  GraphErrorMsg := CString2String(gem(ErrorCode));
end;

function gmn(mn: Integer): CString; asmname 'getmodename';
function GetModeName(ModeNumber: Integer): WrkString;
begin
  GetModeName := CString2String(gmn(ModeNumber));
end;

function gdn: CString; asmname 'getdrivername';
function GetDriverName: WrkString;
begin
  GetDriverName := CString2String(gdn);
end;

type pPT = ^PaletteType;
function gdp: pPT; asmname '__gr_getdefaultpalette';
procedure GetDefaultPalette(var Palette: PaletteType);
begin
 Palette := gdp^;
end;

function InstallUserDriver(const DriverName: String; AutoDetectPtr: Pointer):Integer;
var
  Dummy1: Integer;
  Dummy2: Pointer;
begin
  Dummy1 := Length (DriverName);
  Dummy2 := AutoDetectPtr;
  InstallUserDriver := GrError
end;

function RegisterBGIdriver(Driver: Pointer): Integer;
var Dummy: Pointer;
begin
  Dummy := Driver;
  RegisterBGIdriver := GrError
end;

function Black: Integer;
begin
  Black := EGAColor (0)
end;

function Blue: Integer;
begin
  Blue := EGAColor (1)
end;

function Green: Integer;
begin
  Green := EGAColor (2)
end;

function Cyan: Integer;
begin
  Cyan := EGAColor (3)
end;

function Red: Integer;
begin
  Red := EGAColor (4)
end;

function Magenta: Integer;
begin
  Magenta := EGAColor (5)
end;

function Brown: Integer;
begin
  Brown := EGAColor (6)
end;

function LightGray: Integer;
begin
  LightGray := EGAColor (7)
end;

function DarkGray: Integer;
begin
  DarkGray := EGAColor (8)
end;

function LightBlue: Integer;
begin
  LightBlue := EGAColor (9)
end;

function LightGreen: Integer;
begin
  LightGreen := EGAColor (10)
end;

function LightCyan: Integer;
begin
  LightCyan := EGAColor (11)
end;

function LightRed: Integer;
begin
  LightRed := EGAColor (12)
end;

function LightMagenta: Integer;
begin
  LightMagenta := EGAColor (13)
end;

function Yellow: Integer;
begin
  Yellow := EGAColor (14)
end;

function White: Integer;
begin
  White := EGAColor (15)
end;

end.
