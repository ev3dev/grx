program AllModes;
{
 * test and demo program for the Graph unit
 *
 * Please read the copyright notices of graph.pas
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author : Sven Hilscher
 * e-mail : sven@rufus.central.de
}

uses
  Graph;

function MyStr(Numeric, Len: Integer):WrkString;
var
  RetString : WrkString;
begin
  str(Numeric:Len, RetString);
  MyStr := RetString;
end;


var
  i, c,
  grDriver,
  grMode,
  ErrCode: Integer;

begin
  grDriver := Detect;
  { grDriver := Detect; }
  { grDriver := InstallUserDriver('SVGA256', nil); Not used in GPC }
  InitGraph(grDriver, grMode,'..\..\chr');
  ErrCode := GraphResult;
  if ErrCode = GrOk then
  begin  { Do graphics }
    for i := 0 to GetMaxMode do
    begin
      SetGraphMode(i);

      OutTextXY(10,40, 'Resolution : ' + MyStr(GetMaxX+1,4) + ' x' + MyStr(GetMaxY+1,4));
      OutTextXY(10,50, 'Colors     : ' + MyStr(GetMaxColor + 1, 10));
      OutTextXY(10,70, 'ActMode    : ' + MyStr(i         , 4));
      OutTextXY(10,80, 'ModeName   : ' + GetModeName(i)    );
      OutTextXY(10,140,'Press Enter');

      Rectangle(0,0,GetMaxX,GetMaxY);
      Rectangle(2,2,GetMaxX-2,GetMaxY-2);

      if GetMaxColor < 256 then
	for c := 0 to GetMaxColor do
	begin
	  SetColor(c);
	  Line(c + 10, 5, c + 10, 30)
	end
      else
	for c := 0 to 255 do
	begin
	  SetColor(c);
	  Line(c + 10, 5, c + 10, 30)
	end;

      ReadLn;
      SetColor(White);
      for c:= 1 to GetMaxX div 3 do
	Line(c * 3, 0, c * 3, GetMaxY);
      for c:= 1 to GetMaxY div 3 do
	Line(0, c * 3, GetMaxX, c * 3);
      ReadLn
    end;
    CloseGraph
  end
  else begin
    Writeln('Graphics error:', GraphErrorMsg(ErrCode));
    Write  ('Press Enter ...');
    ReadLn
  end
end.

