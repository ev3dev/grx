Program Blt_Test;

uses GRX;

procedure TestFunc;
var
  x, y, ww, wh, ii, jj : Integer;

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
   x, y, xv, yv, BPP,
   m : Integer;

begin
   x  :=  1024;
   y  :=  768;
   xv :=  1024;
   yv :=  1024;
   BPP:=  8;

(* M := GrSetMode(GR_Width_Height_BPP_Graphics,x,y,bpp,0,0); *)
   m := GrSetMode(Gr_Custom_BPP_Graphics,x,y,BPP,xv,yv);

   TestFunc;
   GrCircle(400,400,200,GrWhite);
   GrCircle(400,400,205,GrWhite);
   GrLineNC(0, 0, GrScreenX, GrScreenY, GrWhite);
   GrLineNC(0, GrScreenY, GrScreenX, 0, GrWhite);
   readln;
   writeln('GrScreenContext');
   GrBitBltNC(GrScreenContext, 200, 200, GrScreenContext, 0, 0, 200, 200, GrWrite);
   readln;
   GrBitBltNC(GrScreenContext, 300, 300, GrScreenContext, 0, 0, 200, 200, GrOr);
   readln;
   GrBitBltNC(GrScreenContext, 400, 400, GrScreenContext, 0, 0, 200, 200, GrAnd);
   readln;
   GrBitBltNC(GrScreenContext, 500, 500, GrScreenContext, 0, 0, 200, 200, GrXor);
   readln;
   GrBitBltNC(GrScreenContext, 600, 600, GrScreenContext, 0, 0, 200, 200, GrImage);
   readln;
end.

