program Demo;
{
 * test and demo program for unit BGI2GRX
 *
 * Please read the copyright notices of BGI2GRX.PAS
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author : Sven Hilscher
 * e-mail : sven@rufus.central.de
}

uses
(*$ifdef __GPC__ *)
  BGI2GRX, SH_Funcs;
(*$else *)
  graph, crt;
(*$endif*)


(*$ifndef __GPC__ *)
type
  WrkString = String[255];
(*$endif*)

var
  Test,
  Corr : Real;

function MyStr(Numeric, Len: Integer):WrkString;
var
  RetString : WrkString;
begin
  (*$ifdef __GPC__ *)
    { MyStr := SH_Str(Numeric, Len); }
    RetString := '';
    WriteStr(RetString, Numeric:Len);
    MyStr := RetString;
  (*$else *)
    str(Numeric:Len, RetString);
    MyStr := RetString;
  (*$endif*)
end;

procedure ColorSetup;
var i: Integer;
begin
  if GetMaxColor < 2 then
  else if GetMaxColor < 16 then
    for i := 1 to 14 do
      SetRGBPalette(i, random(64), random(64), random(64))
  else if GetMaxColor < 256 then begin
    for i := 0 to 63 do begin
      if i > 32 then begin
        SetRGBPalette(i      , i, i - random(16), i - random(16));
        SetRGBPalette(i +  64, i - random(16), i, i - random(16));
        SetRGBPalette(i + 128, i - random(16), i - random(16), i);
        SetRGBPalette(i + 192, i - random(16), i - random(16), i - random(16));
      end
      else begin
        SetRGBPalette(i,    i, i + random(16), i + random(16));
        SetRGBPalette(i +  64, i + random(16), i, i + random(16));
        SetRGBPalette(i + 128, i + random(16), i + random(16), i);
        SetRGBPalette(i + 192, i + random(16), i + random(16), i + random(16));
      end;
      { Set entry 15 to white for text }
      SetRGBPalette(15, 63, 63, 63);
      { Set entry 0 to white for text }
      SetRGBPalette(0, 0, 0, 0);
    end
  end
end;

procedure StartTest(HeadLine: WrkString);
begin
  ClearDevice;
  SetColor(White);
  SetTextStyle(0,0,0);
  SetTextJustify(CenterText, TopText);
  OutTextXY(GetMaxX div 2, 0, Headline + ' Demo - Hit Any Key ...');
  SetTextJustify(LeftText, TopText);
  Line(0, 9, GetMaxX, 9);
end;

procedure SetRandomColor;
var i, c : Integer;
begin
  if GetMaxColor < 256 then begin
    SetColor(random(GetMaxColor + 1));
    SetFillStyle(random(UserFill + 1), random(GetMaxColor + 1));
  end
  else begin
    (*$ifdef __GPC__ *)
      i := random(4);
      c := random(128) + 32;
      case i of
        0: SetRGBColor(c, c - random(32), c + random(32));
        1: SetRGBColor(c - random(32), c + random(32), c);
        2: SetRGBColor(c + random(32), c, c - random(32));
      else
        SetRGBColor(c, c, c)
      end;
      SetFillStyle(random(UserFill + 1), random(255) shl 16 + random(255) shl 8 + random(255));
    (*$endif*)
  end;
end;

procedure BGIInfo(Mode: Integer);
var
  x, y : Integer;
begin
  StartTest('Info Functions');
  outtextxy( 2, 20, 'GetDriverName        : ' + GetDriverName);
  outtextxy( 2, 30, 'GetMaxMode           : ' + MyStr(GetMaxMode    , 10));
  outtextxy( 2, 40, 'GetGraphMode         : ' + MyStr(GetGraphMode  , 10));
  outtextxy( 2, 50, 'GetModeName          : ' + GetModeName(Mode));
  outtextxy( 2, 70, 'GetMaxColor          : ' + MyStr(GetMaxColor   , 10));
  outtextxy( 2, 80, 'GetPaletteSize       : ' + MyStr(GetPaletteSize, 10));
  outtextxy( 2,100, 'GetMaxX              : ' + MyStr(GetMaxX       , 10));
  outtextxy( 2,110, 'GetMaxY              : ' + MyStr(GetMaxY       , 10));
  GetAspectRatio(x, y);
  Corr := ((GetMaxY+1)/(GetMaxX+1)) / 0.75;
  outtextxy( 2,120, 'GetAspectRatio->X    : ' + MyStr(x             , 10));
  outtextxy( 2,130, 'GetAspectRatio->Y    : ' + MyStr(y             , 10));
  outtextxy( 2,150, 'ImageSize(1,8,1,8)   : ' + MyStr(ImageSize(1, 8, 1, 8), 10));
  repeat
  until keypressed;
  while keypressed do x := ord(ReadKey);
