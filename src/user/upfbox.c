/**
 **     UPFBOX.C
 **
 **     Copyright (C) 1997, Michael Goffioul
 **
 **       goffioul@emic.ucl.ac.be
 **
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrPatternFilledBox(int x1,int y1,int x2,int y2,GrPattern *p)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y1,CURC);
        U2SY(y2,CURC);
        GrPatternFilledBox(x1,y1,x2,y2,p);
}
