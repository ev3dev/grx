Unit GRX20;

{ Converted by Sven Hilscher eMail sven@rufus.central.de
  from Headerfile grx20.h }

{*
 ** GRX20.H ---- GRX 2.0 API functions and data structure declarations
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 *}

Interface

Type
  MemPtr = ^Char;

Const
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

  GR_MAX_POLYGON_POINTERS = 1000000;
  GR_MAX_ELLIPSE_POINTERS = 1024 + 5;
  GR_MAX_ANGLE_VALUE      = 3600;
  GR_ARC_STYLE_OPEN       = 0;
  GR_ARC_STYLE_CLOSE1     = 1;
  GR_ARC_STYLE_CLOSE2     = 2;

{ The video driver descriptor structure }

Type
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
    { the order of the next three is kinda important!!! }
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
    deftc,defgc : Integer;        { default text and graphics colors }
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
  GrDriverInfo : __asmname__ 'GrDriverInfo'  ^GrDriverInfoType;

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
  GrContextInfo : __asmname__ 'GrContextInfo' GrContextInfoType;

Function  GrCreateContext(w, h: Integer; memory: MemPtr; var where: GrContext): GrContextPtr; AsmName 'GrCreateContext';
Function  GrCreateFrameContext(md: ByteCard; w, h: Integer; memory: MemPtr; var where: GrContext): GrContextPtr; AsmName 'GrCreateFrameContext';
Function  GrCreateSubContext(x1, y1, x2, y2: Integer; var parent, where: GrContext): GrContextPtr; AsmName 'GrCreateSubContext';
Function  GrSaveContext(var where: GrContext): GrContextPtr; AsmName 'GrSaveContext';
Function  GrCurrentContext: GrContextPtr; AsmName 'GrCurrentContext';
Function  GrScreenContext : GrContextPtr; AsmName 'GrScreenContext';

Procedure GrDestroyContext(var context: GrContext); AsmName 'GrDestroyContext';
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

Function  GrColorValue(c:Integer):Integer; AsmName 'GrColorValue';
Function  GrColorMode(c:Integer):Integer; AsmName 'GrColorMode';
Function  GrWriteModeColor(c:Integer):Integer; AsmName 'GrWriteModeColor';
Function  GrXorModeColor(c:Integer):Integer; AsmName 'GrXorModeColor';
Function  GrOrModeColor(c:Integer):Integer; AsmName 'GrOrModeColor';
Function  GrAndModeColor(c:Integer):Integer; AsmName 'GrAndModeColor';
Function  GrImageModeColor(c:Integer):Integer; AsmName 'GrImageModeColor';
Procedure GrResetColors; AsmName 'GrResetColors';
Procedure GrSetRGBcolorMode; AsmName 'GrSetRGBcolorMode';
Procedure GrRefreshColors; AsmName 'GrRefreshColors';
Function  GrNumColors:Integer; AsmName 'GrNumColors';
Function  GrNumFreeColors:Integer; AsmName 'GrNumFreeColors';
Function  GrBlack:Integer; AsmName 'GrBlack';
Function  GrWhite:Integer; AsmName 'GrWhite';

Function  GrBuildRGBcolorT(r,g,b:Integer):Integer; AsmName 'GrBuildRGBcolorT';
Function  GrBuildRGBcolorR(r,g,b:Integer):Integer; AsmName 'GrBuildRGBcolorR';
Function  GrRGBcolorRed(c:Integer):Integer ; AsmName 'GrRGBcolorRed';
Function  GrRGBcolorGreen(c:Integer):Integer ; AsmName 'GrRGBcolorGreen';
Function  GrRGBcolorBlue(c:Integer):Integer ; AsmName 'GrRGBcolorBlue';

Function  GrAllocColor(r,g,b:Integer):Integer; AsmName 'GrAllocColor'; { shared, read-only }
Function  GrAllocColorID(r,g,b:Integer):Integer; AsmName 'GrAllocColorID'; { potentially inlined version }
Function  GrAllocCell:Integer; AsmName 'GrAllocCell'; { unshared, read-write }

Procedure GrSetColor(c:Integer; r,g,b:Integer); AsmName 'GrSetColor';
Procedure GrFreeColor(c:Integer); AsmName 'GrFreeColor';
Procedure GrFreeCell(c:Integer);  AsmName 'GrFreeCell';

Procedure GrQueryColor(c:Integer; var r,g,b:Integer); AsmName 'GrQueryColor';
Procedure GrQueryColorID(c:Integer; var r,g,b:Integer); AsmName 'GrQueryColorID';

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