end;

procedure LineTest;
var
  i: Word;
begin
  StartTest('Line');
  repeat
    SetRandomColor;
    for i := 1 to 50 do begin
      Line(0, random(GetMaxY)+10, GetMaxX, random(GetMaxY)+10);
      Line(random(GetMaxX), 10, random(GetMaxX), GetMaxY);
    end;
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure RectangleTest;
var
  i: Word;
begin
  StartTest('RecTangle');
  repeat
    SetRandomColor;
    for i := 1 to 50 do
      Rectangle(random(GetMaxX), random(GetMaxY-10)+10, random(GetMaxX), random(GetMaxY-10)+10);
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure BarTest;
var
  i: Word;
begin
  StartTest('Bar');
  repeat
    SetRandomColor;
    for i := 1 to 10 do
      Bar(random(GetMaxX), random(GetMaxY-10)+10, random(GetMaxX), random(GetMaxY-10)+10);
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure CircleTest;
var
  i, r: Integer;
begin
  StartTest('Circle');
  repeat
    SetRandomColor;
    for i := 1 to 10 do begin
      r := random(GetMaxX div 3) + 1;
      Circle(random(GetMaxX - 2 * r) + r, random(GetMaxY - (2 * round(r * Corr) + 10)) + round(r * Corr) + 10, round(r*Corr));
    end;
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure EllipseTest;
var
  i, rx, ry: Word;
begin
  StartTest('Ellipse');
  repeat
    SetRandomColor;
    for i := 1 to 10 do begin
      rx := random(GetMaxX div 3);
      ry := random(GetMaxY div 3);
      Ellipse(random(GetMaxX - 2 * rx) + rx, random(GetMaxY-(2 * ry + 10)) + ry + 10, 0, 359,
              round(rx * Corr), round(ry * Corr));
    end;
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure Ellipse2Test;
var
  i: Word;
begin
  StartTest('Ellipse 2.');
  for i := 20 to GetMaxX div 2 do begin
    SetRandomColor;
    Test := i;
    Ellipse(GetMaxX div 2, GetMaxY div 2 + 5, 0, Round(i * (720 / GetMaxX)),
            i, round(i * GetMaxY / GetMaxX) - 6);
  end;
  repeat
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure FillEllipseTest;
var
  i, rx, ry: Word;
begin
  StartTest('FillEllipse');
  repeat
    SetRandomColor;
    for i := 1 to 10 do begin
      rx := random(GetMaxX div 3);
      ry := random(GetMaxY div 3);
      FillEllipse(random(GetMaxX - 2 * rx) + rx, random(GetMaxY-(2 * round(ry * Corr) + 10)) + round(ry * Corr) + 10,
                  round(rx * Corr), round(ry * Corr));
    end;
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure ArcTest;
var
  i, r: Word;
begin
  StartTest('Arc');
  repeat
    SetRandomColor;
    for i := 1 to 10 do begin
      r := random(GetMaxX div 3);
      Arc(random(GetMaxX - 2 * r) + r, random(GetMaxY-(2 * round(r * Corr) + 10)) + round(r * Corr) + 10, 0, random(360),
          round(r * Corr));
    end;
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure SectorTest;
var
  i, rx, ry: Word;
begin
  StartTest('Sector');
  repeat
    SetRandomColor;
    for i := 1 to 10 do begin
      rx := random(GetMaxX div 3);
      ry := random(GetMaxY div 3);
      Sector(random(GetMaxX - 2 * rx) + rx, random(GetMaxY-(2 * round(ry * Corr) + 10)) + round(ry * Corr) + 10,
             random(360), random(360), round(rx * Corr), round(ry * Corr));
    end;
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure PieSliceTest;
var
  i, r: Word;
begin
  StartTest('PieSlice');
  repeat
    SetRandomColor;
    for i := 1 to 10 do begin
      r := random(GetMaxX div 3);
      PieSlice(random(GetMaxX - 2 * r) + r, random(GetMaxY-(2 * round(r * Corr) + 10)) + round(r * Corr) + 10, 0, random(360),
               round(r * Corr));
    end;
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure Bar3DTest;
var
  i, h, n: Word;
