program Test_GetDefaultPalette;
{
 * test program for unit BGI2GRX
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
  pal : PaletteType;
begin
  { InitGraph is not needed for GetMaxMode and GetModeName in GPC }
  { grDriver := Detect; }
  grDriver := InstallUserDriver('SVGA256', nil); { Not used in GPC }
  InitGraph(grDriver, grMode,'..\..\chr');
  ErrCode := GraphResult;
  if ErrCode = grOk then
  begin  { Do graphics }
    GetDefaultPalette(pal);
    CloseGraph;
    i := pal.Size;
    Writeln('Palette size: ', i);
    for i := 0 to pal.Size-1 do
      Writeln('Color ', i:3, ' : ', pal.Colors[i]);
    Readln;  
  end
  else begin
    Writeln('Graphics error:', GraphErrorMsg(ErrCode));
    Write  ('Press Enter ...');
    Readln;
  end
end.