Procedure GrClearScreen(bg:Integer); AsmName 'GrClearScreen';
Procedure GrClearClipBox(bg:Integer); AsmName 'GrClearClipBox';
Procedure GrPlot(x, y: Integer; c:Integer); AsmName 'GrPlot';
Procedure GrLine(x1, y1, x2, y2: Integer; c:Integer); AsmName 'GrLine';
Procedure GrHLine(x1, x2, y:Integer; c:Integer); AsmName 'GrHLine';
Procedure GrVLine(x, y1, y2: Integer; c: Integer); AsmName 'GrVLine';
Procedure GrBox(x1, y1, x2, y2: Integer; c: Integer); AsmName 'GrBox';
Procedure GrFilledBox(x1, y1, x2, y2: Integer; c: Integer); AsmName 'GrFilledBox';
Procedure GrFramedBox(x1, y1, x2, y2, wdt: Integer; c: GrFBoxColors); AsmName 'GrFramedBox';
Function  GrGenerateEllipse(xc, yc, xa, ya:Integer; var poIntegers: array of Integer):Integer; AsmName 'GrGenerateEllipse';
Function  GrGenerateEllipseArc(xc, yc, xa, ya, start, ende: Integer; var poIntegers: array of Integer):Integer; AsmName 'GrGenerateEllipseArc';
Procedure GrLastArcCoords(var xs, ys, xe, ye, xc, yc: Integer); AsmName 'GrLastArcCoords';
Procedure GrCircle(xc, yc, r: Integer; c: Integer); AsmName 'GrCircle';
Procedure GrEllipse(xc, yc, xa, ya: Integer; c: Integer); AsmName 'GrEllipse';
Procedure GrCircleArc(xc, yc, r, start, ende, style: Integer; c: Integer); AsmName 'GrCircleArc';
Procedure GrEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; c: Integer); AsmName 'GrEllipseArc';
Procedure GrFilledCircle(xc, yc, r: Integer; c: Integer); AsmName 'GrFilledCircle';
Procedure GrFilledEllipse(xc, yc, xa, ya: Integer; c: Integer); AsmName 'GrFilledEllipse';
Procedure GrFilledCircleArc(xc, yc, r, start, ende, style: Integer; c: Integer); AsmName 'GrFilledCircleArc';
Procedure GrFilledEllipseArc(xc, yc, xa, ya, start, ende, style: Integer; c: Integer); AsmName 'GrFilledEllipseArc';
Procedure GrPolyLine(numpts: Integer; var poIntegers: array of Integer; c: Integer); AsmName 'GrPolyLine';
Procedure GrPolygon(numpts: Integer; var poIntegers:array of Integer; c: Integer); AsmName 'GrPolygon';
Procedure GrFilledConvexPolygon(numpts: Integer; var poIntegers:array of Integer; c: Integer); AsmName 'GrFilledConvexPolygon';
Procedure GrFilledPolygon(numpts: Integer; var poIntegers:array of Integer; c: Integer); AsmName 'GrFilledPolygon';
Procedure GrBitBlt(dst: GrContextPtr; x, y: Integer; src: GrContextPtr; x1, y1, x2, y2: Integer; op: Integer); AsmName 'GrBitBlt';
Function  GrPixel(x, y:Integer):Integer; AsmName 'GrPixel';
Function  GrPixelC(c: GrContextPtr; x, y: Integer): Integer; AsmName 'GrPixelC';

