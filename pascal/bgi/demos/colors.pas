program Colors;
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
  BGI2GRX;

var
  i, c,
  grDriver,
  grMode,
  ErrCode: Integer;


procedure ShowColors(Mode: Word);
var
  x, y, c: word;
  f1, f2 : real;
begin
  { Show only 8 bit, hicolor and truecolor modes }
  if GetMaxColor < 255 then return;

  { Show only 640 x 480 modes }
  { if (GetMaxX <> 639) or (GetMaxY <> 479) then return; }

  f1 := GetMaxY / 64;
  f2 := GetMaxY / 256;

  { Red }
  for y := 0 to GetMaxY do begin
    if GetMaxColor = 255 then begin
      SetRGBPalette(Word(y / f1), Word(y / f1), 0, 0);
      SetColor(Word(y / f1));
    end
    else
      SetRGBColor(Word(y / f2), 0, 0);

    Line(0, y, GetMaxX div 4, y);
  end;

  { Green }
  for y := 0 to GetMaxY do begin
    if GetMaxColor = 255 then begin
      SetRGBPalette(64 + Word(y / f1), 0, Word(y / f1), 0);
      SetColor(64 + Word(y / f1));
    end
    else
      SetRGBColor(0, Word(y / f2), 0);

    Line(GetMaxX div 4, y, GetMaxX div 2, y);
  end;

  { Blue }
  for y := 0 to GetMaxY do begin
    if GetMaxColor = 255 then begin
      SetRGBPalette(128 + Word(y / f1), 0, 0, Word(y / f1));
      SetColor(128 + Word(y / f1));
    end
    else
      SetRGBColor(0, 0, Word(y / f2));

    Line(GetMaxX div 2, y, GetMaxX - GetMaxX div 4, y);
  end;

  { Gray }
  for y := 0 to GetMaxY do begin
    if GetMaxColor = 255 then begin
      SetRGBPalette(192 + Word(y / f1), Word(y / f1), Word(y / f1), Word(y / f1));
      SetColor(192 + Word(y / f1));
    end
    else
      SetRGBColor(Word(y / f2), Word(y / f2), Word(y / f2));

    Line(GetMaxX - GetMaxX div 4, y, GetMaxX, y);
  end;

  if GetMaxColor = 255 then
    SetColor(255)
  else
    SetColor(White);
  OutTextXY(10, 2, GetModeName(Mode)) ;
  Rectangle(0,0,GetMaxX,GetMaxY);
  readln;
end;

begin
  grDriver := Detect;
  InitGraph(grDriver, grMode,{'e:\bp\bgi'}'');
  ErrCode := GraphResult;
  if ErrCode = grOk then
  begin  { Do graphics }
    for i := 0 to GetMaxMode do
    begin
      SetGraphMode(i);
      ShowColors(i);
    end;
    CloseGraph;
  end
  else begin
    Writeln('Graphics error:', GraphErrorMsg(ErrCode));
    Write  ('Press Enter ...');
    Readln;
  end
end.