begin
  StartTest('Bar3D');
  n := GetMaxX div 50;
  for i := 0 to (n - 1) do begin
    SetRandomColor;
    h := random(GetMaxY-30);
    Bar3D((GetMaxX div n) * i + 2, GetMaxY -15, (GetMaxX div n) * i + (GetMaxX div n - 12), GetMaxY - (15 + h), 10, TopOn);
    OutTextXY((GetMaxX div n) * i + 2, GetMaxY -12, MyStr(h,4));
  end;
  repeat
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure PolyTest;
var
  i, j : Word;
  a : array[1..4] of PointType;
begin
  StartTest('Poly');
  repeat
    SetRandomColor;
    for i := 1 to 10 do begin
      for j := 1 to 4 do begin
        a[j].x := random(GetMaxX);
        a[j].y := random(GetMaxY - 10) + 10;
      end;
      DrawPoly(4, a);
    end;
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure FillPolyTest;
var
  i, j : Word;
  a : array[1..4] of PointType;
begin
  StartTest('FillPoly');
  repeat
    SetRandomColor;
    for i := 1 to 10 do begin
      for j := 1 to 4 do begin
        a[j].x := random(GetMaxX);
        a[j].y := random(GetMaxY - 10) + 10;
      end;
      FillPoly(4, a);
    end;
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;

procedure FloodFillTest;
var
  i, x, y: Word;
  r, p : real;
begin
  StartTest('LineTo/FloodFill');
  repeat
    SetRandomColor;
    p   :=  0; { Angle }
    r   := 10; { Radius }
    x   :=  GetMaxX div 2;
    y   := 10 + GetMaxY div 2 + 10;
    MoveTo(x, y);
    repeat
      p := p + (Pi / 20);
      r := r + 0.5;
      x := Round(sin(p) *  r        ) + GetMaxX div 2;
      y := Round(cos(p) * (r * Corr)) + GetMaxY div 2 + 10;
      LineTo(x, y);
    until ((r + 1) * Corr) > GetMaxY div 2 - 5;
    RecTangle(0, 10, GetMaxX, GetMaxY);
    FloodFill(GetMaxX div 2,GetMaxY div 2 + 10, GetColor);
  until keypressed;
  while keypressed do i := ord(ReadKey);
end;


procedure ImageTest;
const
  d = 8;
var
  p1, p2 : Pointer;
  Size,
  x, y, c,
  i, j : Word;

begin
  StartTest('GetImage/PutImage');

  { Draw a little ball }
  SetFillStyle(SolidFill, White);
  FillEllipse(30, 25, 15, 15);
  for j := 0 to 20 do
    for i := 0 to 20 do PutPixel(2 * j + 10, 2 * i + 10, 0);

  Size := ImageSize(10, 10, 50, 40) * 4; { Unknown crashes in GPC }
  GetMem(p1, Size);
  GetMem(p2, Size);
  GetImage(10, 10, 50, 40, p1^);

  { Clear the litte ball }
  GetImage(110, 110, 150, 140, p2^);
  PutImage(10, 10, p2^, NormalPut);

  for i := 1 to 50 do begin
    SetRandomColor;
    c := GetColor;
    for j := 1 to 50 do begin
      PutPixel(random(GetMaxX), random(GetMaxY - 10) + 10, c)
    end;
  end;
  x := GetMaxX div 2;
  y := GetMaxY div 2;
  repeat
    if x < d * 3 then
      x := x + random(d)
    else
      if x > GetMaxX - d * 3 then
        x := x - random(d)
      else
        x := x + d - random(d * 2 + 1);

    if y < d * 3 then
      y := y + random(d)
    else
      if y > GetMaxY - d * 3 then
        y := y - random(d)
      else
        y := y + d - random(d * 2 + 1);

    GetImage(x, y, x+40, y+30, p2^);
    PutImage(x, y, p1^, OrPut);
    for i := 1 to 50 do begin
      SetRandomColor;
      c := GetColor;
      for j := 1 to 50 do begin
        PutPixel(random(GetMaxX), random(GetMaxY - 10) + 10, c)
      end;
    end;
    PutImage(x, y, p2^, NormalPut);
  until keypressed;
  while keypressed do i := ord(ReadKey);
  FreeMem(p1, Size);
  FreeMem(p2, Size);
end;

procedure TextTest;
var
  FontName: array[0..10] of WrkString;
  i, pos : Word;