{
  ==================================================================
	NON CLIPPING DRAWING PRIMITIVES
  ==================================================================
}
Procedure GrPlotNC(x, y: Integer; c: Integer); AsmName 'GrPlotNC';
Procedure GrLineNC(x1, y1, x2, y2: Integer; c: Integer); AsmName 'GrLineNC';
Procedure GrHLineNC(x1, x2, y: Integer; c: Integer); AsmName 'GrHLineNC';
Procedure GrVLineNC(x, y1, y2: Integer; c: Integer); AsmName 'GrVLineNC';
Procedure GrBoxNC(x1, y1, x2, y2: Integer; c: Integer); AsmName 'GrBoxNC';
Procedure GrFilledBoxNC(x1, y1, x2, y2: Integer; c: Integer); AsmName 'GrFilledBoxNC';
Procedure GrFramedBoxNC(x1, y1, x2, y2, wdt: Integer; c: GrFBoxColors); AsmName 'GrFramedBoxNC';
Procedure GrBitBltNC(dst: GrContextPtr; x, y: Integer; src: GrContextPtr; x1, y1, x2, y2: Integer; op: Integer); AsmName 'GrBitBltNC';
Function  GrPixelNC(x, y:Integer):Integer; AsmName 'GrPixelNC';
Function  GrPixelCNC(c: GrContextPtr; x, y: Integer): Integer; AsmName 'GrPixelCNC';

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
  GrFont_PC6x8  : __asmname__ 'GrFont_PC6x8'  GrFont;
  GrFont_PC8x8  : __asmname__ 'GrFont_PC8x8'  GrFont;
  GrFont_PC8x14 : __asmname__ 'GrFont_PC8x14' GrFont;
  GrFont_PC8x16 : __asmname__ 'GrFont_PC8x16' GrFont;
  GrDefaultFont : __asmname__ 'GrFont_PC8x14' GrFont;

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
      1: (v : Integer);           { color value for "direct" text }
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
Procedure GrTextXY(x, y: Integer; text: CString; fg, bg: Integer); AsmName 'GrTextXY';

Procedure GrDumpChar(chr, col, row: Integer; var r: GrTextRegion); AsmName 'GrDumpChar';
Procedure GrDumpText(col, row, wdt, hgt: Integer; var r: GrTextRegion); AsmName 'GrDumpText';
Procedure GrDumpTextRegion(var r: GrTextRegion); AsmName 'GrDumpTextRegion';


{
==================================================================
	  THICK AND DASHED LINE DRAWING PRIMITIVES

  !!!  not yet available !!! (SH)

==================================================================

{
 * custom line option structure
 *   zero or one dash pattern length means the line is continuous
 *   the dash pattern always begins with a drawn section
 }

Type
  GrLineOption = record
    no_color    : Integer;   { color used to draw line }
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

{
==================================================================
	 PATTERNED DRAWING AND FILLING PRIMITIVES

	 !!!  not yet available !!! (SH)

==================================================================
}

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
    bmp_fgcolor  : Integer;      { foreground color for fill }
    bmp_bgcolor  : Integer;      { background color for fill }
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
    pxp_oper     : Integer;     { bitblt mode (SET, OR, XOR, AND, IMAGE) }
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
Function  GrBuildPixmapFromBits(bits: CString; w, h: Integer; fgc, bgc: Integer): GrPatternPtr; AsmName 'GrBuildPixmapFromBits';
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

{
==================================================================
	   DRAWING IN USER WINDOW COORDINATES
	 !!!  not yet available !!! (SH)
==================================================================
}


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
  GrCursorPtr = GrCursor;

Function  GrBuildCursor(pixels: CString; pitch, w, h, xo, yo: Integer; c: GrColorTableP): GrCursorPtr; AsmName 'GrBuildCursor';
Procedure GrDestroyCursor(cursor: GrCursorPtr); AsmName 'GrDestroyCursor';
Procedure GrDisplayCursor(cursor: GrCursorPtr); AsmName 'GrDisplayCursor';
Procedure GrEraseCursor(cursor: GrCursorPtr); AsmName 'GrEraseCursor';
Procedure GrMoveCursor(cursor: GrCursorPtr; x, y: Integer); AsmName 'GrMoveCursor';

{ ================================================================== }
{      MOUSE AND KEYBOARD INPUT UTILITIES          }
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
  GR_M_EVENT         = (GR_M_MOTION or GR_M_KEYPRESS or GR_M_BUTTON_DOWN or GR_M_BUTTON_UP);

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
Procedure GrMouseSetColors(fg,bg: Integer); AsmName 'GrMouseSetColors';
Procedure GrMouseSetCursorMode(mode: Integer; x1, y1, x2, y2: Integer; c: Integer); AsmName 'GrMouseSetCursorMode';
Procedure GrMouseDisplayCursor; AsmName 'GrMouseDisplayCursor';
Procedure GrMouseEraseCursor; AsmName 'GrMouseEraseCursor';
Procedure GrMouseUpdateCursor; AsmName 'GrMouseUpdateCursor';
Function  GrMouseCursorIsDisplayed: Boolean; AsmName 'GrMouseCursorIsDisplayed';

Function  GrMouseBlock(c: GrContextPtr; x1, y1, x2, y2: Integer): Integer; AsmName 'GrMouseBlock';
Procedure GrMouseUnBlock(return_value_from_GrMouseBlock: Integer); AsmName 'GrMouseUnBlock';


Implementation
end.

