/**
 ** gradient.c ---- gradient functions
 **
 ** Copyright (C) 2001 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **
 **/

#include <math.h>

#include "libgrx.h"

GrPattern *GrCreateLinGradient(int xi, int yi, int xf, int yf)
{
    GrGradient *g;
    float dx, dy;

    g = malloc(sizeof(GrGradient));
    if (g == NULL) return NULL;
    
    g->grd_ptype = GR_PTYPE_GRADIENT;
    g->grd_mode = GR_LINEAR_GRADIENT;
    g->grd_oper = GrWRITE;
    g->grd_memflags = 1;
    g->grd_ld.xi = xi;
    g->grd_ld.yi = yi;
    g->grd_ld.xf = xf;
    g->grd_ld.yf = yf;
    dx = xf - xi;
    dy = yf - yi;
    g->grd_ld.dist_if = sqrtf(dx*dx + dy*dy);
    g->grd_nstops = 0;
    g->grd_genctbl = 0;

    return (GrPattern *)g;
}

GrPattern *GrCreateRadGradient(int xc, int yc, int r)
{
    GrGradient *g;

    g = malloc(sizeof(GrGradient));
    if (g == NULL) return NULL;
    
    g->grd_ptype = GR_PTYPE_GRADIENT;
    g->grd_mode = GR_RADIAL_GRADIENT;
    g->grd_oper = GrWRITE;
    g->grd_memflags = 1;
    g->grd_rd.xc = xc;
    g->grd_rd.yc = yc;
    g->grd_rd.r = r;
    g->grd_nstops = 0;
    g->grd_genctbl = 0;

    return (GrPattern *)g;
}

int GrAddGradientStop(GrPattern *p, int dist, GrColor c)
{
    GrGradient *g;

    if (p->gp_ptype != GR_PTYPE_GRADIENT) return 0;
    g = (GrGradient *)p;

    if (g->grd_nstops >= GR_GRADIENT_MAXSTOPS) return 0;
    if (dist < 0) dist = 0;
    if (dist > 255) dist = 255;
    if (g->grd_nstops > 0 &&
        dist <= g->grd_stop[g->grd_nstops-1].dist) return 0;
    g->grd_stop[g->grd_nstops].dist = dist;
    g->grd_stop[g->grd_nstops].c = c;
    g->grd_nstops++;
    g->grd_genctbl = 0;

    return 1;
}

int GrGenGradientColorTbl(GrPattern *p)
{
    int fstep, nsteps, step100, i, s;
    int r, g, b, r1, g1, b1, r2, g2, b2;
    int cvg, rcvg;
    GrColor *ctbl;
    GrGradient *grd;

    if (p->gp_ptype != GR_PTYPE_GRADIENT) return 0;
    grd = (GrGradient *)p;

    if (grd->grd_genctbl) return 1;
    if (grd->grd_nstops < 1) return 0;

    grd->grd_ctbl[0] = 256;
    ctbl = &(grd->grd_ctbl[1]);

    for (i=0; i<=grd->grd_stop[0].dist-1; i++) {
        ctbl[i] = grd->grd_stop[0].c;
    }

    for (s=1; s<grd->grd_nstops; s++) {
        fstep = grd->grd_stop[s-1].dist + 1;
        nsteps = grd->grd_stop[s].dist - fstep;
        step100 = 25500 / nsteps;
        ctbl[fstep-1] = grd->grd_stop[s-1].c;
        GrQueryColor(grd->grd_stop[s-1].c, &r1, &g1, &b1);
        GrQueryColor(grd->grd_stop[s].c, &r2, &g2, &b2);
        for (i=0; i<nsteps; i++) {
            cvg = ((i + 1) * step100) / 100;
            rcvg = 255 - cvg;
            r = ((r1 * rcvg) + (r2 * cvg)) / 255;
            g = ((g1 * rcvg) + (g2 * cvg)) / 255;
            b = ((b1 * rcvg) + (b2 * cvg)) / 255;
            ctbl[fstep+i] = GrAllocColor(r, g, b);
        }
    }

    for (i=grd->grd_stop[grd->grd_nstops-1].dist; i<=255; i++) {
        ctbl[i] = grd->grd_stop[grd->grd_nstops-1].c;
    }
    grd->grd_genctbl = 1;

    return 1;
}

