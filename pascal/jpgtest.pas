(**
 ** jpgtest.pas ---- test the ctx2jpeg routines
 **
 ** Copyright (c) 2001 Mariano Alvarez Fernandez
 ** [e-mail: malfer@teleline.es]
 **
 ** This is a test/demo file of the GRX graphics library.
 ** You can use GRX test/demo files as you want.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **)

{$X+}

program jpgtest;

uses gpc, grx;

var grc: GrContextPtr;

procedure imagen( nf: String; scale: Integer );
var
  grc        : GrContextPtr;
  wide, high : Integer;
  s          : String[81];
  w, h       : Integer;
begin
  GrQueryJpeg( nf,w,h );
  WriteStr( s,nf,' ',w,' ',h,' scale 1/',scale );
  wide := min(600 , w div scale);
  high := min(400 , h div scale);
  GrClearScreen( GrAllocColor( 0,0,200 ) );

  GrBox( 10,40,10+wide+1,40+high+1,GrWhite );
  grc := GrCreateSubContext( 11,41,11+wide-1,41+high-1,NIL,NIL );
  GrLoadContextFromJpeg( grc,nf,scale );
  GrDestroyContext( grc );

  GrTextXY( 10,10,s,GrBlack,GrWhite );
  GrTextXY( 10,50+high,'Press any key to continue',GrBlack,GrWhite );
  GrKeyRead;
end;

procedure nojpegsupport;
const
  s: array[0..5] of string[50] = (
		 'Warning!',
		 'You need libjpeg (http://www.ijg.org) and enable',
		 'jpeg support in the GRX lib (edit makedefs.grx)',
		 'to run this demo',
		 ' ',
		 'Press any key to continue...' );
var 
  i: integer;
begin
  GrClearScreen( GrAllocColor( 0,0,100 ) );
  for i:=0 to 5 do
    GrTextXY( 90,160+i*18,s[i],GrWhite,GrNOCOLOR );
  GrKeyRead;
end;

begin
   GrSetMode( GR_width_height_bpp_graphics,640,480,24,0,0 );

   if GrJpegSupport = 0 then begin
		nojpegsupport;
		GrSetMode(GR_default_text,0,0,0,0,0);
		halt( 1 );
   end;

   imagen( '..\test\jpeg1.jpg',1 );
   imagen( '..\test\jpeg1.jpg',2 );
   imagen( '..\test\jpeg1.jpg',4 );
   imagen( '..\test\jpeg1.jpg',8 );
   imagen( '..\test\jpeg2.jpg',1 );
   imagen( '..\test\jpeg2.jpg',2 );
   imagen( '..\test\jpeg2.jpg',4 );
   imagen( '..\test\jpeg2.jpg',8 );

   GrClearScreen( GrAllocColor( 0,100,0 ) );
   grc := GrCreateSubContext( 10,40,10+400-1,40+300-1,NIL,NIL );
   GrLoadContextFromJpeg( grc,'..\test\jpeg1.jpg',2 );
   GrDestroyContext( grc );
   grc := GrCreateSubContext( 210,150,210+400-1,150+300-1,NIL,NIL );
   GrLoadContextFromJpeg( grc,'..\test\jpeg2.jpg',2 );
   GrDestroyContext( grc );

   GrTextXY( 10,10,'Press any key to save color and gray screen',GrBlack,GrWhite );
   GrKeyRead;

   GrSaveContextToJpeg( NIL,'p.jpg',75 );
   GrSaveContextToGrayJpeg( NIL,'pgray.jpg',75 );

   GrClearScreen( GrBlack );
   GrTextXY( 10,10,'Press any key to reload color screen       ',GrBlack,GrWhite );
   GrKeyRead;
   GrLoadContextFromJpeg( NIL,'p.jpg',1 );

   GrTextXY( 10,10,'Press any key to reload gray screen        ',GrBlack,GrWhite );
   GrKeyRead;
   GrClearScreen(GrBlack);
   GrLoadContextFromJpeg( NIL,'pgray.jpg',1 );

   GrTextXY( 10,10,'Press any key to end                       ',GrBlack,GrWhite );
   GrKeyRead;

   GrSetMode(GR_default_text,0,0,0,0,0);
end.
