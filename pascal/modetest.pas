Program Vir_Test;

uses
  grx;

var
  i, f, m : Integer;
  gvm : GrVideoMode;
  cuvm, fivm : GrVideoModePtr;

begin
  { Das initialisiert grx20 erstmal }
  m := GrSetMode(GR_default_text, 0, 0, 0, 0, 0);

  GrSetModeRestore(False);

  for f := GR_unknown_mode to GR_frameSVGA32H do begin
    fivm := GrFirstVideoMode(f);
    while fivm <> Nil do begin
      gvm := fivm^;
      i := i + 1;
      write  (i:3);
      write  (f:3);
      write  (gvm.mode:6);
      write  (gvm.width:5, 'x',gvm.height:4, 'x', ord(gvm.bpp):3);
      { write  (' ', gvm.present > chr(0)); }
      writeln;
      fivm := GrNextVideoMode(fivm);
    end;
  end;
  readln;
  { M := GrSetMode(GR_default_text,0,0,0,0,0); }
end.