GrColor _GrRadGradientColor(GrGradient *g, int x, int y, int xo, int yo)
{
    float dx, dy, dist;
    int cvg;

    dx = x - xo - g->grd_rd.xc;
    dy = y - yo - g->grd_rd.yc;
    dist = sqrtf(dx*dx + dy*dy);
    cvg = (dist * 256) / g->grd_rd.r;
    if (cvg < 0) cvg = 0;
    if (cvg > 255) cvg = 255;
    return g->grd_ctbl[cvg+1];
}

GrColor _GrLinGradientColor(GrGradient *g, int x, int y, int xo, int yo)
{
    float dmax, disti, distf, dist;
    float dx, dy, s, s2, ht;
    int cvg;

    dmax = g->grd_ld.dist_if;
    dx = x - xo - g->grd_ld.xi;
    dy = y - yo - g->grd_ld.yi;
    disti = sqrtf(dx*dx + dy*dy);
    dx = x - xo - g->grd_ld.xf;
    dy = y - yo - g->grd_ld.yf;
    distf = sqrtf(dx*dx + dy*dy);
    s = (dmax + disti + distf) / 2;
    s2 = s * (s-dmax) * (s-disti) * (s-distf);
    ht = (s2 <= 0) ? 0 : (2 * sqrtf(s2)) / dmax;
    dist = sqrtf(disti*disti - ht*ht);
    cvg = (dist * 256) / dmax;
    if (cvg < 0) cvg = 0;
    if (cvg > 255) cvg = 255;
    return g->grd_ctbl[cvg+1];
}

GrColor GrGradientColor(GrPattern *p, int x, int y, int xo, int yo)
{
    if (p->gp_ptype != GR_PTYPE_GRADIENT) return GrNOCOLOR;

    if (p->gp_grd_mode == GR_RADIAL_GRADIENT)
        return _GrRadGradientColor((GrGradient *)p, x, y, xo, yo);
    else
        return _GrLinGradientColor((GrGradient *)p, x, y, xo, yo);
}

GrPattern *GrBuildPixmapFromGradient(GrPattern *p, int xo, int yo, int w, int h)
{
    unsigned char *pixels;
    GrPattern *pixmap;
    GrGradient *g;
    int cvg, i, j;
    float dmax, disti, distf, dist;
    float dx, dy, s, s2, ht;

    if (p->gp_ptype != GR_PTYPE_GRADIENT) return NULL;
    g = (GrGradient *)p;

    pixels = malloc(w*h*sizeof(unsigned char));
    if (pixels == NULL) return NULL;

    GrGenGradientColorTbl(p);

    if (g->grd_mode == GR_RADIAL_GRADIENT) {
        dmax = g->grd_rd.r;
        for (j=0; j<h; j++) {
            for (i=0; i<w; i++) {
                dx = i + xo - g->grd_rd.xc;
                dy = j + yo - g->grd_rd.yc;
                dist = sqrtf(dx*dx + dy*dy);
                cvg = (dist * 256) / dmax;
                if (cvg < 0) cvg = 0;
                if (cvg > 255) cvg = 255;
                pixels[j*w+i] = cvg;
            }
        }
    } else {
        dmax = g->grd_ld.dist_if;
        for (j=0; j<h; j++) {
            for (i=0; i<w; i++) {
                dx = i + xo - g->grd_ld.xi;
                dy = j + yo - g->grd_ld.yi;
                disti = sqrtf(dx*dx + dy*dy);
                dx = i + xo - g->grd_ld.xf;
                dy = j + yo - g->grd_ld.yf;
                distf = sqrtf(dx*dx + dy*dy);
                s = (dmax + disti + distf) / 2;
                s2 = s * (s-dmax) * (s-disti) * (s-distf);
                ht = (s2 <= 0) ? 0 : (2 * sqrtf(s2)) / dmax;
                dist = sqrtf(disti*disti - ht*ht);
                cvg = (dist * 256) / dmax;
                if (cvg < 0) cvg = 0;
                if (cvg > 255) cvg = 255;
                pixels[j*w+i] = cvg;
            }
        }
    }

    pixmap = GrBuildPixmap((char *)pixels, w, h, g->grd_ctbl);
    free(pixels);
    return pixmap;
}
