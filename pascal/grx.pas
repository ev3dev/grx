Unit GRX;

{ Converted by Sven Hilscher eMail sven@rufus.central.de
  from Headerfile grx20.h }

{*
 ** grx20.h ---- GRX 2.x API functions and data structure declarations
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *}

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
  (*.$L tiff *)  (* !!! *)
(*$endif *)
(*$endif *)
(*$endif *)
(*.$L jpeg *)


Interface

Type
  MemPtr     = ^Char;
  GrColor    = Integer; (* color and operation type: MUST be 32bit ! *)
  GrColorPtr = ^GrColor;

Const
  (* these are the supported configurations: *)
  GRX_VERSION_TCC_8086_DOS      = 1;       (* also works with BCC *)
  GRX_VERSION_GCC_386_GO32      = 2;       (* DJGPP *)
  GRX_VERSION_GCC_386_LINUX     = 3;       (* the real stuff *)
  GRX_VERSION_GENERIC_X11       = 4;       (* generic X11 version *)
  GRX_VERSION_WATCOM_DOS4GW     = 5;       (* Watcom C++ 11.0 32 Bit *)


  {
  * available video modes (for 'GrSetMode')
  }

  GR_unknown_mode                      = -1; { initial state }
  { ============= modes which clear the video memory ============= }
  GR_80_25_text                        = 0;  { Extra parameters for GrSetMode: }
  GR_default_text                      = 1;
  GR_width_height_text                 = 2;  { Integer w,Integer h }
  GR_biggest_text                      = 3;
  GR_320_200_graphics                  = 4;
  GR_default_graphics                  = 5;
  GR_width_height_graphics             = 6;  { Integer w,Integer h }
  GR_biggest_noninterlaced_graphics    = 7;
  GR_biggest_graphics                  = 8;
  GR_width_height_color_graphics       = 9;  { Integer w,Integer h,Word nc }
  GR_width_height_color_text           = 10; { Integer w,Integer h,Word nc }
  GR_custom_graphics                   = 11; { Integer w,Integer h,Word nc,Integer vx,Integer vy }
  { ==== equivalent modes which do not clear the video memory ==== }
  GR_NC_80_25_text                     = 12;
  GR_NC_default_text                   = 13;
  GR_NC_width_height_text              = 14; { Integer w,Integer h }
  GR_NC_biggest_text                   = 15;
  GR_NC_320_200_graphics               = 16;
  GR_NC_default_graphics               = 17;
  GR_NC_width_height_graphics          = 18; { Integer w,Integer h }
  GR_NC_biggest_noninterlaced_graphics = 19;
  GR_NC_biggest_graphics               = 20;
  GR_NC_width_height_color_graphics    = 21; { Integer w,Integer h,Word nc }
  GR_NC_width_height_color_text        = 22; { Integer w,Integer h,Word nc }
  GR_NC_custom_graphics                = 23; { Integer w,Integer h,Word nc,Integer vx,Integer vy }
  { ==== plane instead of color based modes ==== }
  { colors = 1 << bpp  >>> resort enum for GRX3 <<< }
  GR_width_height_bpp_graphics         = 24; { int w,int h,int bpp }
  GR_width_height_bpp_text             = 25; { int w,int h,int bpp }
  GR_custom_bpp_graphics               = 26; { int w,int h,int bpp,int vx,int vy }
  GR_NC_width_height_bpp_graphics      = 27; { int w,int h,int bpp }
  GR_NC_width_height_bpp_text          = 28; { int w,int h,int bpp }
  GR_NC_custom_bpp_graphics            = 29; { int w,int h,int bpp,int vx,int vy }

  {
  * Available frame modes (video memory layouts)
  }

  { ====== video frame buffer modes ====== }
  GR_frameUndef   =  0;       { undefined }
  GR_frameText    =  1;       { text modes }
  GR_frameHERC1   =  2;       { Hercules mono }
  GR_frameEGAVGA1 =  3;       { EGA VGA mono }
  GR_frameEGA4    =  4;       { EGA 16 color }
  GR_frameSVGA4   =  5;       { (Super) VGA 16 color }
  GR_frameSVGA8   =  6;       { (Super) VGA 256 color }
  GR_frameVGA8X   =  7;       { VGA 256 color mode X }
  GR_frameSVGA16  =  8;       { Super VGA 32768/65536 color }
  GR_frameSVGA24  =  9;       { Super VGA 16M color }
  GR_frameSVGA32L = 10;       { Super VGA 16M color padded #1 }
  GR_frameSVGA32H = 11;       { Super VGA 16M color padded #2 }
  { ==== modes provided by the X11 driver ===== }
  GR_frameXWIN1   = GR_frameEGAVGA1;
  GR_frameXWIN4   = GR_frameSVGA4;
  GR_frameXWIN8   = GR_frameSVGA8;
  GR_frameXWIN16  = GR_frameSVGA16;
  GR_frameXWIN24  = GR_frameSVGA24;
  GR_frameXWIN32L = GR_frameSVGA32L;
  GR_frameXWIN32H = GR_frameSVGA32H;
  { ==== linear frame buffer modes  ====== }
  GR_frameSVGA8_LFB   = 12;   { (Super) VGA 256 color }
  GR_frameSVGA16_LFB  = 13;   { Super VGA 32768/65536 color }
  GR_frameSVGA24_LFB  = 14;   { Super VGA 16M color }
  GR_frameSVGA32L_LFB = 15;   { Super VGA 16M color padded #1 }
  GR_frameSVGA32H_LFB = 16;   { Super VGA 16M color padded #2 }
  { ====== system RAM frame buffer modes ====== }
  GR_frameRAM1    = 17;       { mono }
  GR_frameRAM4    = 18;       { 16 color planar }
  GR_frameRAM8    = 19;       { 256 color }
  GR_frameRAM16   = 20;       { 32768/65536 color }
  GR_frameRAM24   = 21;       { 16M color }
  GR_frameRAM32L  = 22;       { 16M color padded #1 }
  GR_frameRAM32H  = 23;       { 16M color padded #2 }
  GR_frameRAM3x8  = 24;       { 16M color planar (image mode) }
  { ====== markers for scanning modes ====== }
  GR_firstTextFrameMode     = GR_frameText;
  GR_lastTextFrameMode      = GR_frameText;
  GR_firstGraphicsFrameMode = GR_frameHERC1;
  GR_lastGraphicsFrameMode  = GR_frameSVGA32H_LFB;
  GR_firstRAMframeMode      = GR_frameRAM1;
  GR_lastRAMframeMode       = GR_frameRAM3x8;

  {
  * supported video adapter types
  }

  GR_UNKNOWN = -1;          { not known (before driver set) }
  GR_VGA     =  0;          { VGA adapter }
  GR_EGA     =  1;          { EGA adapter }
  GR_HERC    =  2;          { Hercules mono adapter }
  GR_8514A   =  3;          { 8514A or compatible }
  GR_S3      =  4;          { S3 graphics accelerator }
  GR_XWIN    =  5;          { X11 driver }
  GR_MEM     =  6;          { memory only driver }

  GR_MAX_POLYGON_POINTERS = 1000000;
  GR_MAX_ELLIPSE_POINTERS = 1024 + 5;
  GR_MAX_ANGLE_VALUE      = 3600;
  GR_ARC_STYLE_OPEN       = 0;
  GR_ARC_STYLE_CLOSE1     = 1;
  GR_ARC_STYLE_CLOSE2     = 2;

  { bits in the GrVideoDriver.drvflags field: }
  GR_DRIVERF_USER_RESOLUTION = 1;
	{ set if driver supports user setable arbitrary resolution }



Type
{ The video driver descriptor structure }
  { struct _GR_videoDriver }
  GrVideoDriver = record
	name   : CString;        { driver name }
	adapter: Char;           { adapter type }
	inherit: ^GrVideoDriver; { inherit video modes from this }
	modes  : ^GrVideoMode;   { table of supported modes }
	nmodes : Integer;        { number of modes }
	detect : Pointer;
	init   : Pointer;
	reset  : Pointer;
	selectmode: Pointer;
	drvflags  : Integer;
  end;
  GrVideoDriverPtr = ^GrVideoDriver;

{ Video driver mode descriptor structure }
  { struct _GR_videoMode }
  GrVideoMode = record
	present     : Char;         { is it really available? }
	bpp         : Char;       { log2 of # of colors }
	width,height: ShortInt;    { video mode geometry }
	mode        : ShortInt;         { BIOS mode number (if any) }
	lineoffset  : Integer;         { scan line length }
	privdata    : Integer;        { driver can use it for anything }
	extinfo     : ^GrVideoModeExt;   { extra info (maybe shared) }
  end;
  GrVideoModePtr = ^GrVideoMode;

  {
  * Video driver mode descriptor extension structure. This is a separate
  * structure accessed via a pointer from the main mode descriptor. The
  * reason for this is that frequently several modes can share the same
  * extended info.
  }

  { struct _GR_videoModeExt }
  GrVideoModeExt = record
	mode  : Integer;                  { frame driver for this video mode }
	drv   : ^GrFrameDriver;           { optional frame driver override }
	frame : MemPtr;                   { frame buffer address }
	cprec : array[1..3] of ByteCard;  { color component precisions }
	cpos  : array[1..3] of ByteCard;  { color component bit positions }
	flags : Integer;                  { mode flag bits; see "grdriver.h" }
	setup    : Pointer;
	setvsize : Pointer;
	scroll   : Pointer;
	setbank  : Pointer;
	setrwbanks : Pointer;
	loadcolor : Pointer;
	LFB_Selector : Integer;
  end;

  { The frame driver descriptor structure. }

  { struct _GR_frameDriver }
  GrFrameDriver = record
	mode           : Integer; { supported frame access mode }
	rmode          : Integer; { matching RAM frame (if video) }
	is_video       : Integer; { video RAM frame driver ? }
	row_align      : Integer; { scan line size alignment }
	num_planes     : Integer; { number of planes }
	bits_per_pixel : Integer; { bits per pixel }
	max_plane_size : Integer; { maximum plane size in bytes }
	init           : Pointer;
	readpixel      : Pointer;
	drawpixel      : Pointer;
	drawline       : Pointer;
	drawhline      : Pointer;
	drawvline      : Pointer;
	drawblock      : Pointer;
	drawbitmap     : Pointer;
	drawpattern    : Pointer;
	bitblt         : Pointer;
	bltv2r         : Pointer;
	bltr2v         : Pointer;
	getindexedscanline : Pointer;
	putscanline    : Pointer;
  end;
  GrFrameDriverPtr = ^GrFrameDriver;

  {
  * driver and mode info structure
  }

  { extern const struct _GR_driverInfo }
  GrDriverInfoType = record
	vdriver     : ^GrVideoDriver; { the current video driver }
	curmode     : ^GrVideoMode;   { current video mode pointer }
	actmode     : GrVideoMode;    { copy of above, resized if virtual }
	fdriver     : GrFrameDriver;  { frame driver for the current context }
	sdriver     : GrFrameDriver;  { frame driver for the screen }
	tdriver     : GrFrameDriver;  { a dummy driver for text modes }
	mcode       : ByteCard;       { code for the current mode }
	deftw,defth : Integer;        { default text mode size }
	defgw,defgh : Integer;        { default graphics mode size }
	deftc,defgc : GrColor;        { default text and graphics colors }
	vposx,vposy : Integer;        { current virtual viewport position }
	errsfatal   : Integer;        { if set, exit upon errors }
	moderestore : Integer;        { restore startup video mode if set }
	splitbanks  : Integer;        { indicates separate R/W banks }
	curbank     : Integer;        { currently mapped bank }
	mdsethook   : Pointer;        { callback for mode set }
	setbank     : Pointer;        { banking routine }
	setrwbanks  : Pointer;        { split banking routine }
  end;

var
  GrDriverInfo : ^GrDriverInfoType; AsmName 'GrDriverInfo'; external;

{ setup stuff }

Function  GrSetDriver(drvspec: CString):Integer; AsmName 'GrSetDriver';
Function  GrSetMode(m, w, h, nc, vx, vy:Integer):Integer; AsmName 'GrSetMode';
Function  GrSetViewport(xpos, ypos: Integer):Integer; AsmName 'GrSetViewport';
Procedure GrSetModeHook(hookfunc:Pointer); AsmName 'GrSetModeHook';
Procedure GrSetModeRestore(restoreFlag:Boolean); AsmName 'GrSetModeRestore';

Procedure GrSetErrorHandling(exitIfError:Boolean); AsmName 'GrSetErrorHandling';
Procedure GrSetEGAVGAmonoDrawnPlane(plane:Integer); AsmName 'GrSetEGAVGAmonoDrawnPlane';
Procedure GrSetEGAVGAmonoShownPlane(plane:Integer); AsmName 'GrSetEGAVGAmonoShownPlane';

Function  GrGetLibraryVersion : Integer; AsmName 'GrGetLibraryVersion';
Function  GrGetLibrarySystem  : Integer; AsmName 'GrGetLibrarySystem';

{ inquiry stuff ---- many of these can be macros }

Function  GrCurrentMode:Integer; AsmName 'GrCurrentMode';
Function  GrAdapterType:Integer; AsmName 'GrAdapterType';
Function  GrCurrentFrameMode:Integer; AsmName 'GrCurrentFrameMode';
Function  GrScreenFrameMode:Integer; AsmName 'GrScreenFrameMode';
Function  GrCoreFrameMode:Integer; AsmName 'GrCoreFrameMode';

Function  GrCurrentVideoDriver: GrVideoDriverPtr; AsmName 'GrCurrentVideoDriver' ;
Function  GrCurrentVideoMode:GrVideoModePtr; AsmName 'GrCurrentVideoMode';
Function  GrVirtualVideoMode:GrVideoModePtr; AsmName 'GrVirtualVideoMode';
Function  GrCurrentFrameDriver: GrFrameDriverPtr; AsmName 'GrCurrentFrameDriver';
Function  GrScreenFrameDriver: GrFrameDriverPtr; AsmName 'GrScreenFrameDriver';

Function  GrFirstVideoMode(fmode:Integer):GrVideoModePtr; AsmName 'GrFirstVideoMode';
Function  GrNextVideoMode(prev:GrVideoModePtr):GrVideoModePtr; AsmName 'GrNextVideoMode';

Function  GrScreenX:Integer; AsmName 'GrScreenX';
Function  GrScreenY:Integer; AsmName 'GrScreenY';
Function  GrVirtualX:Integer; AsmName 'GrVirtualX';
Function  GrVirtualY:Integer; AsmName 'GrVirtualY';
Function  GrViewportX:Integer; AsmName 'GrViewportX';
Function  GrViewportY:Integer; AsmName 'GrViewportY';
Function  GrScreenIsVirtual:Boolean; AsmName 'GrScreenIsVirtual';

{ RAM context geometry and memory allocation inquiry stuff }

Function  GrFrameNumPlanes(md:Integer):Integer; AsmName 'GrFrameNumPlanes';
Function  GrFrameLineOffset(md,width:Integer):Integer; AsmName 'GrFrameLineOffset';
Function  GrFramePlaneSize(md,w,h:Integer):Integer; AsmName 'GrFramePlaneSize';
Function  GrFrameContextSize(md,w,h:Integer):Integer; AsmName 'GrFrameContextSize';

Function  GrNumPlanes:Integer; AsmName 'GrNumPlanes';
Function  GrLineOffset(width:Integer):Integer; AsmName 'GrLineOffset';
Function  GrPlaneSize(w,h:Integer):Integer; AsmName 'GrPlaneSize';
Function  GrContextSize(w,h:Integer):Integer; AsmName 'GrContextSize';

{
==================================================================
	  FRAME BUFFER, CONTEXT AND CLIPPING STUFF
==================================================================
}

Type
  GrFrameType = record
	gf_baseaddr   : array[0..3] of MemPtr; { base address of frame memory }
	gf_selector   : ShortInt;              { frame memory segment selector }
	gf_onscreen   : Char;                  { is it in video memory ? }
	gf_memflags   : Char;                  { memory allocation flags }
	gf_lineoffset : Integer;               { offset to next scan line in bytes }
	gf_driver     : ^GrFrameDriver;        { frame access functions }
  end;

  { struct _GR_context }
  GrContext = record
	gc_frame     : GrFrameType;  { frame buffer info }
	gc_root      : ^GrContext;   { context which owns frame }
	gc_xmax      : Integer;      { max X coord (width  - 1) }
	gc_ymax      : Integer;      { max Y coord (height - 1) }
	gc_xoffset   : Integer;      { X offset from root's base }
	gc_yoffset   : Integer;      { Y offset from root's base }
	gc_xcliplo   : Integer;      { low X clipping limit }
	gc_ycliplo   : Integer;      { low Y clipping limit }
	gc_xcliphi   : Integer;      { high X clipping limit }
	gc_ycliphi   : Integer;      { high Y clipping limit }
	gc_usrxbase  : Integer;      { user window min X coordinate }
	gc_usrybase  : Integer;      { user window min Y coordinate }
	gc_usrwidth  : Integer;      { user window width  }
	gc_usrheight : Integer;      { user window height }
  end;
  GrContextPtr = ^GrContext;

  { extern const struct _GR_contextInfo }
  GrContextInfoType = record
	current: GrContext;  { the current context }
	screen : GrContext;  { the screen context }
  end;

var
  GrContextInfo : GrContextInfoType; AsmName 'GrContextInfo'; external;

Function  GrCreateContext(w, h: Integer; memory: MemPtr; var where: GrContext): GrContextPtr; AsmName 'GrCreateContext';
Function  GrCreateFrameContext(md: ByteCard; w, h: Integer; memory: MemPtr; var where: GrContext): GrContextPtr; AsmName 'GrCreateFrameContext';
Function  GrCreateSubContext(x1, y1, x2, y2: Integer; var parent, where: GrContext): GrContextPtr; AsmName 'GrCreateSubContext';
Function  GrSaveContext(var where: GrContext): GrContextPtr; AsmName 'GrSaveContext';
Function  GrCurrentContext: GrContextPtr; AsmName 'GrCurrentContext';
Function  GrScreenContext : GrContextPtr; AsmName 'GrScreenContext';

Procedure GrDestroyContext(context: GrContextPtr); AsmName 'GrDestroyContext';
Procedure GrResizeSubContext(context: GrContextPtr; x1, y1, x2, y2: Integer); AsmName 'GrResizeSubContext';
Procedure GrSetContext(context: GrContextPtr); AsmName 'GrSetContext';

Procedure GrSetClipBox(x1, y1, x2, y2:Integer); AsmName 'GrSetClipBox';
Procedure GrGetClipBox(var x1p, y1p, x2p, y2p: Integer); AsmName 'GrGetClipBox';
Procedure GrResetClipBox; AsmName 'GrResetClipBox';

Function  GrMaxX: Integer; AsmName 'GrMaxX';
Function  GrMaxY: Integer; AsmName 'GrMaxY';
Function  GrSizeX:Integer; AsmName 'GrSizeX';
Function  GrSizeY:Integer; AsmName 'GrSizeY';
Function  GrLowX: Integer; AsmName 'GrLowX';
Function  GrLowY: Integer; AsmName 'GrLowY';
Function  GrHighX:Integer; AsmName 'GrHighX';
Function  GrHighY:Integer; AsmName 'GrHighY';

{
==================================================================
			 COLOR STUFF
==================================================================
}

{ Flags to 'OR' to colors for various operations }

const
  GrWRITE      = 0                ;  { write color }
  GrXOR        = $01000000        ;  { to "XOR" any color to the screen }
  GrOR         = $02000000        ;  { to "OR" to the screen }
  GrAND        = $03000000        ;  { to "AND" to the screen }
  GrIMAGE      = $04000000        ;  { BLIT: write, except given color }
  GrCVALUEMASK = $00ffffff        ;  { color value mask }
  GrCMODEMASK  = $ff000000        ;  { color operation mask }
  GrNOCOLOR    = GrXOR or 0       ;  { GrNOCOLOR is used for "no" color }

Function  GrColorValue(c:GrColor):GrColor; AsmName 'GrColorValue';
Function  GrColorMode(c:GrColor):GrColor; AsmName 'GrColorMode';
Function  GrWriteModeColor(c:GrColor):GrColor; AsmName 'GrWriteModeColor';
Function  GrXorModeColor(c:GrColor):GrColor; AsmName 'GrXorModeColor';
Function  GrOrModeColor(c:GrColor):GrColor; AsmName 'GrOrModeColor';
Function  GrAndModeColor(c:GrColor):GrColor; AsmName 'GrAndModeColor';
Function  GrImageModeColor(c:GrColor):GrColor; AsmName 'GrImageModeColor';

Procedure GrResetColors; AsmName 'GrResetColors';
Procedure GrSetRGBcolorMode; AsmName 'GrSetRGBcolorMode';
Procedure GrRefreshColors; AsmName 'GrRefreshColors';

Function  GrNumColors:GrColor; AsmName 'GrNumColors';
Function  GrNumFreeColors:GrColor; AsmName 'GrNumFreeColors';

Function  GrBlack:GrColor; AsmName 'GrBlack';
Function  GrWhite:GrColor; AsmName 'GrWhite';

Function  GrBuildRGBcolorT(r,g,b:Integer):GrColor; AsmName 'GrBuildRGBcolorT';
Function  GrBuildRGBcolorR(r,g,b:Integer):GrColor; AsmName 'GrBuildRGBcolorR';
Function  GrRGBcolorRed(c:GrColor):Integer ; AsmName 'GrRGBcolorRed';
Function  GrRGBcolorGreen(c:GrColor):Integer ; AsmName 'GrRGBcolorGreen';
Function  GrRGBcolorBlue(c:GrColor):Integer ; AsmName 'GrRGBcolorBlue';

Function  GrAllocColor(r,g,b:Integer):GrColor; AsmName 'GrAllocColor'; { shared, read-only }
Function  GrAllocColorID(r,g,b:Integer):GrColor; AsmName 'GrAllocColorID'; { potentially inlined version }
Function  GrAllocCell:GrColor; AsmName 'GrAllocCell'; { unshared, read-write }

Function  GrAllocEgaColors:GrColorPtr; AsmName 'GrAllocEgaColor'; { shared, read-only standard EGA colors }

Procedure GrSetColor(c:GrColor; r,g,b:Integer); AsmName 'GrSetColor';
Procedure GrFreeColor(c:GrColor); AsmName 'GrFreeColor';
Procedure GrFreeCell(c:GrColor);  AsmName 'GrFreeCell';

Procedure GrQueryColor(c:GrColor; var r,g,b:Integer); AsmName 'GrQueryColor';
Procedure GrQueryColorID(c:GrColor; var r,g,b:Integer); AsmName 'GrQueryColorID';

Function  GrColorSaveBufferSize:Integer ; AsmName 'GrColorSaveBufferSize';
Procedure GrSaveColors(buffer:Pointer); AsmName 'GrSaveColors';
Procedure GrRestoreColors(buffer:Pointer); AsmName 'GrRestoreColors';

Type
  { typedef struct } { framed box colors }
  GrFBoxColors = record
	fbx_Integercolor : Integer;
	fbx_topcolor     : Integer;
	fbx_rightcolor   : Integer;
	fbx_bottomcolor  : Integer;
	fbx_leftcolor    : Integer;
  end;

Procedure GrClearScreen(bg:GrColor); AsmName 'GrClearScreen';
Procedure GrClearClipBox(bg:GrColor); AsmName 'GrClearClipBox';
Procedure GrPlot(x, y: Integer; c:GrColor); AsmName 'GrPlot';
Procedure GrLine(x1, y1, x2, y2: Integer; c:GrColor); AsmName 'GrLine';
Procedure GrHLine(x1, x2, y:Integer; c:GrColor); AsmName 'GrHLine';
Procedure GrVLine(x, y1, y2: Integer; c: GrColor); AsmName 'GrVLine';
Procedure GrBox(x1, y1, x2, y2: Integer; c: GrColor); AsmName 'GrBox';
Procedure GrFilledBox(x1, y1, x2, y2: Integer; c: GrColor); AsmName 'GrFilledBox';
Procedure GrFramedBox(x1, y1, x2, y2, wdt: Integer; var c: GrFBoxColors); AsmName 'GrFramedBox';
Function  GrGenerateEllipse(xc, yc, xa, ya:Integer; var poIntegers: array of Integer):Integer; AsmName 'GrGenerateEllipse';
Function  GrGenerateEllipseArc(xc, yc, xa, ya, start, ende: Integer; var poIntegers: array of Integer):Integer; AsmName 'GrGenerateEllipseArc';
Procedure GrLastArcCoords(var xs, ys, xe, ye, xc, yc: Integer); AsmName 'GrLastArcCoords';
Procedure GrCircle(xc, yc, r: Integer; c: GrColor); AsmName 'GrCircle';
Procedure GrEllipse(xc, yc, xa, ya: Integer; c: GrColor); AsmName 'GrEllipse';
Procedure GrCircleArc(xc, yc, r, start, ende, style: Integer; c: GrColor); AsmName 'GrCircleArc';
Procedure GrEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; c: GrColor); AsmName 'GrEllipseArc';
Procedure GrFilledCircle(xc, yc, r: Integer; c: GrColor); AsmName 'GrFilledCircle';
Procedure GrFilledEllipse(xc, yc, xa, ya: Integer; c: GrColor); AsmName 'GrFilledEllipse';
Procedure GrFilledCircleArc(xc, yc, r, start, ende, style: Integer; c: GrColor); AsmName 'GrFilledCircleArc';
Procedure GrFilledEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; c: GrColor); AsmName 'GrFilledEllipseArc';
Procedure GrPolyLine(numpts: Integer; var poIntegers: array of Integer; c: GrColor); AsmName 'GrPolyLine';
Procedure GrPolygon(numpts: Integer; var poIntegers:array of Integer; c: GrColor); AsmName 'GrPolygon';
Procedure GrFilledConvexPolygon(numpts: Integer; var poIntegers:array of Integer; c: GrColor); AsmName 'GrFilledConvexPolygon';
Procedure GrFilledPolygon(numpts: Integer; var poIntegers:array of Integer; c: GrColor); AsmName 'GrFilledPolygon';
Procedure GrBitBlt(dst: GrContextPtr; x, y: Integer; src: GrContextPtr; x1, y1, x2, y2: Integer; op: GrColor); AsmName 'GrBitBlt';
Function  GrPixel(x, y:Integer):GrColor; AsmName 'GrPixel';
Function  GrPixelC(c: GrContextPtr; x, y: Integer): GrColor; AsmName 'GrPixelC';

Procedure GrFloodFill(x, y: Integer; border, c: GrColor); AsmName 'GrFloodFill';

Function GrGetScanline(x1,x2,yy: Integer) : GrColorPtr; AsmName 'GrGetScanline';
Function GrGetScanlineC(ctx: GrContextPtr; x1,x2,yy: Integer) : GrColorPtr; AsmName 'GrGetScanlineC';
(* Input   ctx: source context, if NULL the current context is used *)
(*         x1 : first x coordinate read                             *)
(*         x2 : last  x coordinate read                             *)
(*         yy : y coordinate                                        *)
(* Output  NIL      : error / no data (clipping occured)            *)
(*         else                                                     *)
(*           p[0..w]: pixel values read                             *)
(*                      (w = |x2-y1|)                               *)
(*           Output data is valid until next GRX call !             *)

Procedure GrPutScanline(x1,x2,yy: Integer;c: GrColorPtr; op: GrColor); AsmName 'GrPutScanline';
(* Input   x1 : first x coordinate to be set                        *)
(*         x2 : last  x coordinate to be set                        *)
(*         yy : y coordinate                                        *)
(*         c  : c[0..(|x2-x1|] hold the pixel data                  *)
(*         op : Operation (GrWRITE/GrXOR/GrOR/GrAND/GrIMAGE)        *)
(*                                                                  *)
(* Note    c[..] data must fit GrCVALUEMASK otherwise the results   *)
(*         are implementation dependend.                            *)
(*         => You can't supply operation code with the pixel data!  *)


{
  ==================================================================
	NON CLIPPING DRAWING PRIMITIVES
  ==================================================================
}
Procedure GrPlotNC(x, y: Integer; c: GrColor); AsmName 'GrPlotNC';
Procedure GrLineNC(x1, y1, x2, y2: Integer; c: GrColor); AsmName 'GrLineNC';
Procedure GrHLineNC(x1, x2, y: Integer; c: GrColor); AsmName 'GrHLineNC';
Procedure GrVLineNC(x, y1, y2: Integer; c: GrColor); AsmName 'GrVLineNC';
Procedure GrBoxNC(x1, y1, x2, y2: Integer; c: GrColor); AsmName 'GrBoxNC';
Procedure GrFilledBoxNC(x1, y1, x2, y2: Integer; c: GrColor); AsmName 'GrFilledBoxNC';
Procedure GrFramedBoxNC(x1, y1, x2, y2, wdt: Integer; var c: GrFBoxColors); AsmName 'GrFramedBoxNC';
Procedure GrBitBltNC(dst: GrContextPtr; x, y: Integer; src: GrContextPtr; x1, y1, x2, y2: Integer; op: GrColor); AsmName 'GrBitBltNC';
Function  GrPixelNC(x, y:Integer):GrColor; AsmName 'GrPixelNC';
Function  GrPixelCNC(c: GrContextPtr; x, y: Integer): GrColor; AsmName 'GrPixelCNC';

{
==================================================================
	 FONTS AND TEXT PRIMITIVES
==================================================================
}

{ text drawing directions }
Const
  GR_TEXT_RIGHT      = 0;  { normal }
  GR_TEXT_DOWN       = 1;  { downward }
  GR_TEXT_LEFT       = 2;  { upside down, right to left }
  GR_TEXT_UP         = 3;  { upward }
  GR_TEXT_DEFAULT    = GR_TEXT_RIGHT;
  { GR_TEXT_IS_VERTICAL(d) = ((d) and 1); }

  { text alignment options }
  GR_ALIGN_LEFT      = 0;  { X only }
  GR_ALIGN_TOP       = 0;  { Y only }
  GR_ALIGN_CENTER    = 1;  { X, Y     }
  GR_ALIGN_RIGHT     = 2;  { X only }
  GR_ALIGN_BOTTOM    = 2;  { Y only }
  GR_ALIGN_BASELINE  = 3;  { Y only }
  GR_ALIGN_DEFAULT   = GR_ALIGN_LEFT;

  { character types in text strings }
  GR_BYTE_TEXT    = 0;  { one byte per character }
  GR_WORD_TEXT    = 1;  { two bytes per character }
  GR_ATTR_TEXT    = 2;  { chr w/ PC style attribute byte }

{ OR this to the foreground color value for underlined text when
  using GR_BYTE_TEXT or GR_WORD_TEXT modes. }

{ GR_UNDERLINE_TEXT  =  (GrXOR << 4); }

  { Font conversion flags for 'GrLoadConvertedFont'. OR them as desired. }
  GR_FONTCVT_NONE       =  0;  { no conversion }
  GR_FONTCVT_SKIPCHARS  =  1;  { load only selected characters }
  GR_FONTCVT_RESIZE     =  2;  { resize the font }
  GR_FONTCVT_ITALICIZE  =  4;  { tilt font for "italic" look }
  GR_FONTCVT_BOLDIFY    =  8;  { make a "bold"(er) font  }
  GR_FONTCVT_FIXIFY     = 16;  { convert prop. font to fixed wdt }
  GR_FONTCVT_PROPORTION = 32;  { convert fixed font to prop. wdt }

Type
{ font structures }
  { typedef struct _GR_fontHeader }    { font descriptor }
  GrFontHeader = record
	name         : CString; { font name }
	family       : CString; { font family name }
	proportional : Char;    { characters have varying width }
	scalable     : Char;    { derived from a scalable font }
	preloaded    : Char;    { set when linked Integero program }
	modified     : Char;    { "tweaked" font (resized, etc..) }
	width        : Integer; { width (proportional=>average) }
	height       : Integer; { font height }
	baseline     : Integer; { baseline pixel pos (from top) }
	ulpos        : Integer; { underline pixel pos (from top) }
	ulheight     : Integer; { underline width }
	minchar      : Integer; { lowest character code in font }
	numchars     : Integer; { number of characters in font }
  end;

  { typedef struct _GR_fontChrInfo } { character descriptor }
  GrFontChrInfo = record
	width  : Integer;     { width of this character }
	offset : Integer;     { offset from start of bitmap }
  end;

  { typedef struct _GR_font } { the complete font }
  AuxoffsType = array[1..7] of Integer;
  GrFont = record
	h        : GrFontHeader;  { the font info structure }
	bitmap   : CString;       { character bitmap array }
	auxmap   : CString;       { map for rotated & underline chrs }
	minwidth : Integer;       { width of narrowest character }
	maxwidth : Integer;       { width of widest character }
	auxsize  : Integer;       { allocated size of auxiliary map }
	auxnext  : Integer;       { next free byte in auxiliary map }
	auxoffs  : ^AuxoffsType;  { offsets to completed aux chars }
	chrinfo  : array[1..1] of GrFontChrInfo; { character info (not act. size) }
  end;

  GrFontPtr = ^GrFont;

var
  GrFont_PC6x8  : GrFont; AsmName 'GrFont_PC6x8'; external;
  GrFont_PC8x8  : GrFont; AsmName 'GrFont_PC8x8'; external;
  GrFont_PC8x14 : GrFont; AsmName 'GrFont_PC8x14'; external;
  GrFont_PC8x16 : GrFont; AsmName 'GrFont_PC8x16'; external;
  GrDefaultFont : GrFont; AsmName 'GrFont_PC8x14'; external;

Function GrLoadFont(name: CString): GrFontPtr; AsmName 'GrLoadFont';
Function GrLoadConvertedFont(name: CString; cvt, w, h, minch, maxch: Integer): GrFontPtr; AsmName 'GrLoadConvertedFont';
Function GrBuildConvertedFont(var from: GrFont; cvt, w, h, minch, maxch: Integer): GrFontPtr; AsmName 'GrBuildConvertedFont';

Procedure GrUnloadFont(var font: GrFont); AsmName 'GrUnloadFont';
Procedure GrDumpFont(var f: GrFont; CsymbolName, FileName: CString); AsmName 'GrDumpFont';
Procedure GrSetFontPath(path_list: CString); AsmName 'GrSetFontPath';

Function  GrFontCharPresent(var font: GrFont; chr: Integer): Boolean; AsmName 'GrFontCharPresent';
Function  GrFontCharWidth(var font: GrFont; chr: Integer): Integer; AsmName 'GrFontCharWidth';
Function  GrFontCharHeight(var font: GrFont; chr: Integer): Integer; AsmName 'GrFontCharHeight';
Function  GrFontCharBmpRowSize(var font: GrFont; chr: Integer): Integer; AsmName 'GrFontCharBmpRowSize';
Function  GrFontCharBitmapSize(var font: GrFont; chr: Integer): Integer; AsmName 'GrFontCharBitmapSize';
Function  GrFontStringWidth(var font: GrFont; text: Pointer; len, typ_e: Integer): Integer; AsmName 'GrFontStringWidth';
Function  GrFontStringHeight(var font: GrFont; text: Pointer; len, typ_e: Integer): Integer; AsmName 'GrFontStringHeight';
Function  GrProportionalTextWidth(var font: GrFont; text: Pointer; len, typ_e: Integer): Integer; AsmName 'GrProportionalTextWidth';
Function  GrBuildAuxiliaryBitmap(var font: GrFont; chr, dir, ul: Integer): CString; AsmName 'GrBuildAuxiliaryBitmap';
Function  GrFontCharBitmap(var font: GrFont; chr: Integer): CString; AsmName 'GrFontCharBitmap';
Function  GrFontCharAuxBmp(var font: GrFont; chr, dir, ul: Integer): CString; AsmName 'GrFontCharAuxBmp';

Type
  GrColorTableP = ^Integer;

  { typedef union _GR_textColor }   { text color union }
  GrTextColor = record
	case 1..2 of
	  1: (v : GrColor);           { color value for "direct" text }
	  2: (p : GrColorTableP);     { color table for attribute text }
  end;

  { typedef struct _GR_textOption } { text drawing option structure }
  GrTextOption = record
	txo_font    : GrFontPtr;    { font to be used }
	txo_fgcolor : GrTextColor;  { foreground color }
	txo_bgcolor : GrTextColor;  { background color }
	txo_chrtype : ByteCard;     { character type (see above) }
	txo_direct  : ByteCard;     { direction (see above) }
	txo_xalign  : ByteCard;     { X alignment (see above) }
	txo_yalign  : ByteCard;     { Y alignment (see above) }
  end;

  { typedef struct }        { fixed font text window desc. }
  GrTextRegion = record
	txr_font       : GrTextColor; { font to be used }
	txr_fgcolor    : GrTextColor; { foreground color }
	txr_bgcolor    : GrTextColor; { background color }
	txr_buffer     : Pointer;     { pointer to text buffer }
	txr_backup     : Pointer;     { optional backup buffer }
	txr_width      : Integer;     { width of area in chars }
	txr_height     : Integer;     { height of area in chars }
	txr_lineoffset : Integer;     { offset in buffer(s) between rows }
	txr_xpos       : Integer;     { upper left corner X coordinate }
	txr_ypos       : Integer;     { upper left corner Y coordinate }
	txr_chrtype    : Char;        { character type (see above) }
  end;

Function  GrCharWidth(chr: Integer; var opt: GrTextOption): Integer; AsmName 'GrCharWidth';
Function  GrCharHeight(chr: Integer; var opt: GrTextOption): Integer; AsmName 'GrCharHeight';
Procedure GrCharSize(chr: Integer; var opt: GrTextOption; var w, h: Integer); AsmName 'GrCharSize';
Function  GrStringWidth(var text: CString; length: Integer; var opt: GrTextOption): Integer; AsmName 'GrStringWidth';
Function  GrStringHeight(var text: CString; length: Integer; var opt: GrTextOption): Integer; AsmName 'GrStringHeight';
Procedure GrStringSize(var text: CString; length: Integer; var opt: GrTextOption;var w, h: Integer); AsmName 'GrStringSize';

Procedure GrDrawChar(chr, x, y: Integer; var opt: GrTextOption); AsmName 'GrDrawChar';
Procedure GrDrawString(text: CString; length, x, y: Integer; var opt: GrTextOption); AsmName 'GrDrawString';
Procedure GrTextXY(x, y: Integer; text: CString; fg, bg: GrColor); AsmName 'GrTextXY';

Procedure GrDumpChar(chr, col, row: Integer; var r: GrTextRegion); AsmName 'GrDumpChar';
Procedure GrDumpText(col, row, wdt, hgt: Integer; var r: GrTextRegion); AsmName 'GrDumpText';
Procedure GrDumpTextRegion(var r: GrTextRegion); AsmName 'GrDumpTextRegion';


{ =================================================================
		THICK AND DASHED LINE DRAWING PRIMITIVES
  ================================================================== }

{
 * custom line option structure
 *   zero or one dash pattern length means the line is continuous
 *   the dash pattern always begins with a drawn section
 }

Type
  GrLineOption = record
	no_color    : GrColor;   { color used to draw line }
	lno_width   : Integer;   { width of the line }
	lno_pattlen : Integer;   { length of the dash pattern }
	lno_dashpat : ^ByteCard; { draw/nodraw pattern }
  end;

Procedure GrCustomLine(x1, y1, x2, y2: Integer; var o: GrLineOption); AsmName 'GrCustomLine';
Procedure GrCustomBox(x1, y1, x2, y2: Integer; var o: GrLineOption); AsmName 'GrCustomBox';
Procedure GrCustomCircle(xc, yc, r: Integer; var o: GrLineOption); AsmName 'GrCustomCircle';
Procedure GrCustomEllipse(xc, yc, r: Integer; var o: GrLineOption); AsmName 'GrCustomEllipse';
Procedure GrCustomCircleArc(xc, yc, r, start, ende, style: Integer; var o: GrLineOption); AsmName 'GrCustomCircleArc';
Procedure GrCustomEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; var o: GrLineOption); AsmName 'GrCustomEllipseArc';
Procedure GrCustomPolyLine(numpts: Integer; var poIntegers: array of Integer; var o: GrLineOption); AsmName 'GrCustomPolyLine';
Procedure GrCustomPolygon(numpts: Integer; var poIntegers: array of Integer; var o: GrLineOption); AsmName 'GrCustomPolygon';

{ ==================================================================
	   PATTERNED DRAWING AND FILLING PRIMITIVES
  ================================================================== }

  {
  * BITMAP: a mode independent way to specify a fill pattern of two
  *   colors. It is always 8 pixels wide (1 byte per scan line), its
  *   height is user-defined. SET THE TYPE FLAG TO ZERO!!!
  }

Type
  { typedef struct _GR_bitmap }
  GrBitmap = record
	bmp_ispixmap : Integer;      { type flag for pattern union }
	bmp_height   : Integer;      { bitmap height }
	bmp_data     : ^ByteCard;    { pointer to the bit pattern }
	bmp_fgcolor  : GrColor;      { foreground color for fill }
	bmp_bgcolor  : GrColor;      { background color for fill }
	bmp_memflags : Integer;      { set if dynamically allocated }
  end;

  {
  * PIXMAP: a fill pattern stored in a layout identical to the video RAM
  *   for filling using 'bitblt'-s. It is mode dependent, typically one
  *   of the library functions is used to build it. KEEP THE TYPE FLAG
  *   NONZERO!!!
  }

  { typedef struct _GR_pixmap }
  GrPixmap = record
	pxp_ispixmap : Integer;     { type flag for pattern union }
	pxp_width    : Integer;     { pixmap width (in pixels)  }
	pxp_height   : Integer;     { pixmap height (in pixels) }
	pxp_oper     : GrColor;     { bitblt mode (SET, OR, XOR, AND, IMAGE) }
	pxp_source   : GrFrameType; { source context for fill }
  end;

  {
  * Fill pattern union -- can either be a bitmap or a pixmap
  }

  { typedef union _GR_pattern }
  GrPattern = record
	gp_ispixmap : Integer;      { nonzero for pixmaps }
	gp_bitmap   : GrBitmap;     { fill bitmap }
	gp_pixmap   : GrPixmap;     { fill pixmap }
  end;
  GrPatternPtr = ^GrPattern;

  {
  * Draw pattern for line drawings -- specifies both the:
  *   (1) fill pattern, and the
  *   (2) custom line drawing option
  }
  GrLinePattern = record
	lnp_pattern  : GrPatternPtr;   { fill pattern }
	lnp_option   : ^GrLineOption;  { width + dash pattern }
  end;
  GrLinePatternPtr = GrLinePattern;

Function  GrBuildPixmap(pixels: CString; w, h: Integer; colors : GrColorTableP): GrPatternPtr; AsmName 'GrBuildPixmap';
Function  GrBuildPixmapFromBits(bits: CString; w, h: Integer; fgc, bgc: GrColor): GrPatternPtr; AsmName 'GrBuildPixmapFromBits';
Function  GrConvertToPixmap(src: GrContextPtr): GrPatternPtr; AsmName 'GrConvertToPixmap';

Procedure GrDestroyPattern(p: GrPatternPtr); AsmName 'GrDestroyPattern';

Procedure GrPatternedLine(x1, y1, x2, y2: Integer; lp: GrLinePatternPtr); AsmName 'GrPatternedLine';
Procedure GrPatternedBox(x1, y1, x2, y2: Integer; lp: GrLinePatternPtr); AsmName 'GrPatternedBox';
Procedure GrPatternedCircle(xc, yc, r: Integer; lp: GrLinePatternPtr); AsmName 'GrPatternedCircle';
Procedure GrPatternedEllipse(xc, yc, xa, ya: Integer; lp: GrLinePatternPtr); AsmName 'GrPatternedEllipse';
Procedure GrPatternedCircleArc(xc, yc, r, start, ende, style: Integer; lp: GrLinePatternPtr); AsmName 'GrPatternedCircleArc';
Procedure GrPatternedEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; lp: GrLinePatternPtr); AsmName 'GrPatternedEllipseArc';
Procedure GrPatternedPolyLine(numpts: Integer; var poIntegers: array of Integer; lp: GrLinePatternPtr); AsmName 'GrPatternedPolyLine';
Procedure GrPatternedPolygon(numpts: Integer; var poIntegers: array of Integer; lp: GrLinePatternPtr); AsmName 'GrPatternedPolygon';

Procedure GrPatternFilledPlot(x, y: Integer; p: GrPatternPtr); AsmName 'GrPatternFilledPlot';
Procedure GrPatternFilledLine(x1, y1, x2, y2: Integer; p: GrPatternPtr); AsmName 'GrPatternFilledLine';
Procedure GrPatternFilledBox(x1, y1, x2, y2: Integer; p: GrPatternPtr); AsmName 'GrPatternFilledBox';
Procedure GrPatternFilledCircle(xc, yc, r: Integer; p: GrPatternPtr); AsmName 'GrPatternFilledCircle';
Procedure GrPatternFilledEllipse(xc, yc, xa, ya: Integer; p: GrPatternPtr); AsmName 'GrPatternFilledEllipse';
Procedure GrPatternFilledCircleArc(xc, yc, r, start, ende, style: Integer; p: GrPatternPtr); AsmName 'GrPatternFilledCircleArc';
Procedure GrPatternFilledEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; p: GrPatternPtr); AsmName 'GrPatternFilledEllipseArc';
Procedure GrPatternFilledConvexPolygon(numpts: Integer; var poIntegers: array of Integer; p: GrPatternPtr); AsmName 'GrPatternFilledConvexPolygon';
Procedure GrPatternFilledPolygon(numpts: Integer; var poIntegers: array of Integer; p: GrPatternPtr); AsmName 'GrPatternFilledPolygon';
Procedure GrPatternFloodFill(x, y: Integer; border: GrColor; p: GrPatternPtr); AsmName 'GrPatternFloodFill';

Procedure GrPatternDrawChar(chr, x, y: Integer; var opt: GrTextOption; p: GrPatternPtr); AsmName 'GrPatternDrawChar';
Procedure GrPatternDrawString(text: CString; length, x, y: Integer; var opt: GrTextOption; p: GrPatternPtr); AsmName 'GrPatternDrawString';
Procedure GrPatternDrawStringExt(text: CString; length, x, y: Integer; var opt: GrTextOption; p: GrPatternPtr); AsmName 'GrPatternDrawStringExt';


(*
**  <image.h>   - Image Utility
**                by Michal Stencl Copyright (c) 1998 for GRX
**
**
**  <e-mail>    - [stenclpmd@ba.telecom.sk]
**
*)

Type { Original GRX decl:
	 GrImage = GrPixmap;
	 GrImagePtr = ^GrImage;
       GPC isn't case sensitive so GrImage conficts with GrIMAGE above.
       Since we only need GrImagePtr we take a short cut here: }
     GrImagePtr = ^GrPixmap;


(* Flags for GrImageInverse() *)
const
  GR_IMAGE_INVERSE_LR = 1;  (* inverse left right *)
  GR_IMAGE_INVERSE_TD = 2;  (* inverse top down   *)

Function  GrImageBuild(pixels: CString; w, h: Integer; colors: GrColorTableP) : GrImagePtr; AsmName 'GrImageBuild';
Procedure GrImageDestroy(i: GrImagePtr); AsmName 'GrImageDestroy';
Procedure GrImageDisplay(x,y: Integer; i: GrImagePtr); AsmName 'GrImageDisplay';
Procedure GrImageDisplayExt(x1,y1,x2,y2: Integer; i: GrImagePtr); AsmName 'GrImageDisplayExt';
Procedure GrImageFilledBoxAlign(xo,yo,x1,y1,x2,y2: Integer; p: GrImagePtr); AsmName 'GrImageFilledBoxAlign';
Procedure GrImageHLineAlign(xo,yo,x,y,width: Integer; p: GrImagePtr); AsmName 'GrImageHLineAlign';
Procedure GrImagePlotAlign(xo,yo,x,y: Integer; p: GrImagePtr); AsmName 'GrImagePlotAlign';

Function  GrImageInverse(p: GrImagePtr; flag: Integer) : GrImagePtr; AsmName 'GrImageInverse';
Function  GrImageStretch(p: GrImagePtr; nwidth, nheight: Integer) : GrImagePtr; AsmName 'GrImageStretch';

Function  GrImageFromPattern(p: GrPatternPtr) : GrImagePtr; AsmName 'GrImageFromPattern';
Function  GrImageFromContext(c: GrContextPtr) : GrImagePtr; AsmName 'GrImageFromContext';
Function  GrImageBuildUsedAsPattern(pixels: CString; w, h: Integer; colors: GrColorTableP) : GrImagePtr; AsmName 'GrImageBuildUsedAsPattern';

Function  GrPatternFromImage(i: GrImagePtr) : GrPatternPtr; AsmName 'GrPatternFromImage';


{ ==================================================================
		 DRAWING IN USER WINDOW COORDINATES
  ================================================================== }

Procedure GrSetUserWindow( x1, y1, x2, y2: Integer);
Procedure GrGetUserWindow(var x1, y1, x2, y2: Integer);
Procedure GrGetScreenCoord(var x, y: Integer);
Procedure GrGetUserCoord(var x, y: Integer);

Procedure GrUsrPlot(x, y: Integer; c:GrColor); AsmName 'GrUsrPlot';
Procedure GrUsrLine(x1, y1, x2, y2: Integer; c:GrColor); AsmName 'GrUsrLine';
Procedure GrUsrHLine(x1, x2, y:Integer; c:GrColor); AsmName 'GrUsrHLine';
Procedure GrUsrVLine(x, y1, y2: Integer; c: GrColor); AsmName 'GrUsrVLine';
Procedure GrUsrBox(x1, y1, x2, y2: Integer; c: GrColor); AsmName 'GrUsrBox';
Procedure GrUsrFilledBox(x1, y1, x2, y2: Integer; c: GrColor); AsmName 'GrUsrFilledBox';
Procedure GrUsrFramedBox(x1, y1, x2, y2, wdt: Integer; c: GrFBoxColors); AsmName 'GrUsrFramedBox';

Procedure GrUsrCircle(xc, yc, r: Integer; c: GrColor); AsmName 'GrUsrCircle';
Procedure GrUsrEllipse(xc, yc, xa, ya: Integer; c: GrColor); AsmName 'GrUsrEllipse';
Procedure GrUsrCircleArc(xc, yc, r, start, ende, style: Integer; c: GrColor); AsmName 'GrUsrCircleArc';
Procedure GrUsrEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; c: GrColor); AsmName 'GrUsrEllipseArc';
Procedure GrUsrFilledCircle(xc, yc, r: Integer; c: GrColor); AsmName 'GrUsrFilledCircle';
Procedure GrUsrFilledEllipse(xc, yc, xa, ya: Integer; c: GrColor); AsmName 'GrUsrFilledEllipse';
Procedure GrUsrFilledCircleArc(xc, yc, r, start, ende, style: Integer; c: GrColor); AsmName 'GrUsrFilledCircleArc';
Procedure GrUsrFilledEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; c: GrColor); AsmName 'GrUsrFilledEllipseArc';
Procedure GrUsrPolyLine(numpts: Integer; var poIntegers: array of Integer; c: GrColor); AsmName 'GrUsrPolyLine';
Procedure GrUsrPolygon(numpts: Integer; var poIntegers:array of Integer; c: GrColor); AsmName 'GrUsrPolygon';
Procedure GrUsrFilledConvexPolygon(numpts: Integer; var poIntegers:array of Integer; c: GrColor); AsmName 'GrUsrFilledConvexPolygon';
Procedure GrUsrFilledPolygon(numpts: Integer; var poIntegers:array of Integer; c: GrColor); AsmName 'GrUsrFilledPolygon';
Procedure GrUsrFloodFill(x, y: Integer; border, c: GrColor); AsmName 'GrUsrFloodFill';

Function  GrUsrPixel(x, y:Integer):GrColor; AsmName 'GrUsrPixel';
Function  GrUsrPixelC(c: GrContextPtr; x, y: Integer):GrColor; AsmName 'GrUsrPixelC';

Procedure GrUsrCustomLine(x1, y1, x2, y2: Integer; var o: GrLineOption); AsmName 'GrUsrCustomLine';
Procedure GrUsrCustomBox(x1, y1, x2, y2: Integer; var o: GrLineOption); AsmName 'GrUsrCustomBox';
Procedure GrUsrCustomCircle(xc, yc, r: Integer; var o: GrLineOption); AsmName 'GrUsrCustomCircle';
Procedure GrUsrCustomEllipse(xc, yc, r: Integer; var o: GrLineOption); AsmName 'GrUsrCustomEllipse';
Procedure GrUsrCustomCircleArc(xc, yc, r, start, ende, style: Integer; var o: GrLineOption); AsmName 'GrUsrCustomCircleArc';
Procedure GrUsrCustomEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; var o: GrLineOption); AsmName 'GrUsrCustomEllipseArc';
Procedure GrUsrCustomPolyLine(numpts: Integer; var poIntegers: array of Integer; var o: GrLineOption); AsmName 'GrUsrCustomPolyLine';
Procedure GrUsrCustomPolygon(numpts: Integer; var poIntegers: array of Integer; var o: GrLineOption); AsmName 'GrUsrCustomPolygon';

Procedure GrUsrPatternedLine(x1, y1, x2, y2: Integer; lp: GrLinePatternPtr); AsmName 'GrUsrPatternedLine';
Procedure GrUsrPatternedBox(x1, y1, x2, y2: Integer; lp: GrLinePatternPtr); AsmName 'GrUsrPatternedBox';
Procedure GrUsrPatternedCircle(xc, yc, r: Integer; lp: GrLinePatternPtr); AsmName 'GrUsrPatternedCircle';
Procedure GrUsrPatternedEllipse(xc, yc, xa, ya: Integer; lp: GrLinePatternPtr); AsmName 'GrUsrPatternedEllipse';
Procedure GrUsrPatternedCircleArc(xc, yc, r, start, ende, style: Integer; lp: GrLinePatternPtr); AsmName 'GrUsrPatternedCircleArc';
Procedure GrUsrPatternedEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; lp: GrLinePatternPtr); AsmName 'GrUsrPatternedEllipseArc';
Procedure GrUsrPatternedPolyLine(numpts: Integer; var poIntegers: array of Integer; lp: GrLinePatternPtr); AsmName 'GrUsrPatternedPolyLine';
Procedure GrUsrPatternedPolygon(numpts: Integer; var poIntegers: array of Integer; lp: GrLinePatternPtr); AsmName 'GrUsrPatternedPolygon';

Procedure GrUsrPatternFilledPlot(x, y: Integer; p: GrPatternPtr); AsmName 'GrPatternFilledPlot';
Procedure GrUsrPatternFilledLine(x1, y1, x2, y2: Integer; p: GrPatternPtr); AsmName 'GrUsrPatternFilledLine';
Procedure GrUsrPatternFilledBox(x1, y1, x2, y2: Integer; p: GrPatternPtr); AsmName 'GrUsrPatternFilledBox';
Procedure GrUsrPatternFilledCircle(xc, yc, r: Integer; p: GrPatternPtr); AsmName 'GrUsrPatternFilledCircle';
Procedure GrUsrPatternFilledEllipse(xc, yc, xa, ya: Integer; p: GrPatternPtr); AsmName 'GrUsrPatternFilledEllipse';
Procedure GrUsrPatternFilledCircleArc(xc, yc, r, start, ende, style: Integer; p: GrPatternPtr); AsmName 'GrUsrPatternFilledCircleArc';
Procedure GrUsrPatternFilledEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; p: GrPatternPtr); AsmName 'GrUsrPatternFilledEllipseArc';
Procedure GrUsrPatternFilledConvexPolygon(numpts: Integer; var poIntegers: array of Integer; p: GrPatternPtr); AsmName 'GrUsrPatternFilledConvexPolygon';
Procedure GrUsrPatternFilledPolygon(numpts: Integer; var poIntegers: array of Integer; p: GrPatternPtr); AsmName 'GrUsrPatternFilledPolygon';
Procedure GrUsrPatternFloodFill(x, y: Integer; border: GrColor; p: GrPatternPtr); AsmName 'GrUsrPatternFloodFill';

Procedure GrUsrDrawChar(chr, x, y: Integer; var opt: GrTextOption); AsmName 'GrUsrDrawChar';
Procedure GrUsrDrawString(text: CString; length, x, y: Integer; var opt: GrTextOption); AsmName 'GrUsrDrawString';
Procedure GrUsrTextXY(x, y: Integer; text: CString; fg, bg: Integer); AsmName 'GrUsrTextXY';

{
==================================================================
		GRAPHICS CURSOR UTILITIES
==================================================================
}
Type
  { typedef struct _GR_cursor }
  GrCursor = record
	work         : GrContext;  { work areas (4) }
	xcord,ycord  : Integer;    { cursor position on screen }
	xsize,ysize  : Integer;    { cursor size }
	xoffs,yoffs  : Integer;    { LU corner to hot poInteger offset }
	xwork,ywork  : Integer;    { save/work area sizes }
	xwpos,ywpos  : Integer;    { save/work area position on screen }
	displayed    : Integer;    { set if displayed }
  end;
  GrCursorPtr = ^GrCursor;

Function  GrBuildCursor(pixels: Pointer; pitch, w, h, xo, yo: Integer; c: GrColorTableP): GrCursorPtr; AsmName 'GrBuildCursor';
Procedure GrDestroyCursor(cursor: GrCursorPtr); AsmName 'GrDestroyCursor';
Procedure GrDisplayCursor(cursor: GrCursorPtr); AsmName 'GrDisplayCursor';
Procedure GrEraseCursor(cursor: GrCursorPtr); AsmName 'GrEraseCursor';
Procedure GrMoveCursor(cursor: GrCursorPtr; x, y: Integer); AsmName 'GrMoveCursor';

{ ================================================================== }
{      MOUSE AND KEYBOARD INPUT UTILITIES                            }
{ ================================================================== }

Const
  GR_M_MOTION        = $0001;      { mouse event flag bits }
  GR_M_LEFT_DOWN     = $0002;
  GR_M_LEFT_UP       = $0004;
  GR_M_RIGHT_DOWN    = $0008;
  GR_M_RIGHT_UP      = $0010;
  GR_M_MIDDLE_DOWN   = $0020;
  GR_M_MIDDLE_UP     = $0040;
  GR_M_BUTTON_DOWN   = (GR_M_LEFT_DOWN or GR_M_MIDDLE_DOWN or GR_M_RIGHT_DOWN);
  GR_M_BUTTON_UP     = (GR_M_LEFT_UP   or GR_M_MIDDLE_UP   or GR_M_RIGHT_UP);
  GR_M_BUTTON_CHANGE = (GR_M_BUTTON_UP or GR_M_BUTTON_DOWN);

  GR_M_LEFT          = 1;       { mouse button index bits }
  GR_M_RIGHT         = 2;
  GR_M_MIDDLE        = 4;

  GR_M_KEYPRESS      = $0080;      { other event flag bits }
  GR_M_POLL          = $0100;
  GR_M_NOPAINT       = $0200;
  GR_SIZE_CHANGED    = $0400;
  GR_M_EVENT         = (GR_M_MOTION or GR_M_KEYPRESS or GR_M_BUTTON_DOWN or GR_M_BUTTON_UP or GR_SIZE_CHANGED);

  GR_KB_RIGHTSHIFT   = $01;    { Keybd states: right shift key depressed }
  GR_KB_LEFTSHIFT    = $02;    { left shift key depressed }
  GR_KB_CTRL         = $04;    { CTRL depressed }
  GR_KB_ALT          = $08;    { ALT depressed }
  GR_KB_SCROLLOCK    = $10;    { SCROLL LOCK active }
  GR_KB_NUMLOCK      = $20;    { NUM LOCK active }
  GR_KB_CAPSLOCK     = $40;    { CAPS LOCK active }
  GR_KB_INSERT       = $80;    { INSERT state active }
  GR_KB_SHIFT        = (GR_KB_LEFTSHIFT or GR_KB_RIGHTSHIFT);

  GR_M_CUR_NORMAL    = 0;       { MOUSE CURSOR modes: just the cursor }
  GR_M_CUR_RUBBER    = 1;       { rectangular rubber band (XOR-d to the screen) }
  GR_M_CUR_LINE      = 2;       { line attached to the cursor }
  GR_M_CUR_BOX       = 3;       { rectangular box dragged by the cursor }
  GR_M_CUR_CROSS     = 4;       { crosshair }

  GR_M_QUEUE_SIZE    = 128;     { default queue size }

Type
  { typedef struct _GR_mouseEvent }         { mouse event buffer structure }
  GrMouseEvent = record
	flags   : Integer;           { event type flags (see above) }
	x,y     : Integer;           { mouse coordinates }
	buttons : Integer;           { mouse button state }
	key     : Integer;           { key code from keyboard }
	kbstat  : Integer;           { keybd status (ALT, CTRL, etc..) }
	dtime   : Integer;           { time since last event (msec) }
  end;
  GrMouseEventPtr = ^GrMouseEvent;

Function  GrMouseDetect: Boolean; AsmName 'GrMouseDetect';
Procedure GrMouseEventMode(dummy: Integer); AsmName 'GrMouseEventMode';
Procedure GrMouseInit; AsmName 'GrMouseInit';
Procedure GrMouseInitN(queue_size: Integer); AsmName 'GrMouseInitN';
Procedure GrMouseUnInit; AsmName 'GrMouseUnInit';
Procedure GrMouseSetSpeed(spmult, spdiv: Integer); AsmName 'GrMouseSetSpeed';
Procedure GrMouseSetAccel(thresh, accel: Integer); AsmName 'GrMouseSetAccel';
Procedure GrMouseSetLimits(x1, y1, x2, y2: Integer); AsmName 'GrMouseSetLimits';
Procedure GrMouseGetLimits(var x1, y1, x2, y2: Integer); AsmName 'GrMouseGetLimits';
Procedure GrMouseWarp(x, y: Integer); AsmName 'GrMouseWarp';
Procedure GrMouseEventEnable(enable_kb, enable_ms: Boolean); AsmName 'GrMouseEventEnable';
Procedure GrMouseGetEvent(flags: Integer; event: GrMouseEventPtr); AsmName 'GrMouseGetEvent';
Procedure GrMouseGetEventT(flags: Integer; event: GrMouseEventPtr; timout_msecs: Integer); AsmName 'GrMouseGetEventT';
Function  GrMousePendingEvent: Integer; AsmName 'GrMousePendingEvent';

Function  GrMouseGetCursor: GrCursorPtr; AsmName 'GrMouseGetCursor';
Procedure GrMouseSetCursor(cursor: GrCursorPtr); AsmName 'GrMouseSetCursor';
Procedure GrMouseSetColors(fg,bg: GrColor); AsmName 'GrMouseSetColors';
Procedure GrMouseSetCursorMode(mode: Integer; x1, y1, x2, y2: Integer; c: Integer); AsmName 'GrMouseSetCursorMode';
Procedure GrMouseDisplayCursor; AsmName 'GrMouseDisplayCursor';
Procedure GrMouseEraseCursor; AsmName 'GrMouseEraseCursor';
Procedure GrMouseUpdateCursor; AsmName 'GrMouseUpdateCursor';
Function  GrMouseCursorIsDisplayed: Boolean; AsmName 'GrMouseCursorIsDisplayed';

Function  GrMouseBlock(c: GrContextPtr; x1, y1, x2, y2: Integer): Integer; AsmName 'GrMouseBlock';
Procedure GrMouseUnBlock(return_value_from_GrMouseBlock: Integer); AsmName 'GrMouseUnBlock';


(* ================================================================== *)
(* --------------         keyboard interface         ---------------- *)
(* ================================================================== *)
const
	 GrKey_NoKey              = $0000;  (* no key available *)
	 GrKey_OutsideValidRange  = $0100;  (* key typed but code outside 1..GrKey_LastDefinedKeycode *)

	 (* standard ASCII key codes *)
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
	 GrKey_small_a            = $0061;
	 GrKey_small_b            = $0062;
	 GrKey_small_c            = $0063;
	 GrKey_small_d            = $0064;
	 GrKey_small_e            = $0065;
	 GrKey_small_f            = $0066;
	 GrKey_small_g            = $0067;
	 GrKey_small_h            = $0068;
	 GrKey_small_i            = $0069;
	 GrKey_small_j            = $006a;
	 GrKey_small_k            = $006b;
	 GrKey_small_l            = $006c;
	 GrKey_small_m            = $006d;
	 GrKey_small_n            = $006e;
	 GrKey_small_o            = $006f;
	 GrKey_small_p            = $0070;
	 GrKey_small_q            = $0071;
	 GrKey_small_r            = $0072;
	 GrKey_small_s            = $0073;
	 GrKey_small_t            = $0074;
	 GrKey_small_u            = $0075;
	 GrKey_small_v            = $0076;
	 GrKey_small_w            = $0077;
	 GrKey_small_x            = $0078;
	 GrKey_small_y            = $0079;
	 GrKey_small_z            = $007a;
	 GrKey_LBrace             = $007b;
	 GrKey_Pipe               = $007c;
	 GrKey_RBrace             = $007d;
	 GrKey_Tilde              = $007e;
	 GrKey_Control_Backspace  = $007f;

	 (* extended key codes as defined in DJGPP *)
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

	 (* some additional codes not in DJGPP *)
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

	 (* this may be usefull for table allocation ... *)
	 GrKey_LastDefinedKeycode = GrKey_Shift_Left;

	 (* some well known synomyms *)
	 GrKey_BackSpace          = GrKey_Control_H;
	 GrKey_Tab                = GrKey_Control_I;
	 GrKey_LineFeed           = GrKey_Control_J;
	 GrKey_Escape             = GrKey_Control_LBracket;
	 GrKey_Return             = GrKey_Control_M;

Type
  GrKeyType = ShortInt;

(*
** new functions to replace the old style
**   kbhit / getch / getkey / getxkey / getkbstat
** keyboard interface
*)
Function GrKeyPressed: Integer; AsmName 'GrKeyPressed';
Function GrKeyRead: GrKeyType; AsmName 'GrKeyRead';
Function GrKeyStat: Integer; AsmName 'GrKeyStat';


(* ================================================================== *)
(*                            ADDON FUNCTIONS                         *)
(*  these functions may not be installed or available on all system   *)
(* ================================================================== *)

(*
** SaveContextToTiff - Dump a context in a TIFF file
**
** Arguments:
**   cxt:   Context to be saved (NIL -> use current context)
**   tiffn: Name of tiff file
**   compr: Compression method (see tiff.h), 0: automatic selection
**   docn:  string saved in the tiff file (DOCUMENTNAME tag)
**
**  Returns  0 on success
**          -1 on error
**
** requires tifflib by  Sam Leffler (sam@engr.sgi.com)
**        available at  ftp://ftp.sgi.com/graphics/tiff
*)
Function SaveContextToTiff(cxt: GrContextPtr; tiffn: CString;
				compr: Integer; docn:  CString) : Integer;
  AsmName 'SaveContextToTiff';

(*
** SaveContextToJpeg - Dump a context in a JPEG file
**
** Arguments:
**   cxt:      Context to be saved (NULL -> use current context)
**   jpegn:    Name of the jpeg file
**   accuracy: Accuracy percentage (100 for no loss of quality)
**
**  Returns  0 on success
**          -1 on error
**
** requires jpeg-6a by  IJG (Independent JPEG Group)
**        available at  ftp.uu.net as graphics/jpeg/jpegsrc.v6a.tar.gz
*)
Function SaveContextToJpeg(cxt: GrContextPtr; jpegn: CString; accuracy: Integer) : Integer;
  AsmName 'SaveContextToJpeg';

Implementation
end.

