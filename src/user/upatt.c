/**
 **     UPATT.C
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


void GrUsrPatternedLine(int x1,int y1,int x2,int y2,GrLinePattern *lp)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y1,CURC);
        U2SY(y2,CURC);
        GrPatternedLine(x1,y1,x2,y2,lp);
}

void GrUsrPatternedBox(int x1,int y1,int x2,int y2,GrLinePattern *lp)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y1,CURC);
        U2SY(y2,CURC);
        GrPatternedBox(x1,y1,x2,y2,lp);
}

void GrUsrPatternedEllipse(int xc,int yc,int xa,int ya,GrLinePattern *lp)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        GrPatternedEllipse(xc,yc,xa,ya,lp);
}

void GrUsrPatternedCircle(int xc,int yc,int r,GrLinePattern *lp)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrPatternedCircle(xc,yc,r,lp);
#else
        GrUsrPatternedEllipse(xc,yc,r,r,lp);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrPatternedEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,GrLinePattern *lp)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        GrPatternedEllipseArc(xc,yc,xa,ya,start,end,style,lp);
}

void GrUsrPatternedCircleArc(int xc,int yc,int r,int start,int end,int style,GrLinePattern *lp)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrPatternedCircleArc(xc,yc,r,start,end,style,lp);
#else
        GrUsrPatternedEllipseArc(xc,yc,r,r,start,end,style,lp);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrPatternedPolyLine(int numpts,int points[][2],GrLinePattern *lp)
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
          GrPatternedPolyLine(numpts,tmp,lp);
        }
        reset_alloca()
}

void GrUsrPatternedPolygon(int numpts,int points[][2],GrLinePattern *lp)
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
          GrPatternedPolygon(numpts,tmp,lp);
        }
        reset_alloca()
}
