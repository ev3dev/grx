/**
 ** UPFFLOOD.C ---- pattern fill an arbitrary area in user coordinates
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 ** [e-mail: hsc@techfak.uni-kiel.de]
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrPatternFloodFill(int x, int y, GrColor border, GrPattern *p)
{
        U2SX(x,CURC);
        U2SY(y,CURC);
        GrPatternFloodFill(x,y,border,p);
}
