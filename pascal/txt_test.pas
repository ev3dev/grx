Program Text_Test;
 
uses
  grx;

procedure TestFunc;
var
  x, y, ww, wh, ii, jj : integer;

begin
   x := GrSizeX;
   y := GrSizeY;
   ww := round((x-10)/32);
   wh := round((y-10)/8);

   GrSetRGBcolorMode;
   for ii := 0 to 7 do
     for jj := 0 to 31 do
      GrFilledBox(5+jj*ww,5+ii*wh,5+jj*ww+ww-1,5+ii*wh+wh-1,ii*32+jj);
end; { TestFunc }

var
   i, j,
   x, y, xv, yv, c,
   m : Integer;
   { St: WrkStr; }
   St : String(255);
   f : GrFont;
   o  : GrTextOption;
   Col: GrTextColor;
   MyCo: GrContextPtr;

begin
   x  :=  640;
   y  :=  480;
   xv :=  640;
   yv :=  480;
   c  :=  256;

   M := GrSetMode(GR_width_height_color_graphics,x,y,c,xv,yv);
   { M := GrSetMode(GR_custom_graphics,x,y,c,xv,yv); }

   TestFunc;
   GrCircle(400,400,200,GrWhite);
   GrCircle(400,400,205,GrWhite);
   GrLineNC(0, 0, GrScreenX, GrScreenY, GrWhite);
   GrLineNC(0, GrScreenY, GrScreenX, 0, GrWhite);

   { o.txo_font    := @GrFont_PC8x16; }
   o.txo_font    := GrLoadFont('pc8x16.fnt');
   o.txo_fgcolor.v := GrBlack;
   o.txo_bgcolor.v := GrWhite;
   o.txo_chrtype := GR_BYTE_TEXT;
   o.txo_direct  := GR_TEXT_DEFAULT;
   o.txo_xalign  := GR_ALIGN_DEFAULT;
   o.txo_yalign  := GR_ALIGN_DEFAULT;

   GrDrawString('This is simple text', 16, 20, 20, o);
   St := 'This text is more complex';
   o.txo_font    := @GrFont_PC8x8;
   GrDrawString(St, Length(St), 20, 50, o);
   GrTextXY(20, 80, 'This is another text', GrBlack, GrWhite);

   o.txo_font    := GrLoadFont('helv22.fnt');
   GrDrawString(St, Length(St), 20, 100, o);

   { GrLoadConvertedFont(var name: Char; cvt, w, h, minch, maxch: Integer): GrFontPt;}
   o.txo_font    := GrLoadConvertedFont('helv22.fnt',
                  { GR_FONTCVT_NONE       =  0;
                    GR_FONTCVT_SKIPCHARS  =  1;
                    GR_FONTCVT_RESIZE     =  2;
                    GR_FONTCVT_ITALICIZE  =  4;
                    GR_FONTCVT_BOLDIFY    =  8;
                    GR_FONTCVT_FIXIFY     = 16;
                    GR_FONTCVT_PROPORTION = 32; }
                    GR_FONTCVT_RESIZE     or
                    GR_FONTCVT_ITALICIZE,
                    20, 30,
                    32, 127);
   GrDrawString(St, Length(St), 20, 130, o);

   readln;
end.
