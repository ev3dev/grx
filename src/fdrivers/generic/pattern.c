/**
 ** GENERIC/PATTERN.C ---- generic (=slow) patterned scan line fill routine
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
  
void drawpattern(int x,int y,int w,char patt,long fg,long bg)
{
        uchar mask = 0x80;
        w += x;
        do {
            drawpixel(x,y,(patt & mask) ? fg : bg);
            if((mask >>= 1) == 0) mask = 0x80;
        } while(++x != w);
}


