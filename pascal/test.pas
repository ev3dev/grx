unit test;

INTERFACE

uses grx;

var exit_message: string [1000];

procedure InitTest;
procedure EndTest;

IMPLEMENTATION

procedure InitTest;
   var M,n,i: integer;
       w,h,c: integer = 0;
begin
   if ParamCount < 2 then 
      M := GrSetMode(GR_default_graphics,0,0,0,0,0)
   else begin
      for n:=1 to Paramcount do begin
         readstr(Paramstr(n),i);
         case Paramstr(n)[length(ParamStr(n))] of
            'k','K': i := i shl 10;
            'm','M': i := i shl 20;
         end;
         case n of
            1: w:=i;
            2: h:=i;
            3: c:=i;
         end;
      end;
      if ParamCount = 2 then
         M:=GrSetMode(GR_width_height_graphics,w,h,0,0,0)
      else 
         M := GrSetMode(GR_width_height_color_graphics,w,h,c,0,0);
   end;
end;

procedure EndTest;
   var M:integer; k:GrKeyType;
begin
   M:=GrSetMode(GR_default_text,0,0,0,0,0);
   if exit_message <> '' then begin
      writeln(exit_message);
      k:=GrKeyRead;
   end;
end;

end.
