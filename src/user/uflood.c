/**
 ** UFLOOD.C ---- fill an arbitrary area in user coordinates
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 ** [e-mail: hsc@techfak.uni-kiel.de]
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrFloodFill(int x, int y, GrColor border, GrColor c)
{
        U2SX(x,CURC);
        U2SY(y,CURC);
        GrFloodFill(x,y,border,c);
}