begin
  StartTest('Text');
  repeat
    SetRandomColor;
    OutTextXY(random(GetMaxX - 100), random(GetMaxY - 10) + 10, 'OutTextXY DefaultFont');
  until keypressed;
  while keypressed do i := ord(ReadKey);
  StartTest('Vector Fonts');
  repeat
    SetRandomColor;
    SetTextStyle(random(10)+1, random(2), random(20) + 6);
    OutTextXY(random(GetMaxX - 100), random(GetMaxY - 10), 'Vector Fonts');
  until keypressed;
  while keypressed do i := ord(ReadKey);
  StartTest('Font Names');
  FontName[ 0] := 'DefaultFont';
  FontName[ 1] := 'TriplexFont';
  FontName[ 2] := 'SmallFont';
  FontName[ 3] := 'SansSerifFont';
  FontName[ 4] := 'GothicFont';
  FontName[ 5] := 'ScriptFont';
  FontName[ 6] := 'SimplexFont';
  FontName[ 7] := 'TriplexScrFont';
  FontName[ 8] := 'ComplexFont';
  FontName[ 9] := 'EuropeanFont';
  FontName[10] := 'BoldFont';
  pos := 0;
  for i := 0 to 10 do begin
    SetTextStyle(i, HorizDir, 0);
    OutTextXY(10, pos +  12, chr(i div 10 + 48) + chr(i mod 10 + 48) + ' ' + FontName[i]);
    Inc(pos, TextHeight('H') + 2);
  end;
  repeat
  until keypressed;
  while keypressed do i := ord(ReadKey);
  StartTest('Vertical Fonts');
  pos := 0;
  for i := 0 to 10 do begin
    SetTextStyle(i, VertDir, 0);
    OutTextXY(pos + 10, 12, chr(i div 10 + 48) + chr(i mod 10 + 48) + ' ' + FontName[i]);
    Inc(pos, TextHeight('H') + 2);
  end;
  repeat
  until keypressed;
  while keypressed do i := ord(ReadKey);
  StartTest('SetTextJustify');
  for i := 1 to 10 do begin
    SetTextJustify(CenterText, TopText);
    if GetMaxX < 320 then
      SetTextStyle(SmallFont, HorizDir, 5)
    else
      SetTextStyle(SmallFont, HorizDir, 7);
    OutTextXY(GetMaxX div 2, GetMaxY div 2 - 60, 'That''s all friends');
    OutTextXY(GetMaxX div 2, GetMaxY div 2 - 20, 'Have a good time while using');
    OutTextXY(GetMaxX div 2, GetMaxY div 2 + 20, 'GPC, GRX20, BCC2GRX and BGI2GRX !');
    SetTextStyle(DefaultFont, HorizDir, 0);
    OutTextXY(GetMaxX div 2, GetMaxY div 2 + 80, 'Contact me: sven@rufus.central.de');
  end;
  repeat
  until keypressed;
  while keypressed do i := ord(ReadKey);

end;

var
  i, c, m,
  grDriver,
  grMode,
  ErrCode: Integer;
  ModeName: array[0..200] of WrkString;

begin
  { Try different drivers in Boland Pascal }
  { No difference in GPC  }

  grDriver := Detect;
  { grDriver := CGA;      }
  { grDriver := MCGA;     }
  { grDriver := EGA;      }
  { grDriver := EGA64;    }
  { grDriver := EGAMono;  }
  { grDriver := VGA;      }
  { grDriver := InstallUserDriver('svga256', nil) }

  InitGraph(grDriver, grMode,'..\..\chr');
  ErrCode := GraphResult;
  if ErrCode = grOk then
  begin  { Do graphics }
    m := GetMaxMode;
    for grMode := 0 to m do
      ModeName[grMode] := GetModeName(grMode);
    grMode := -1;
    CloseGraph;
    while (grMode < 0) or (grMode > m) do begin
      writeln;
      for grMode := 0 to m do begin
        if grMode mod 2 = 0 then writeln;
        write('   #',grMode:1, ' = "', ModeName[grMode],'"');
      end;
      writeln;
      write('Modenumber (0..',m:1,') : ');
      readln(grMode);
    end;
    m := grMode;
    InitGraph(grDriver, grMode,'..\..\chr');
    SetGraphMode(m);
    ColorSetup;
    BGIInfo(m);
    LineTest;
    RectangleTest;
    BarTest;
    CircleTest;
    EllipseTest;
    Ellipse2Test;
    FillEllipseTest;
    ArcTest;
    Bar3DTest;
    SectorTest;
    PieSliceTest;
    PolyTest;
    FillPolyTest;
    FloodFillTest;
    ImageTest;
    TextTest;
    CloseGraph;
  end
  else begin
    Writeln('Graphics error:', GraphErrorMsg(ErrCode));
    Write  ('Press Enter ...');
    Readln;
  end;
end.