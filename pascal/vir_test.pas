Program Vir_Test;

uses
  grx;

procedure TestFunc;
var
  x, y, ww, wh, ii, jj : Integer;

begin
   x := GrSizeX;
   y := GrSizeY;
   ww := round((x-10)/32);
   wh := round((y-10)/8);

   GrSetRGBcolorMode;
   for ii := 0 to 7 do
     for jj := 0 to 31 do
      GrFilledBox(5+jj*ww,5+ii*wh,5+jj*ww+ww-1,5+ii*wh+wh-1,ii*32+jj);
end; { TestFunc }

var
   i, j, x1, y1,
   MouseMode, DrawMode,
   xc, yc,
   x, y, xv, yv, c : Integer;
   bgc, fgc,
   m : Integer;
   evt: GrMouseEvent;
   key : Char;
   Finito : Boolean;
   Conti: GrContext;
   ContiPtr : GrContextPtr;

begin
   x  :=  640;
   y  :=  480;
   xv :=  1024;
   yv :=  1024;
   c  :=  256;
   xc :=  40;
   yc :=  30;

   M := GrSetMode(GR_custom_graphics,x,y,c,xv,yv);
   GrSetModeRestore(False);

   xv := GrVirtualX;
   yv := GrVirtualY;

   TestFunc;
   GrTextXY(10, 10, ' Use the Keys <1> <2> <3> <4> <0> and press the mousebutton ', GrBlack, GrWhite);
   GrTextXY(10, 30, ' In "mode 3" use "<" and ">" to resize the area  "Q" = Quit ', GrBlack, GrWhite);

   if GrMouseDetect then begin
     GrMouseEventMode(1);
     GrMouseInit;
     GrMouseSetColors(GrAllocColor(200,50,150),GrBlack);
     { GrMouseSetLimits(10, 10, xv-10, yv-10); }
     GrMouseDisplayCursor;
     Finito := false;
     repeat
       GrMouseGetEvent(GR_M_EVENT,@evt);
       if (evt.flags and GR_M_KEYPRESS) > 0 then begin
	 key := chr(evt.key);
	 case key of
	   'Q', 'q': Finito := true;
	   'W', 'w': GrMouseWarp(2, 2);
	   'U', 'u': GrMouseUpdateCursor;
	   '>': if (xc < x) and (MouseMode = 3) and ((evt.flags and GR_M_LEFT_DOWN) = 0) then begin
		  xc := xc + 8;
		  yc := yc + 6;
		  GrMouseEraseCursor;
		  GrMouseSetCursorMode(GR_M_CUR_BOX, -xc div 2, -yc div 2, xc div 2, yc div 2,GrWhite);
		  GrMouseDisplayCursor;
		end;
	   '<': if (xc > 8) and (MouseMode = 3) and ((evt.flags and GR_M_LEFT_DOWN) = 0) then begin
		  xc := xc - 8;
		  yc := yc - 6;
		  GrMouseEraseCursor;
		  GrMouseSetCursorMode(GR_M_CUR_BOX, -xc div 2, -yc div 2, xc div 2, yc div 2,GrWhite);
		  GrMouseDisplayCursor;
		end;
	   '1': begin
		  GrMouseEraseCursor;
		  GrMouseSetCursorMode(GR_M_CUR_NORMAL,0,0,0,0,0);
		  GrMouseDisplayCursor;
		  MouseMode := 1;
		end;
	   '2': begin
		  GrMouseEraseCursor;
		  GrMouseSetCursorMode(GR_M_CUR_NORMAL,0,0,0,0,0);
		  GrMouseDisplayCursor;
		  MouseMode := 2;
		end;
	   '3': begin
		  GrMouseEraseCursor;
		  GrMouseSetCursorMode(GR_M_CUR_BOX, -xc div 2, -yc div 2, xc div 2, yc div 2,GrWhite);
		  GrMouseDisplayCursor;
		  MouseMode := 3;
		end;
	   '0': begin
		  GrMouseEraseCursor;
		  GrMouseSetCursorMode(GR_M_CUR_NORMAL,0,0,0,0,0);
		  GrMouseDisplayCursor;
		  MouseMode := 0;
		end;
	   '4': begin
		  GrMouseEraseCursor;
		  GrMouseSetCursorMode(GR_M_CUR_NORMAL,0,0,0,0,0);
		  GrMouseDisplayCursor;
		  MouseMode := 4;
		end;
	   '5': begin
		  GrMouseEraseCursor;
		  GrMouseSetCursorMode(GR_M_CUR_CROSS,0,0,GrWhite,0,0);
		  GrMouseDisplayCursor;
		  MouseMode := 4;
		end;
	 end;
       end;
       if (evt.flags and GR_M_LEFT_DOWN) > 0 then begin
	 case MouseMode of
	   1: begin
		x1 := evt.x;
		y1 := evt.y;
		GrMouseEraseCursor;
		GrMouseSetCursorMode(GR_M_CUR_RUBBER,evt.x,evt.y,GrWhite,0,0);
		GrMouseDisplayCursor;
	      end;
	   2: begin
		x1 := evt.x;
		y1 := evt.y;
		GrMouseEraseCursor;
		GrMouseSetCursorMode(GR_M_CUR_LINE,evt.x,evt.y,GrWhite,0,0);
		GrMouseDisplayCursor;
	      end;
	   3: begin
		x1 := evt.x;
		y1 := evt.y;  { Why I have to save the position ??? }
		ContiPtr := GrCreateContext(xc + 1, yc + 1, Nil, Conti);
		GrMouseEraseCursor;
		GrBitBlt(@Conti, 0, 0, GrScreenContext, x1 - (xc div 2), y1 - (yc div 2), x1 + (xc div 2), y1 + (yc div 2), GrWRITE);
		GrMouseDisplayCursor;
	      end;
	   4: begin
		x1 := evt.x;
		y1 := evt.y;
		DrawMode := 4;
	      end;
	 end;
       end;
       if (evt.flags and GR_M_LEFT_UP) > 0 then begin
	 case MouseMode of
	   1: begin
		GrBox(x1, y1, evt.x, evt.y, GrAllocColor(50,100,150));
		GrMouseEraseCursor;
		GrMouseSetCursorMode(GR_M_CUR_NORMAL,0,0,0,0,0);
		GrMouseDisplayCursor;
	      end;
	   2: begin
		GrLine(x1, y1, evt.x, evt.y, GrAllocColor(150,100,50));
		GrMouseEraseCursor;
		GrMouseSetCursorMode(GR_M_CUR_NORMAL,0,0,0,0,0);
		GrMouseDisplayCursor;
	      end;
	   3: begin
		GrMouseEraseCursor;
		GrBitBlt(GrScreenContext, evt.x - (xc div 2), evt.y - (yc div 2), @Conti, 0, 0, xc, yc, GrWRITE);
		GrDestroyContext(@Conti);
		GrMouseDisplayCursor;
	      end;
	   4: DrawMode := 0;
	 end;
       end;
       if DrawMode = 4 then begin
	 GrLine(x1, y1, evt.x, evt.y, GrWhite);
	 x1 := evt.x;
	 y1 := evt.y;
       end;
       if (evt.flags and GR_M_RIGHT_DOWN) > 0 then begin
       end;
       { Move the visible Part of the virtual screen }
       M := GrSetViewPort(evt.x - x div 2, evt.y - y div 2);
     until Finito;
     GrMouseUnInit;
   end;
   M := GrSetMode(GR_default_text,0,0,0,0,0);
   writeln(xv);
   writeln(yv);
end.

