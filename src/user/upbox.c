/**
 **     UPBOX.C
 **
 **     Copyright (C) 1997, Michael Goffioul
 **
 **       goffioul@emic.ucl.ac.be
 **
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrPatternedBox(int x1,int y1,int x2,int y2,GrLinePattern *lp)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y1,CURC);
        U2SY(y2,CURC);
        GrPatternedBox(x1,y1,x2,y2,lp);
}
