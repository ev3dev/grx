/**
 ** ARCTEST.C ---- test arc outline and filled arc drawing
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include <string.h>
#include "test.h"

TESTFUNC(arctest)
{
        char buff[300];
        int  xc,yc,xa,ya,start,end;
        FILE *fp;

        fp = fopen("arctest.dat","r");
        if(fp == NULL) return;
        while(fgets(buff,299,fp) != NULL) {
            int len = strlen(buff);
            while(--len >= 0) {
                    if(buff[len] == '\n') { buff[len] = '\0'; continue; }
                    if(buff[len] == '\r') { buff[len] = '\0'; continue; }
                    break;
            }
            if(sscanf(buff,
                      "arc xc=%d yc=%d xa=%d ya=%d start=%d end=%d",
                      &xc,&yc,&xa,&ya,&start,&end) == 6) {
                GrClearScreen(GrBlack());
                GrFilledEllipse(xc,yc,xa,ya,GrAllocColor(0,0,255));
                GrEllipseArc(xc,yc,xa,ya,start,end,GR_ARC_STYLE_CLOSE2,GrWhite());
                GrTextXY(0,0,buff,GrWhite(),GrNOCOLOR);
                getch();
                GrClearScreen(GrBlack());
                GrFilledEllipseArc(xc,yc,xa,ya,start,end,GR_ARC_STYLE_CLOSE2,GrAllocColor(255,0,0));
                GrTextXY(0,0,buff,GrWhite(),GrNOCOLOR);
                getch();
            }
        }
        fclose(fp);
}

