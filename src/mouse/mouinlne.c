/**
 ** MOUINLNE.C ---- the mouse inline functions
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"

void (GrMouseEventMode)(int dummy)
{
        GrMouseEventMode(dummy);
}

void (GrMouseInit)(void)
{
        GrMouseInit();
}

void (GrMouseUnInit)(void)
{
        GrMouseUnInit();
}

void (GrMouseGetEvent)(int flags,GrMouseEvent *event)
{
        GrMouseGetEvent(flags,event);
}

int (GrMousePendingEvent)(void)
{
        return(GrMousePendingEvent());
}

GrCursor *(GrMouseGetCursor)(void)
{
        return(GrMouseGetCursor());
}

int (GrMouseCursorIsDisplayed)(void)
{
        return(GrMouseCursorIsDisplayed());
}

void (GrMouseGetLimits)(int *x1,int *y1,int *x2,int *y2)
{
        GrMouseGetLimits(x1,y1,x2,y2);
}

int (GrMouseBlock)(GrContext *c,int x1,int y1,int x2,int y2)
{
        return(GrMouseBlock(c,x1,y1,x2,y2));
}

void (GrMouseUnBlock)(int flags)
{
        GrMouseUnBlock(flags);
}

