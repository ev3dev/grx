(**
 ** polytest.pas ---- test polygon rendering
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
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
 **
 **)

program polytest;

uses gpc, grx, test;

type EGA             = array [0..63] of GrColor;
     EGAPtr          = ^EGA;
     WrkString       = string[80];

var  f               : text;
     line            : WrkString;
     nb              : integer;
     convex,collect  : boolean;
     k               : GrKeyType;
     polygon         : array [0..300] of PointType;
     pEGA            : EGAPtr;
     black,white,red : GrColor;

procedure TestPoly(n:integer; var points: array of PointType; convex:boolean);
begin
   GrClearScreen(black);
   GrPolygon(n,points,white);
   GrFilledPolygon(n,points,(red or GrXOR));
   k:=GrKeyRead;
   if convex or (n <= 3) then begin
      GrClearScreen(black);
      GrFilledPolygon(n,points,white);
      GrFilledConvexPolygon(n,points,(red or GrXOR));
      k:=GrKeyRead;
   end;
end;

procedure SpeedTest;
var
   pts : array[0..3,0..1] of integer;
   ww  : integer = GrSizeX div 10;
   hh  : integer = GrSizeY div 10;
   sx  : integer = (GrSizeX - 2*ww) div 32;
   sy  : integer = (GrSizeY - 2*hh) div 32;
   ii,jj : integer;
   color : GrColor;
   t1,t2,t3,mu1,mu2,mu3: integer;
begin
   GrClearScreen(black);
   t1 := GetCPUTime(mu1);
   pts[0][1] := 0;
   pts[1][1] := hh;
   pts[2][1] := 2*hh;
   pts[3][1] := hh;
   color := 0;
   for ii := 0 to 31 do begin
      pts[0][0] := ww;
      pts[1][0] := 2*ww;
      pts[2][0] := ww;
      pts[3][0] := 0;
      for jj := 0 to 31 do begin
         GrFilledPolygon(4,pts, pEGA^[color] or GrXOR);
         color := (color + 1) and 15;
         inc(pts[0][0],sx);
         inc(pts[1][0],sx);
         inc(pts[2][0],sx);
         inc(pts[3][0],sx);
      end;
      inc(pts[0][1],sy);
      inc(pts[1][1],sy);
      inc(pts[2][1],sy);
      inc(pts[3][1],sy);
   end;
   t2 := GetCPUTime(mu2);
   pts[0][1] := 0;
   pts[1][1] := hh;
   pts[2][1] := 2*hh;
   pts[3][1] := hh;
   color := 0;
   for ii := 0 to 31 do begin
      pts[0][0] := ww;
      pts[1][0] := 2*ww;
      pts[2][0] := ww;
      pts[3][0] := 0;
      for jj := 0 to 31 do begin
         GrFilledConvexPolygon(4,pts, pEGA^[color] or GrXOR);
         color := (color + 1) and 15;
         inc(pts[0][0],sx);
         inc(pts[1][0],sx);
         inc(pts[2][0],sx);
         inc(pts[3][0],sx);
      end;
      inc(pts[0][1],sy);
      inc(pts[1][1],sy);
      inc(pts[2][1],sy);
      inc(pts[3][1],sy);
   end;
   t3 := GetCPUTime(mu3);
   writestr(exit_message,
      "Times to scan 1024 polygons\n",
      '   with GrFilledPolygon: ',(t2+mu2/1e6)-(t1+mu1/1e6):0:2," (s)\n",
      '   with GrFilledConvexPolygon: ',(t3+mu3/1e6)-(t2+mu2/1e6):0:2," (s)\n");
end;

begin
   InitTest;
   pEGA:=EGAPtr(GrAllocEgaColors);
   black:=pEGA^[0]; red:=pEGA^[12]; white:=pEGA^[15];

   assign(f,'../test/polytest.dat'); reset(f);
   collect:=false;
   while not eof(f) do begin
      readln(f,line);
      if not collect then begin
         if Copy(line,1,5)='begin' then begin
            collect:=true;
            convex := line[6]='c';
            nb:=0;
         end
      end else begin
         if Copy(line,1,3)='end' then begin
            if nb>0 then TestPoly(nb,polygon,convex);
            collect:=false;
         end else begin
            with polygon[nb] do readstr(line,X,Y);
            inc(nb);
         end;
      end;
   end;
   close(f);
   SpeedTest;
   EndTest;
end.