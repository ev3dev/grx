Unit BGI2GRX;

{
 * BGI2GRX  -  Interfacing BGI based graphics programs to LIBGRX
 * Copyright (C) 96 by Sven Hilscher
 *
 * ==============================================================
 * This unit only works with the BCC2GRX library, now part of GRX
 * BGI2GRX is created by converting the file BCC2GRX.H
 * ==============================================================
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author : Sven Hilscher
 * e-mail : sven@rufus.central.de
 *
 * Version 1.0 12/96
 *
 * 22/02/01 patches by Maurice Lombardi <Maurice.Lombardi@ujf-grenoble.fr>
 * Necessary to correct the drawpoly and fillpoly declarations.
 * Some changes to make this file compile with more recent versions of
 * gpc (change in declaration of external variables, removal of now
 * invalid __soso__  declarations).
}

(*$ifdef __DJGPP__ *)
  (*$L grx20 *)
(*$else *)
(*$ifdef __MINGW32__ *)
  (*$L grx20 *)
(*$else *)  
(*$ifdef _WIN32 *)
  (*$L grxW32 *)
  (*$L vfs.c *)
  (*$L user32 *)
  (*$L gdi32 *)
(*$else *)
  (*$L grx20X *)
  (*$L X11 *)
(*$endif *)
(*$endif *)
(*$endif *)


Interface

Type
  PChar     = ^char;

{ Pointer   = ^Void; I hate the warnings }
  Pointer   = PChar;

  WrkString = String[255];


Const

  grOk              =  0;
  grNoInitGraph     = -1;
  grNotDetected     = -2;
  grFileNotFound    = -3;
  grInvalidDriver   = -4;
  grNoLoadMem       = -5;
  grNoScanMem       = -6;
  grNoFloodMem      = -7;
  grFontNotFound    = -8;
  grNoFontMem       = -9;
  grInvalidMode     = -10;
  grError           = -11;
  grIOerror         = -12;
  grInvalidFont     = -13;
  grInvalidFontNum  = -14;
  grInvalidVersion  = -18;

  Detect             =(-2); { in Borland compilers is Detect = 0 !!! }
  NATIVE_GRX         =(-3);
  Current_Driver     =(-1);
  VGA                =( 9);
  EGA                =( 3);
  IBM8514            =( 6);
  HERCMono           =( 7);
  EGA64              =( 4);
  EGAMono            =( 5);
  CGA                =( 1);
  MCGA               =( 2);
  ATT400             =( 8);
  PC3270            = (10);
{ driver definitions from BC++ 4.5 : }
  DETECTX           = (DETECT);
  VGA256            = (11);
  ATTDEB            = (12);
  TOSHIBA           = (13);
  SVGA16            = (14);
  SVGA256           = (15);
  SVGA32K           = (16);
  SVGA64K           = (17);
  VESA16            = (18);
  VESA256           = (19);
  VESA32K           = (20);
  VESA64K           = (21);
  VESA16M           = (22);
  ATI16             = (23);
  ATI256            = (24);
  ATI32K            = (25);
  COMPAQ            = (26);
  TSENG316          = (27);
  TSENG3256         = (28);
  TSENG416          = (29);
  TSENG4256         = (30);
  TSENG432K         = (31);
  GENOA5            = (32);
  GENOA6            = (33);
  OAK               = (34);
  PARADIS16         = (35);
  PARADIS256        = (36);
  TECMAR            = (37);
  TRIDENT16         = (38);
  TRIDENT256        = (39);
  VIDEO7            = (40);
  VIDEO7II          = (41);
  S3                = (42);
  ATIGUP            = (43);

  VGALO            =  0;
  VGAMED           =  1;
  VGAHI            =  2;
  IBM8514LO        =  0;
  IBM8514HI        =  1;
  HERCMONOHI       =  0;
  CGAC0            =  0;
  CGAC1            =  1;
  CGAC2            =  2;
  CGAC3            =  3;
  CGAHI            =  4;
  MCGAC0           =  CGAC0;
  MCGAC1           =  CGAC1;
  MCGAC2           =  CGAC2;
  MCGAC3           =  CGAC3;
  MCGAMED          =  CGAHI;
  MCGAHI           =  5;
  ATT400C0         =  MCGAC0;
  ATT400C1         =  MCGAC1;
  ATT400C2         =  MCGAC2;
  ATT400C3         =  MCGAC3;
  ATT400MED        =  MCGAMED;
  ATT400HI         =  MCGAHI;
  EGA64LO          =  0;
  EGA64HI          =  1;
  EGALO            =  0;
  EGAHI            =  1;
  EGAMONOHI        =  0;
  PC3270HI         =  0;
{ mode definitions from BC++ 4.5 : }
  RES640x350       =  0;
  RES640x480       =  1;
  RES800x600       =  2;
  RES1024x768      =  3;
  RES1280x1024     =  4;

