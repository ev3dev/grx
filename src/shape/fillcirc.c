/**
 ** FILLCIRC.C ---- filled circle, ellipse and arcs
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrFilledEllipse(int xc,int yc,int xa,int ya,long c)
{
        GrFillArg fval;
        fval.color = c;
        _GrScanEllipse(xc,yc,xa,ya,&_GrSolidFiller,fval,TRUE);
}

void GrFilledEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,long c)
{
        GrFillArg fval;
        int pnts[GR_MAX_ELLIPSE_POINTS + 1][2];
        int npts  = GrGenerateEllipseArc(xc,yc,xa,ya,start,end,pnts);
        if(style == GR_ARC_STYLE_CLOSE2) {
            pnts[npts][0] = xc;
            pnts[npts][1] = yc;
            npts++;
        }
        fval.color = c;
        _GrScanPolygon(npts,pnts,&_GrSolidFiller,fval);
}

void GrFilledCircle(int xc,int yc,int r,long c)
{
        GrFilledEllipse(xc,yc,r,r,c);
}

void GrFilledCircleArc(int xc,int yc,int r,int start,int end,int style,long c)
{
        GrFilledEllipseArc(xc,yc,r,r,start,end,style,c);
}

