/**
 **
 **        UELLARC.C
 **
 **        Copyright (C), Michaël Goffioul
 **
 **
 ** Note : compiling the library with -DUSR_KEEP_SHAPE makes a circle
 **           looks like a circle on the screen
 **/

#include "libgrx.h"
#include "usercord.h"

static void udrawarc(int xc,int yc,int xa,int ya,int start,int end,int style,long c,int filled)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        (*(filled ? GrFilledEllipseArc : GrEllipseArc))(xc,yc,xa,ya,start,end,style,c);
}

void GrUsrCircleArc(int xc,int yc,int r,int start,int end,int style,long c)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrCircleArc(xc,yc,r,start,end,style,c);
#else
        udrawarc(xc,yc,r,r,start,end,style,c,FALSE);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrFilledCircleArc(int xc,int yc,int r,int start,int end,int style,long c)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrFilledCircleArc(xc,yc,r,start,end,style,c);
#else
        udrawarc(xc,yc,r,r,start,end,style,c,TRUE);
#endif /* USR_KEEP_SHAPE */
}

void GrUsrEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,long c)
{
        udrawarc(xc,yc,xa,ya,start,end,style,c,FALSE);
}

void GrUsrFilledEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,long c)
{
        udrawarc(xc,yc,xa,ya,start,end,style,c,TRUE);
}
