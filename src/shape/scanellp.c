/**
 ** SCANELLP.C ---- draw the outline or scan fill an ellipse
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "alloca.h"
#include "arith.h"
#include "clipping.h"
#include "shapes.h"

#define  MAXR        120                /* max radius for which Bresenheim works */

void _GrScanEllipse(int xc,int yc,int xa,int ya,GrFiller *f,GrFillArg c,int filled)
{
        int x1,x2,y1,y2;
        if(xa < 0) xa = (-xa);
        if(ya < 0) ya = (-ya);
        x1 = xc - xa; y1 = yc - ya;
        x2 = xc + xa; y2 = yc + ya;
        clip_ordbox(CURC,x1,y1,x2,y2);
        mouse_block(CURC,x1,y1,x2,y2);
        setup_alloca();
        if((xa == 0) || (ya == 0)) (*f->line)(
            (x1 + CURC->gc_xoffset),
            (y1 + CURC->gc_yoffset),
            (x2 - x1),
            (y2 - y1),
            c
        );
        else if((xa > MAXR) || (ya > MAXR)) {        /* Bresenheim would overflow !! */
            int (*points)[2] = alloca(sizeof(int) * 2 * GR_MAX_ELLIPSE_POINTS);
            if(points != NULL) {
                int count = GrGenerateEllipse(xc,yc,xa,ya,points);
                if(filled) _GrScanConvexPoly(count,points,f,c);
                else           _GrDrawPolygon(count,points,f,c,TRUE);
            }
        }
        else {
            int *scans = alloca(sizeof(int) * (ya + 1));
            int  row   = ya;
            int  col   = 0;
            if(scans != NULL) {
                long yasq  = umul32(ya,ya);
                long xasq  = umul32(xa,xa);
                long xasq2 = xasq + xasq;
                long yasq2 = yasq + yasq;
                long xasq4 = xasq2 + xasq2;
                long yasq4 = yasq2 + yasq2;
                long error = (xasq2 * (row - 1) * row) +
                             (yasq2 * (1 - xasq))      +
                             xasq;
                while((xasq * row) > (yasq * col)) {
                    if(error >= 0) {
                        scans[row] = col;
                        row--;
                        error -= xasq4 * row;
                    }
                    error += yasq2 * (3 + (col << 1));
                    col++;
                }
                error = (yasq2 * (col + 1) * col)          +
                        (xasq2 * ((row * (row - 2)) + 1)) +
                        (yasq  * (1 - xasq2));
                while(row >= 0) {
                    scans[row] = col;
                    if(error <= 0) {
                        col++;
                        error += yasq4 * col;
                    }
                    row--;
                    error += xasq2 * (2 - (row << 1));
                }
                for(row = y1; row <= y2; row++) {
                    col = iabs(yc - row);
                    if(!filled && (col < ya)) {
                        x1 = xc - scans[col];
                        x2 = xc - scans[col + 1];
                        if(x1 < x2) x2--;
                        for( ; ; ) {
                            clip_ordxrange_(CURC,x1,x2,break,);
                            (*f->scan)(
                                (x1  + CURC->gc_xoffset),
                                (row + CURC->gc_yoffset),
                                (x2  - x1 + 1),
                                c
                            );
                            break;
                        }
                        x1 = xc + scans[col + 1];
                        x2 = xc + scans[col];
                        if(x1 < x2) x1++;
                    }
                    else {
                        x1 = xc - scans[col];
                        x2 = xc + scans[col];
                    }
                    clip_ordxrange_(CURC,x1,x2,continue,);
                    (*f->scan)(
                        (x1  + CURC->gc_xoffset),
                        (row + CURC->gc_yoffset),
                        (x2  - x1 + 1),
                        c
                    );
                }
            }
        }
        reset_alloca();
        mouse_unblock();
}

