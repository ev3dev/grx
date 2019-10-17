/**
 ** life_db.c ---- Conway's life program
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
 **
 ** This is a test/demo file of the GRX graphics library.
 ** You can use GRX test/demo files as you want.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** 190819 Double buffer version of life, much faster than the normal 
 **        version in X11 and Win32, a bit slower in Linux-fb and djgpp
 **/

#include "test.h"
#include "rand.h"

#include <malloc.h>
#include <string.h>
#include <time.h>

TESTFUNC(life)
{
    int  W = GrSizeX();
    int  H = GrSizeY();
    char **map[3],**old2,**old,**cur,**aux;
    int  *xp,*xn,*yp,*yn;
    int  which,x,y,gen;
    GrColor c[2], c2[2];
    long thresh;
    GrEvent ev;
    GrContext *grc;

    GrEventGenExpose(GR_GEN_EXPOSE_YES);

    if ( (grc = GrCreateContext( GrScreenX(),GrScreenY(),NULL,NULL )) == NULL ) {
        printf("Error creating double buffer\n");
        return;
    }
    
    for(which = 0; which < 3; which++) {
        cur = malloc(H * sizeof(char *));
        if(!cur) return;
        map[which] = cur;
        for(y = 0; y < H; y++) {
            cur[y] = malloc(W);
            if(!cur[y]) return;
        }
    }
    xp = malloc(W * sizeof(int));
    xn = malloc(W * sizeof(int));
    yp = malloc(H * sizeof(int));
    yn = malloc(H * sizeof(int));
    if(!xp || !xn || !yp || !yn) return;
    for(x = 0; x < W; x++) {
        xp[x] = (x + W - 1) % W;
        xn[x] = (x + W + 1) % W;
    }
    for(y = 0; y < H; y++) {
        yp[y] = (y + H - 1) % H;
        yn[y] = (y + H + 1) % H;
    }
    c[0] = GrBlack();
    c[1] = GrWhite();
    //c2[0] = GrAllocColor2(0x555555);
    c2[0] = GrAllocColor2(0xAA0000);
    c2[1] = GrAllocColor2(0xFFFF55);

    old2 = map[0];
    old = map[1];
    cur = map[2];
    SRND((int)time(NULL));

    GrMouseEraseCursor();
    GrSetContext( grc );
    for(y = 0; y < H; y++) {
        for(x = 0; x < W; x++) {
            int ii = RND() % 53;
            while(--ii >= 0) RND();
            old2[y][x] = old[y][x] = (((RND() % 131) > 107) ? 1 : 0);
            GrPlotNC(x,y,c[(int)old[y][x]]);
        }
    }
     /* djgpp does'nt work if not set the screen context before blit */
    GrSetContext( NULL );
    GrBitBlt(GrScreenContext(),0,0,grc,0,0,grc->gc_xmax,grc->gc_ymax,GrWRITE);
    //GrMouseDisplayCursor();
    
    thresh = (((unsigned long)RND() << 16) + RND()) % 1003567UL;
    gen    = (Argc > 0) ? 1 : 0;
    while (1) {
        for(y = 0; y < H; y++) {
            char *prow = old[yp[y]];
            char *crow = old[y];
            char *nrow = old[yn[y]];
            char *curr = cur[y];
            for(x = 0; x < W; x++) {
                int  xprev = xp[x];
                int  xnext = xn[x];
                char live  = prow[xprev] +
                             prow[x]     +
                             prow[xnext] +
                             crow[xprev] +
                             crow[xnext] +
                             nrow[xprev] +
                             nrow[x]     +
                             nrow[xnext];
                live = ((live | crow[x]) == 3) ? 1 : 0;
                if(--thresh <= 0) {
                    live  ^= gen;
                    thresh = (((unsigned long)RND() << 16) + RND()) % 1483567UL;
                }
                curr[x] = live;
            }
        }
        //GrMouseEraseCursor();
        GrSetContext( grc );
        for(y = 0; y < H; y++) {
            char *curr = cur[y];
            char *oldr = old[y];
            char *oldr2 = old2[y];
            for(x = 0; x < W; x++) {
                if(curr[x] != oldr[x])
                    GrPlotNC(x,y,c2[(int)curr[x]]);
                else if(oldr2[x] != oldr[x])
                    GrPlotNC(x,y,c[(int)oldr[x]]);
            }
        }
        GrSetContext( NULL );
        GrBitBlt(GrScreenContext(),0,0,grc,0,0,grc->gc_xmax,grc->gc_ymax,GrWRITE);
        //GrMouseDisplayCursor();

        aux = old2;
        old2 = old;
        old = cur;
        cur = aux;
        
        if (GrEventCheck()) {
            GrEventWait(&ev);
            if (ev.type == GREV_EXPOSE) {
                printf("expose %ld %ld %ld %ld %d\n",
                       ev.p1, ev.p2, ev.p3, ev.p4, ev.kbstat);
                if (ev.kbstat == 0) {// no more EXPOSE events follow
                    //GrMouseEraseCursor();
                    GrBitBlt(GrScreenContext(),0,0,grc,0,0,
                             grc->gc_xmax,grc->gc_ymax,GrWRITE);
                    //GrMouseDisplayCursor();
                }
            } else
                break;
        }
    }
}