{ NATIVE_GRX modes : }
  GRX_DEFAULT_GRAPHICS              =  0;
  GRX_BIGGEST_NONINTERLACED_GRAPHICS=  1;
  GRX_BIGGEST_GRAPHICS              =  2;
  GRX_BGI_EMULATION                 =  3;
  {$W-} { underscore warning disabled }
  __FIRST_DRIVER_SPECIFIC_MODE      =  4;
  {$W+}

  Black        =  0;
  Blue         =  1;
  Green        =  2;
  Cyan         =  3;
  Red          =  4;
  Magenta      =  5;
  Brown        =  6;
  LightGray    =  7;
  DarkGray     =  8;
  LightBlue    =  9;
  LightGreen   = 10;
  LightCyan    = 11;
  LightRed     = 12;
  LightMagenta = 13;
  Yellow       = 14;
  { White        = 15; }
  { WHITE        = (__gr_White); }

  EGABlack       =  0;
  EGABlue        =  1;
  EGAGreen       =  2;
  EGACyan        =  3;
  EGARed         =  4;
  EGAMagenta     =  5;
  EGABrown       = 20;
  EGALightgray   =  7;
  EGADarkgray    = 56;
  EGALightblue   = 57;
  EGALightgreen  = 58;
  EGALightcyan   = 59;
  EGALightred    = 60;
  EGALightmagenta= 61;
  EGAYellow      = 62;
  EGAWhite       = 63;

  SolidLn      =  0;
  DottedLn     =  1;
  CenterLn     =  2;
  DashedLn     =  3;
  UserBitLn    =  4;

  NormWidth    =  1;
  ThickWidth   =  3;

  DefaultFont     =  0; { 8x8 bit mapped font }
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

  HorizDir        =  0; { left to right }
  VertDir         =  1; { bottom to top }

  UserCharSize    =  0; { user-defined char size }

  NormalPut       =  0; { MOV }
  CopyPut         =  0; { MOV }
  XORPut          =  1; { XOR }
  OrPut           =  2; { OR  }
  AndPut          =  3; { AND }
 {NotPut          =  4;}{ NOT } {not available}

  LeftText        =  0;
  CenterText      =  1;
  RightText       =  2;
  BottomText      =  0;
  TopText         =  2;

  MaxColors   = 15;

  { Clipping }
  ClipOn      = True;
  ClipOff     = False;

  { Bar3D }
  TopOn       = True;
  TopOff      = False;

  { FillStyles }
  EmptyFill      =  0;   { fills area in background color }
  SolidFill      =  1;   { fills area in solid fill color }
  LineFill       =  2;   { --- fill }
  LtSlashFill    =  3;   { /// fill }
  SlashFill      =  4;   { /// fill with thick lines }
  BkSlashFill    =  5;   { \\\ fill with thick lines }
  LtBkSlashFill  =  6;   { \\\ fill }
  HatchFill      =  7;   { light hatch fill }
  XHatchFill     =  8;   { heavy cross hatch fill }
  InterleaveFill =  9;   { interleaving line fill }
  WideDotFill    = 10;   { Widely spaced dot fill }
  CloseDotFill   = 11;   { Closely spaced dot fill }
  UserFill       = 12;   { user defined fill }

Type

  PaletteType = record
	Size    : Byte;
	Colors  : array[0..MaxColors] of Byte;
  end;

  LineSettingsType = record
	LineStyle : Integer;
	uPattern  : ShortInt;  { ??? original Pattern }
	Thickness : Integer;
  end;

  TextSettingsType = record
	 Font      : Integer;
	 Direction : Integer;
	 CharSize  : Integer;
	 Horiz     : Integer;
	 Vert      : Integer;
   end;

  FillSettingsType = record
	 Pattern : Integer;
	 Color   : Integer;
  end;

  FillPatternType = array[1..8] of Byte;

{ This definition is compatible with the grx
   definition 'int pts[][2]' used to define polygons }

  PointType = record
	X, Y : Integer;
  end;

  ViewPortType = record
	left, top, right, bottom : Integer;
	Clip : Boolean; { ??? was Integer }
  end;

  ArcCoordsType = record
	X, Y:Integer;
	Xstart, Ystart, Xend, Yend : Integer;
  end;

{ ------------------------------------------------------------------ }
{ ---                  BGI - API definitions                     --- }

procedure DetectGraph(var GraphDriver, GraphMode: Integer); C;
procedure InitGraph(var GraphDriver:Integer; var GraphMode: Integer; PathToDriver: WrkString);
procedure SetGraphMode(Mode: Integer); C;
function  GetModeName(ModeNumber: Integer): WrkString;
procedure GraphDefaults; C;
function  GetDriverName: WrkString;
function  GraphErrorMsg(ErrorCode: Integer): WrkString;
function  GetMaxX: Integer; C;
function  GetMaxY: Integer; C;
function  GetMaxColor: Integer; C;
procedure GetViewSettings(var ViewPort: ViewPortType); C;
procedure SetViewPort(left, top, right, bottom: Integer; Clip: Boolean); C;
procedure GetLineSettings(var LineInfo: LineSettingsType); C;
procedure SetLineStyle(LineStyle: Integer; Pattern: Integer; Thickness: Integer); C;
procedure ClearViewPort; C;
function  GetPixel(X,Y: Integer): Integer; C;
procedure PutPixel(X, Y: Integer; Pixel: Integer); C;
procedure Bar3D(left, top, right, bottom: Integer; Depth: Integer; TopFlag: Boolean); C;
procedure Rectangle(left, top, right, bottom: Integer); C;
procedure FillPoly(NumPoints: Word; var PolyPoints); C;
procedure FillEllipse(X, Y: Integer; XRadius, YRadius: Integer); C;
procedure GetArcCoords(var ArcCoords: ArcCoordsType); C;
procedure FloodFill(X, Y: Integer; Border: Integer); C;
procedure SetFillPattern(uPattern: FillPatternType; Color: Integer); C;
procedure SetFillStyle(Pattern: Integer; Color: Integer); C;
procedure GetImage(left, top, right, bottom: Integer; var BitMap: Void); C;
procedure PutImage(left, top: Integer; var BitMap: Void; Op: Integer); C;
function  ImageSize(left, top, right, bottom: Integer): Integer; C;
procedure GetTextSettings(var TextTypeInfo: TextSettingsType); C;
procedure SetTextJustify(Horiz, Vert: Integer); C;
procedure SetTextStyle(Font, Direction: Integer; CharSize: Integer); C;
procedure SetRGBPalette(Color, Red, Green, Blue: Integer); C;
procedure SetUserCharSize(MultX, DivX, MultY, DivY: Integer); C;
procedure SetWriteMode(Mode: Integer); C;
procedure OutText(TextString: WrkString);
procedure OutTextXY(X,Y: Integer; TextString: WrkString);
function  TextHeight(TextString: WrkString): Integer;
function  TextWidth(TextString: WrkString): Integer;

function  RegisterBGIfont(Font: pointer): Integer; C;
function  InstallUserFont(Name: WrkString): Integer;

function  GetPaletteSize: Integer; C;
procedure GetPalette(var Palette: PaletteType); C;
procedure SetPalette(ColorNum: Word; Color: Integer); AsmName '__gr_setpalette';
procedure SetAllPalette(var Palette: PaletteType); C;

procedure RestoreCrtMode; AsmName '__gr_restorecrtmode';
procedure CloseGraph; AsmName '__gr_closegraph';
procedure SetColor(Color: Integer); AsmName '__gr_setcolor';
procedure SetBkColor(Color: Integer); AsmName '__gr_setbkcolor';
procedure Bar(left, top, right, bottom: Integer); AsmName '__gr_bar';
Function  GraphResult:Integer; AsmName '__gr_graphresult';
procedure Line(x1, y1, x2, y2: Integer); AsmName '__gr_line';
procedure MoveTo(x, y: Integer); AsmName '__gr_moveto';
procedure Arc(X, Y: Integer; StAngle, EndAngle, Radius: Word); AsmName '__gr_arc';
procedure Circle(X,Y: Integer; Radius: Word); AsmName '__gr_circle';
procedure ClearDevice; AsmName '__gr_cleardevice';
procedure DrawPoly(NumPoints: Word; var PolyPoints); AsmName '__gr_drawpoly';
procedure Ellipse(X, Y: Integer; StAngle, EndAngle: Word; XRadius, YRadius: Word); AsmName '__gr_ellipse';
procedure GetAspectRatio(var Xasp, Yasp: Integer); AsmName '__gr_getaspectratio';
function  GetBkColor: Word; AsmName '__gr_getbkcolor';
function  GetColor: Word; AsmName '__gr_getcolor';
procedure GetFillPattern(var FillPattern: FillPatternType); AsmName '__gr_getfillpattern';
procedure GetFillSettings(var FillInfo: FillSettingsType); AsmName '__gr_getfillsettings';
function  GetMaxMode: Integer; AsmName '__gr_getmaxmode';
function  GetGraphMode: Integer; AsmName '__gr_getgraphmode';
function  GetX: Integer; AsmName '__gr_getx';
function  GetY: Integer; AsmName '__gr_gety';
function  InstallUserDriver(Name: WrkString; AutoDetectPtr: pointer):integer;
procedure LineRel(Dx, Dy: Integer); AsmName '__gr_linerel';
procedure LineTo(X, Y: Integer); AsmName '__gr_lineto';
procedure MoveRel(Dx, Dy: Integer); AsmName '__gr_moverel';
procedure PieSlice(X, Y: Integer; StAngle, EndAngle, Radius: Word); AsmName '__gr_pieslice';
function  RegisterBGIdriver(driver: pointer): Integer;
procedure Sector(x, y: Integer; StAngle,EndAngle, XRadius, YRadius: Word); AsmName '__gr_sector';
procedure SetAspectRatio(Xasp, Yasp: Integer); AsmName '__gr_setaspectratio';
procedure SetGraphBufSize(BufSize: Word); AsmName '__gr_setgraphbufsize';
procedure SetActivePage(Page: Word); AsmName '__gr_setactivepage';
procedure SetVisualPage(Page: Word); AsmName '__gr_setvisualpage';
procedure GetDefaultPalette(var Palette: PaletteType);

function  White:Word; AsmName '__gr_White';

procedure GetModeRange(GraphDriver:Integer; var LoMode, HiMode:Integer); AsmName '__gr_getmoderange';

{ ------------------------------------------------------------------ }
{ ---                  BGI - API extensions                      --- }

{ Linkable font files }
var
{$if __GPC_RELEASE__ < 20000412}
  Bold_Font : __asmname__ '_bold_font' Pointer;
  Euro_Font : __asmname__ '_euro_font' Pointer;
  Goth_Font : __asmname__ '_goth_font' Pointer;
  Lcom_Font : __asmname__ '_lcom_font' Pointer;
  Litt_Font : __asmname__ '_litt_font' Pointer;
  Sans_Font : __asmname__ '_sans_font' Pointer;
  Scri_Font : __asmname__ '_scri_font' Pointer;
  Simp_Font : __asmname__ '_simp_font' Pointer;
  Trip_Font : __asmname__ '_trip_font' Pointer;
  Tscr_Font : __asmname__ '_tscr_font' Pointer;
{$else}
  Bold_Font : Pointer; AsmName '_bold_font'; external;
  Euro_Font : Pointer; AsmName '_euro_font'; external;
  Goth_Font : Pointer; AsmName '_goth_font'; external;
  Lcom_Font : Pointer; AsmName '_lcom_font'; external;
  Litt_Font : Pointer; AsmName '_litt_font'; external;
  Sans_Font : Pointer; AsmName '_sans_font'; external;
  Scri_Font : Pointer; AsmName '_scri_font'; external;
  Simp_Font : Pointer; AsmName '_simp_font'; external;
  Trip_Font : Pointer; AsmName '_trip_font'; external;
  Tscr_Font : Pointer; AsmName '_tscr_font'; external;
{$endif}


procedure SetBGImode(Var gdrv, gmode: Integer); AsmName 'set_BGI_mode';
  { Translates BGI driver/mode into BGI2GRX driver/mode pair
    for usage with InitGraph / SetGraphMode                  }

procedure SetBGImodeWHC(Var gdrv, gmode: Integer; Width, Height, Colors: Integer); AsmName '__gr_set_BGI_mode_whc';
  { Determines a driver/mode pair for InitGraph that will set
    up a graphics mode with the desired resolution and colors }

procedure SetBGImodepages(p: Integer); AsmName '__gr_set_BGI_mode_pages';
function  GetBGImodepages : Integer; AsmName '__gr_get_BGI_mode_pages';
  { enable multiple graphics pages by
      SetBGImodepages(2); InitGraph(gd,gm,'');
      if (GraphResult=grOk) and (GetBGImodepages=2) then PlayWithPages; }

function  GetModeMaxColor(Mode: Integer) : Integer; AsmName '__gr_getmodemaxcolor';
function  GetModeMaxX(Mode: Integer) : Integer; AsmName '__gr_getmodemaxx';
function  GetModeMaxY(Mode: Integer) : Integer; AsmName '__gr_getmodemaxy';
  { like GetMaxColor, GetMaxX, GetMaxY but for any available mode }

procedure SetRGBColor(R, G, B: Word); AsmName '__gr_setrgbcolor';
  { Set the actual drawing color to r/g/b.
    Functional in HiColor/TrueColor modes only }

procedure GetRBGpalette(Color: Integer; var Reg, Green, Blue: Integer); AsmName '__getrgbpalette';
  { Returns the actual RGB palette. Functional in
    256 color modes only. Won't work on Linux systems }

Function EGAcolor(egacol: Integer): Integer; AsmName '_ega_color';
  { Transforms the predefined 16 EGA colors (RED, BLUE, MAGENTA, ...)
    into the correct color value for the active graphics mode }

procedure TextLineStyle(on: Boolean); AsmName '__gr_textlinestyle';
  { Enable/disable linestyle drawing of vector fonts.
    Currently only functional with GRX v1.x          }

function  GetActivePage : Integer; AsmName '__gr_getactivepage';
function  GetVisualPage : Integer; AsmName '__gr_getvisualpage';
  { Counterparts of SetActivePage/SetVisualPage }

Implementation


{
| AFAIK there's no way to pass value parameter strings by pointer
| in GPC  ---  Define some helper functions passing pointers
}

procedure ig(var gd, gm : integer; var ptd: WrkString);
    AsmName '__gr_p_initgraph';

function th(var txt : WrkString) : Integer;
    Asmname '__gr_p_textheight';

function tw(var txt : WrkString) : Integer;
    Asmname '__gr_p_textwidth';

function iuf(var ts: WrkString): Integer;
    Asmname '__gr_p_installuserfont';

procedure ot(var ts: WrkString);
    Asmname '__gr_p_outtext';

procedure otxy(x,y: Integer; var ts: WrkString);
    Asmname '__gr_p_outtextxy';


{ Convert strings returned from C into GPC strings }
function StrPas(Src: PChar): WrkString;
var
  S : WrkString;
begin
  S := '';
  if (Src <> NIL)
  then while ( (Src^ <> chr(0)) AND (length(S) < S.capacity)) do
	   begin
		 S := S + Src^;
		 inc(Word(Src));
	   end;
  StrPas := S;
end;



procedure InitGraph(var GraphDriver:Integer; var GraphMode: Integer;
						    PathToDriver: WrkString);
begin
  ig(GraphDriver, GraphMode, PathToDriver);
end;



function TextHeight(TextString: WrkString): Integer;
begin
  TextHeight := th(TextString);
end;



function TextWidth(TextString: WrkString): Integer;
begin
  TextWidth := tw(TextString);
end;



function InstallUserFont(Name: WrkString): Integer;
begin
  InstallUserFont := iuf(Name);
end;



procedure OutText(TextString: WrkString);
begin
  ot(TextString);
end;



procedure OutTextXY(X,Y: Integer; TextString: WrkString);
begin
  otxy(X, Y, TextString);
end;



function  GraphErrorMsg(ErrorCode: Integer): WrkString;
  function gem(ec: Integer): PChar; Asmname 'grapherrormsg';
begin
  GraphErrorMsg := StrPas(gem(ErrorCode));
end;



function  GetModeName(ModeNumber: Integer): WrkString;
  function gmn(mn: Integer): PChar; Asmname 'getmodename';
begin
  GetModeName := StrPas(gmn(ModeNumber));
end;



function  GetDriverName: WrkString;
var
  Buff : PChar;
  function gdn: PChar; Asmname 'getdrivername';

begin
  Buff := gdn;
  GetDriverName := StrPas(Buff);
end;



procedure GetDefaultPalette(var Palette: PaletteType);
  type pPT = ^PaletteType;
  function gdp: pPT; AsmName '__gr_getdefaultpalette';
begin
 Palette := gdp^;
end;



function InstallUserDriver(Name: WrkString; AutoDetectPtr: pointer):integer;
begin InstallUserdriver := grError end;

function RegisterBGIdriver(driver: pointer): Integer;
begin RegisterBGIdriver := grError end;

end.

