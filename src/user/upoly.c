/**
 **
 **     UPOLY.C
 **
 **     Copyright (C), Michaël Goffioul
 **     goffioul@emic.ucl.ac.be
 **
 **/

#include "libgrx.h"
#include "alloca.h"
#include "usercord.h"

static void udrawpoly(int numpts,int points[][2],long c,int closed)
{
        int pt;
        setup_alloca()
        int (*tmp)[2] = alloca(sizeof(int) * 2 * numpts);

        if (tmp != NULL) {
          for ( pt = 0; pt < numpts; pt++) {
                tmp[pt][0] = points[pt][0];
                tmp[pt][1] = points[pt][1];
                U2SX(tmp[pt][0],CURC);
                U2SY(tmp[pt][1],CURC);
          }
          (*(closed ? GrPolygon : GrPolyLine))(numpts,tmp,c);
        }
        reset_alloca()
}

void GrUsrPolyLine(int numpts,int points[][2],long c)
{
        udrawpoly(numpts,points,c,FALSE);
}

void GrUsrPolygon(int numpts,int points[][2],long c)
{
        udrawpoly(numpts,points,c,TRUE);
}

void GrUsrFilledConvexPolygon(int numpts,int points[][2],long c)
{
        int pt;
        setup_alloca()
        int (*tmp)[2] = alloca(sizeof(int) * 2 * numpts);

        if (tmp != NULL) {
          for ( pt = 0; pt < numpts; pt++) {
                tmp[pt][0] = points[pt][0];
                tmp[pt][1] = points[pt][1];
                U2SX(tmp[pt][0],CURC);
                U2SY(tmp[pt][1],CURC);
          }
          GrFilledConvexPolygon(numpts,tmp,c);
        }
        reset_alloca()
}

void GrUsrFilledPolygon(int numpts,int points[][2],long c)
{
        int pt;
        setup_alloca()
        int (*tmp)[2] = alloca(sizeof(int) * 2 * numpts);

        if (tmp != NULL) {
          for ( pt = 0; pt < numpts; pt++) {
                tmp[pt][0] = points[pt][0];
                tmp[pt][1] = points[pt][1];
                U2SX(tmp[pt][0],CURC);
                U2SY(tmp[pt][1],CURC);
          }
          GrFilledPolygon(numpts,tmp,c);
        }
        reset_alloca()
}
