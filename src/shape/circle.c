/**
 ** CIRCLE.C ---- draw circles, ellipses and their arcs
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrEllipse(int xc,int yc,int xa,int ya,long c)
{
        GrFillArg fval;
        fval.color = c;
        _GrScanEllipse(xc,yc,xa,ya,&_GrSolidFiller,fval,FALSE);
}

void GrEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,long c)
{
        GrFillArg fval;
        int pnts[GR_MAX_ELLIPSE_POINTS + 1][2];
        int npts  = GrGenerateEllipseArc(xc,yc,xa,ya,start,end,pnts);
        int close = FALSE;
        switch(style) {
          case GR_ARC_STYLE_CLOSE2:
            pnts[npts][0] = xc;
            pnts[npts][1] = yc;
            npts++;
          case GR_ARC_STYLE_CLOSE1:
            close = TRUE;
            break;
        }
        fval.color = c;
        _GrDrawPolygon(npts,pnts,&_GrSolidFiller,fval,close);
}

void GrCircle(int xc,int yc,int r,long c)
{
        GrEllipse(xc,yc,r,r,c);
}

void GrCircleArc(int xc,int yc,int r,int start,int end,int style,long c)
{
        GrEllipseArc(xc,yc,r,r,start,end,style,c);
}

