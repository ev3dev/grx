Program Taste;

uses
  grx;

var
  MouseMode, DrawMode,
  m,
  x, y, xv, yv, co : Integer;
  evt: GrMouseEvent;
  key : char;
  Finito : boolean;

procedure MouseBehandlung;
   var st:string[80];
begin
  if (evt.flags and GR_M_KEYPRESS) > 0 then begin
    key := chr(evt.key);
    writestr(st,'Key :', key:2, ord(key):4, evt.key mod 256: 4, evt.key div 256: 4, evt.key: 6);
    GrTextXY(evt.x,evt.y,st,GrWhite,GrBlack);
    case evt.key of
      81, 113 : Finito := true; {'Q', 'q'}
    end;
  end;
end;

begin
  
  Writeln('Use ''Q'' to Quit'); 

  x  := 640;
  y  := 480;
  co := 256;
  M  := GrSetMode(GR_default_graphics,x,y,co,0,0);
  
  if GrMouseDetect then begin
    GrMouseEventMode(1);
    GrMouseInit;
    GrMouseDisplayCursor;
    DrawMode  := 0;
    MouseMode := 4;
  end else
    exit;

  repeat
    Finito  := false;

    GrMouseGetEventT(GR_M_EVENT,@evt,0);
    if evt.flags > 0 then MouseBehandlung;

  until Finito;
end.
