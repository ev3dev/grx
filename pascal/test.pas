unit Test;

interface

uses GRX;

var exit_message: String [1000];

procedure InitTest;
procedure EndTest;

implementation

procedure InitTest;
   var m,n,i: Integer;
       w,h,c: Integer = 0;
begin
   if ParamCount < 2 then
      m := GrSetMode(Gr_Default_Graphics,0,0,0,0,0)
   else begin
      for n:=1 to ParamCount do begin
         ReadStr(ParamStr(n),i);
         case ParamStr(n)[Length(ParamStr(n))] of
            'k','K': i := i shl 10;
            'm','M': i := i shl 20;
         end;
         case n of
            1: w:=i;
            2: h:=i;
            3: c:=i;
         end
      end;
      if ParamCount = 2 then
         m :=GrSetMode(Gr_Width_Height_Graphics,w,h,0,0,0)
      else
         m := GrSetMode(Gr_Width_Height_Color_Graphics,w,h,c,0,0)
   end
end;

procedure EndTest;
   var m:Integer;
begin
   m:=GrSetMode(Gr_Default_Text,0,0,0,0,0);
   if exit_message <> '' then begin
      WriteLn(exit_message);
      Readln
   end
end;

end.
