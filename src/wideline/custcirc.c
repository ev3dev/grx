/**
 ** CUSTCIRC.C ---- draw dashed and/or wide circles and ellipses and their arcs
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrCustomEllipse(int xc,int yc,int xa,int ya,GrLineOption *o)
{
        GrFillArg fval;
        int pnts[GR_MAX_ELLIPSE_POINTS][2];
        int npts  = GrGenerateEllipse(xc,yc,xa,ya,pnts);
        fval.color = o->lno_color;
        _GrDrawCustomPolygon(npts,pnts,o,&_GrSolidFiller,fval,TRUE,TRUE);
}

void GrCustomEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,GrLineOption *o)
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
        fval.color = o->lno_color;
        _GrDrawCustomPolygon(npts,pnts,o,&_GrSolidFiller,fval,close,TRUE);
}

void GrCustomCircle(int xc,int yc,int r,GrLineOption *o)
{
        GrCustomEllipse(xc,yc,r,r,o);
}

void GrCustomCircleArc(int xc,int yc,int r,int start,int end,int style,GrLineOption *o)
{
        GrCustomEllipseArc(xc,yc,r,r,start,end,style,o);
}

