program Minimum_Size_Test;
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
  grDriver,
  grMode   : Integer;
begin
  grDriver := Detect;
  InitGraph(grDriver, grMode,'');
  CloseGraph;
end.
