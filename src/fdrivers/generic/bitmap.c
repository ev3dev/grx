/**
 ** GENERIC/BITMAP.C ---- generic (=slow) font or bitmap draw routine
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
  
void drawbitmap(int x,int y,int w,int h,char far *bmp,int pitch,int start,long fg,long bg)
{
        w += x; h += y;
        bmp += (uint)start >> 3;
        start &= 7;
        do {
            uchar far *bitp = (uchar far *)bmp;
            uchar bits = *bitp;
            uchar mask = 0x80 >> start;
            int          xx = x;
            do {
                drawpixel(xx,y,(bits & mask) ? fg : bg);
                if((mask >>= 1) == 0) bits = *++bitp,mask = 0x80;
            } while(++xx != w);
            bmp += pitch;
        } while(++y != h);
}

