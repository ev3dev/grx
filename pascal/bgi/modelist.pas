program ModeList;
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
  BGI2GRX;
(*$else *)
  graph;
(*$endif*)

var
  i,
  grDriver,
  grMode,
  ErrCode: Integer;

begin
  { InitGraph is not needed for GetMaxMode and GetModeName in GPC }
  { grDriver := Detect; }
  grDriver := InstallUserDriver('SVGA256', nil); { Not used in GPC }
  InitGraph(grDriver, grMode,'..\..\chr');
  ErrCode := GraphResult;
  if ErrCode = grOk then
  begin  { Do graphics }
    (*$ifdef __GPC__ *)
      CloseGraph;
    (*$endif*)
    for i := 0 to GetMaxMode do begin
      writeln(i:4, ' ' + GetModeName(i));
    end;
    readln;
    (*$ifndef __GPC__ *)
      CloseGraph;
    (*$endif*)
  end
  else begin
    Writeln('Graphics error:', GraphErrorMsg(ErrCode));
    Write  ('Press Enter ...');
    Readln;
  end
end.