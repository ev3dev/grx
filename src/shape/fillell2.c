/**
 ** FILLELL2.C ---- filled ellipse arc
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "allocate.h"
#include "shapes.h"

void GrFilledEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,GrColor c)
{
    int (*pnts)[2];
    setup_ALLOC();
    pnts = ALLOC(sizeof(int) * 2 * (GR_MAX_ELLIPSE_POINTS + 1));
    if (pnts != NULL)
    {
        GrFillArg fval;
        int npts  = GrGenerateEllipseArc(xc,yc,xa,ya,start,end,pnts);
        if(style == GR_ARC_STYLE_CLOSE2) {
            pnts[npts][0] = xc;
            pnts[npts][1] = yc;
            npts++;
        }
        fval.color = c;
        _GrScanPolygon(npts,pnts,&_GrSolidFiller,fval);
        FREE(pnts);
    }
    reset_ALLOC();
}
