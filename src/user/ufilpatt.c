/**
 **     UFILPATT.C
 **
 **     Copyright (C) 1997, Michael Goffioul
 **
 **       goffioul@emic.ucl.ac.be
 **
 **
 ** Note : compiling the library with -DUSR_KEEP_SHAPE makes a circle
 **        looks like a circle on the screen
 **/

#include "libgrx.h"
#include "alloca.h"
#include "usercord.h"

void GrUsrPatternFilledPlot(int x,int y,GrPattern *p)
{
        U2SX(x,CURC);
        U2SY(y,CURC);
        GrPatternFilledPlot(x,y,p);
}

void GrUsrPatternFilledLine(int x1,int y1,int x2,int y2,GrPattern *p)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y1,CURC);
        U2SY(y2,CURC);
        GrPatternFilledLine(x1,y1,x2,y2,p);
}

void GrUsrPatternFilledBox(int x1,int y1,int x2,int y2,GrPattern *p)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y1,CURC);
        U2SY(y2,CURC);
        GrPatternFilledBox(x1,y1,x2,y2,p);
}

void GrUsrPatternFilledEllipse(int xc,int yc,int xa,int ya,GrPattern *p)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        GrPatternFilledEllipse(xc,yc,xa,ya,p);
}

void GrUsrPatternFilledCircle(int xc,int yc,int r,GrPattern *p)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrPatternFilledCircle(xc,yc,r,p);
#else
        GrUsrPatternFilledEllipse(xc,yc,r,r,p);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrPatternFilledEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,GrPattern *p)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        GrPatternFilledEllipseArc(xc,yc,xa,ya,start,end,style,p);
}

void GrUsrPatternFilledCircleArc(int xc,int yc,int r,int start,int end,int style,GrPattern *p)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrPatternFilledCircleArc(xc,yc,r,start,end,style,p);
#else
        GrUsrPatternFilledEllipseArc(xc,yc,r,r,start,end,style,p);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrPatternFilledPolygon(int numpts,int points[][2],GrPattern *p)
{
        int pt;
        setup_alloca()
        int (*tmp)[2] = alloca(sizeof(int) * 2 * numpts);

        if (tmp != NULL) {
          for ( pt = 0;pt < numpts;pt++) {
                tmp[pt][0] = points[pt][0];
                tmp[pt][1] = points[pt][1];
                U2SX(tmp[pt][0],CURC);
                U2SY(tmp[pt][1],CURC);
          }
          GrPatternFilledPolygon(numpts,tmp,p);
        }
        reset_alloca()
}

void GrUsrPatternFilledConvexPolygon(int numpts,int points[][2],GrPattern *p)
{
        int pt;
        setup_alloca()
        int (*tmp)[2] = alloca(sizeof(int) * 2 * numpts);

        if (tmp != NULL) {
          for ( pt = 0;pt < numpts;pt++) {
                tmp[pt][0] = points[pt][0];
                tmp[pt][1] = points[pt][1];
                U2SX(tmp[pt][0],CURC);
                U2SY(tmp[pt][1],CURC);
          }
          GrPatternFilledConvexPolygon(numpts,tmp,p);
        }
        reset_alloca()
}
