/**
 **     UCUST.C
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


void GrUsrCustomLine(int x1,int y1,int x2,int y2,GrLineOption *lo)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y1,CURC);
        U2SY(y2,CURC);
        GrCustomLine(x1,y1,x2,y2,lo);
}

void GrUsrCustomBox(int x1,int y1,int x2,int y2,GrLineOption *lo)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y1,CURC);
        U2SY(y2,CURC);
        GrCustomBox(x1,y1,x2,y2,lo);
}

void GrUsrCustomEllipse(int xc,int yc,int xa,int ya,GrLineOption *lo)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        GrCustomEllipse(xc,yc,xa,ya,lo);
}

void GrUsrCustomCircle(int xc,int yc,int r,GrLineOption *lo)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrCustomCircle(xc,yc,r,lo);
#else
        GrUsrCustomEllipse(xc,yc,r,r,lo);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrCustomEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,GrLineOption *lo)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        GrCustomEllipseArc(xc,yc,xa,ya,start,end,style,lo);
}

void GrUsrCustomCircleArc(int xc,int yc,int r,int start,int end,int style,GrLineOption *lo)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrCustomCircleArc(xc,yc,r,start,end,style,lo);
#else
        GrUsrCustomEllipseArc(xc,yc,r,r,start,end,style,lo);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrCustomPolyLine(int numpts,int points[][2],GrLineOption *lo)
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
          GrCustomPolyLine(numpts,tmp,lo);
        }
        reset_alloca()
}

void GrUsrCustomPolygon(int numpts,int points[][2],GrLineOption *lo)
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
          GrCustomPolygon(numpts,tmp,lo);
        }
        reset_alloca()
}
