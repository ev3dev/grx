/**
 ** GENERIC/LINE.C ---- generic (=slow) line draw routine
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
  
void drawline(int x,int y,int dx,int dy,long c)
{
        int cnt,err,yoff = 1;
        if(dx < 0) {
            x += dx; dx = (-dx);
            y += dy; dy = (-dy);
        }
        if(dy < 0) {
            yoff = (-1);
            dy         = (-dy);
        }
        if(dx > dy) {
            err = (cnt = dx) >> 1;
            do {
                drawpixel(x,y,c);
                if((err -= dy) < 0) err += dx,y += yoff;
                x++;
            } while(--cnt >= 0);
        }
        else {
            err = (cnt = dy) >> 1;
            do {
                drawpixel(x,y,c);
                if((err -= dx) < 0) err += dy,x++;
                y += yoff;
            } while(--cnt >= 0);
        }
}

