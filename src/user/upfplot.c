/**
 **     UPFPLOT.C
 **
 **     Copyright (C) 1997, Michael Goffioul
 **
 **       goffioul@emic.ucl.ac.be
 **
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrPatternFilledPlot(int x,int y,GrPattern *p)
{
        U2SX(x,CURC);
        U2SY(y,CURC);
        GrPatternFilledPlot(x,y,p);
}
