/*
 * drwcpoly.c ---- draw the outline of a custom (wide and/or dashed) polygon
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <grx/draw.h>
#include <grx/wideline.h>

#include "arith.h"
#include "clipping.h"
#include "globals.h"
#include "libgrx.h"
#include "memcopy.h"
#include "mouse.h"
#include "shapes.h"

/*
 * update the end point of line #1 and the starting point of line #2
 * so that they intersect
 */
static void intersect(GrxPoint *l1s, GrxPoint *l1e, GrxPoint *l2s, GrxPoint *l2e)
{
#define x11 l1s->x
#define y11 l1s->y
#define x12 l1e->x
#define y12 l1e->y
#define x21 l2s->x
#define y21 l2s->y
#define x22 l2e->x
#define y22 l2e->y
    if (x12 != x21 || y12 != y21) {
        int dx1 = x12 - x11;
        int dy1 = y12 - y11;
        int dx2 = x22 - x21;
        int dy2 = y22 - y21;
        long det = imul32(dx2, dy1) - imul32(dx1, dy2);
        if (det != 0) {
            /* Compute t for the parametric equation of line #2 */
            /* then: x = x21 + dx2 * t2, and y = y21 + dy2 * t2 */
            /* but do this with integer arithmetic */
            /* and do rounding instead of truncation */
            long det_t2 = imul32(dx1, (y21 - y11)) - imul32(dy1, (x21 - x11));
            long ldet = labs(det);
            /* make sure we're still near old start point of line #2 */
            if (labs(det_t2) < (((ldet << 1) + ldet) >> 1)) {
                int xdif2 = (int)(((long)(dx2 << 1) * det_t2) / det);
                int ydif2 = (int)(((long)(dy2 << 1) * det_t2) / det);
                if (xdif2 > 0)
                    xdif2++;
                if (ydif2 > 0)
                    ydif2++;
                xdif2 = x21 + (xdif2 >> 1);
                ydif2 = y21 + (ydif2 >> 1);
                /* don't create triangles */
                if (xdif2 != x11 && xdif2 != x22 && ydif2 != y11 && ydif2 != y22) {
                    l1e->x = l2s->x = xdif2;
                    l1e->y = l2s->y = ydif2;
                    return;
                }
            }
        }
        {
            /*
             * no good intersection point till now
             * Check mean point and its eight neighbours for
             * best compromise intersection point
             *
             *  y-y11   y12-y11           y-y21   y22-y21
             *  ----- - ------- = 0  and  ----- - ------- = 0
             *  x-x11   x12-x11           x-x21   x22-x21
             *
             * should hold for intersection point (x,y)
             * Measuring the errors for both lines:
             *
             * e1 = (x12-x11)(y-y11)-(x-x11)(y12-y11) = dx1(y-y11)-(x-x11)dy1
             * e2 = (x22-x21)(y-y21)-(x-x21)(y22-y21) = dx2(y-y21)-(x-x21)dy2
             *
             * search minimal err = |e1| + |e2|
             */
            static int xr[9] = { 0, -1, 0, 1, 0, -1, 1, 1, -1 };
            static int yr[9] = { 0, 0, 1, 0, -1, 1, 1, -1, -1 };
            int xc = (x12 + x21) >> 1;
            int yc = (y12 + y21) >> 1;
            int xb = 0, yb = 0;
            int i;
            long minerr = 0, err;
            for (i = 0; i < 9; ++i) {
                int x = xc + xr[i];
                int y = yc + yr[i];
                long e1 = imul32(dx1, (y - y11)) - imul32(dy1, (x - x11));
                long e2 = imul32(dx2, (y - y21)) - imul32(dy2, (x - x21));
                err = labs(e1) + labs(e2);
                if (i == 0 || err < minerr) {
                    minerr = err;
                    xb = xr[i];
                    yb = yr[i];
                    if (minerr == 0)
                        break;
                }
            }
            l1e->x = l2s->x = xc + xb;
            l1e->y = l2s->y = yc + yb;
        }
    }
#undef x11
#undef y11
#undef x12
#undef y12
#undef x21
#undef y21
#undef x22
#undef y22
}

/*
 * generate the four corner points of a wide line segment
 *
 * p1 end : rect[0]  rect[1]
 * p2 end : rect[2]  rect[3]
 */
static void genrect(GrxPoint p1, GrxPoint p2, int w, GrxPoint rect[4])
{
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int wx, wy, wx1, wy1, wx2, wy2;

    if (dx == 0) {
        wx = w;
        wy = 0;
    }
    else if (dy == 0) {
        wx = 0;
        wy = w;
    }
    else {
        unsigned long minerr, error = ~0UL, w2 = imul32(w, w);
        int mindelta = umin(iabs(dx), iabs(dy));
        int maxdelta = umax(iabs(dx), iabs(dy));
        wx1 = w / 2;
        if (wx1 <= 0)
            wx1 = 1;
        if (wx1 + wx1 < w)
            ++wx1;
        wy1 = 0;
        do {
            wx = wx1++;
            wy = wy1;
            wy1 = urscale(wx1, mindelta, maxdelta);
            minerr = error;
            error = imul32(wx1, wx1) + imul32(wy1, wy1) - w2;
            error = labs(error);
        } while (error <= minerr);
        if (iabs(dx) > iabs(dy))
            iswap(wx, wy);
    }
    if (dx < 0)
        wy = (-wy);
    if (dy >= 0)
        wx = (-wx);
    wx1 = -(wx >> 1);
    wy1 = -(wy >> 1);
    wx2 = wx + wx1;
    wy2 = wy + wy1;
    if ((wx1 + wx2) < 0)
        wx1++, wx2++;
    if ((wy1 + wy2) < 0)
        wy1++, wy2++;
    rect[0].x = p1.x + wx1;
    rect[0].y = p1.y + wy1;
    rect[1].x = p1.x + wx2;
    rect[1].y = p1.y + wy2;
    rect[2].x = p2.x + wx2;
    rect[2].y = p2.y + wy2;
    rect[3].x = p2.x + wx1;
    rect[3].y = p2.y + wy1;
}

/*
 * working version of the line pattern and fill argument structures
 */
typedef struct {
    int w;                     /* line width */
    int psegs;                 /* number of pattern segments */
    int plength;               /* total length of pattern in pixels */
    int ppos;                  /* current pattern position (modulo plength) */
    int on;                    /* is the pattern currently on ? */
    const unsigned char *patt; /* the pattern bits */
    GrFiller *f;               /* the filler functions */
    GrFillArg c;               /* the filler argument */
} linepatt;

static void solidsegment1(
    GrxPoint p1, GrxPoint p2, const GrxPoint *prev, const GrxPoint *next, linepatt *p)
{
    int x1 = p1.x, y1 = p1.y;
    int x2 = p2.x, y2 = p2.y;
    (*p->f->line)(
        (x1 + CURC->x_offset), (y1 + CURC->y_offset), (x2 - x1), (y2 - y1), p->c);
}

static void solidsegmentw(
    GrxPoint p1, GrxPoint p2, const GrxPoint *prev, const GrxPoint *next, linepatt *p)
{
    GrxPoint rect[4], prect[4], nrect[4];
    genrect(p1, p2, p->w, rect);
    if (prev)
        genrect(*prev, p1, p->w, prect);
    if (next)
        genrect(p2, *next, p->w, nrect);
    if (prev && next) {
        GrxPoint point;
        point.x = rect[1].x;
        point.y = rect[1].y;
        intersect(&prect[1], &prect[2], &rect[1], &rect[2]);
        intersect(&point, &rect[2], &nrect[1], &nrect[2]);
        point.y = rect[0].x;
        point.y = rect[0].y;
        intersect(&prect[0], &prect[3], &rect[0], &rect[3]);
        intersect(&point, &rect[3], &nrect[0], &nrect[3]);
    }
    else if (prev) {
        intersect(&prect[1], &prect[2], &rect[1], &rect[2]);
        intersect(&prect[0], &prect[3], &rect[0], &rect[3]);
    }
    else if (next) {
        intersect(&rect[1], &rect[2], &nrect[1], &nrect[2]);
        intersect(&rect[0], &rect[3], &nrect[0], &nrect[3]);
    }
    _GrScanConvexPoly(4, rect, p->f, p->c);
}

static void dashedsegment(GrxPoint p1, GrxPoint p2, const GrxPoint *prev,
    const GrxPoint *next, linepatt *p,
    void (*doseg)(GrxPoint, GrxPoint, const GrxPoint *, const GrxPoint *, linepatt *))
{
    int on, pos, len, seg;
    int x, y, dx, dy;
    int error, erradd, errsub, count;
    int xinc1, xinc2, yinc1, yinc2;
    GrxPoint start = { 0, 0 };
    GrxPoint end = { 0, 0 };
    int se_init;

    /* find the current starting segment for the pattern */
    pos = (p->ppos %= p->plength);
    for (on = 1, seg = 0;;) {
        len = p->patt[seg];
        if (pos < len) {
            len -= pos;
            break;
        }
        if (++seg >= p->psegs)
            seg = 0;
        on ^= 1;
        pos -= len;
    }
    /* Can't have a zero length element here */

    /* set up line drawing */
    x = p1.x;
    dx = p2.x - x;
    y = p1.y;
    dy = p2.y - y;
    if (dx >= 0) {
        xinc2 = 1;
    }
    else {
        xinc2 = -1;
        dx = -dx;
    }
    if (dy >= 0) {
        yinc2 = 1;
    }
    else {
        yinc2 = -1;
        dy = -dy;
    }
    if (dx >= dy) {
        count = dx + 1;
        error = dx >> 1;
        errsub = dy;
        erradd = dx;
        xinc1 = xinc2;
        yinc1 = 0;
    }
    else {
        count = dy + 1;
        error = dy >> 1;
        errsub = dx;
        erradd = dy;
        yinc1 = yinc2;
        xinc1 = 0;
    }
    se_init = 0;
    if (on) {
        start.x = x;
        start.y = y;
        se_init = 1;
    }
    else {
        prev = NULL;
    }
    /* go */
    while (--count >= 0) {
        if (on) {
            end.x = x;
            end.y = y;
            se_init |= 2;
        }
        if ((error -= errsub) < 0) {
            error += erradd;
            x += xinc2;
            y += yinc2;
        }
        else {
            x += xinc1;
            y += yinc1;
        }
        if (--len <= 0) {
            /* end of current segment */
            int old_state = on;
            do {
                if (++seg >= p->psegs)
                    seg = 0;
                len = p->patt[seg];
                on ^= 1;
            } while (len == 0);
            if (!old_state && on && count > 0) {
                start.x = x;
                start.y = y;
                se_init = 1;
            }
            else if (old_state && !on) {
                (*doseg)(start, end, prev, NULL, p);
                prev = NULL;
                se_init = 0;
            }
            /* else: zero length element(s), continue current segment */
        }
    }
    if (on && se_init == 3) {
        (*doseg)(start, end, prev, next, p);
    }
    p->on = on;
}

static void dashedsegment1(
    GrxPoint p1, GrxPoint p2, const GrxPoint *prev, const GrxPoint *next, linepatt *p)
{
    dashedsegment(p1, p2, prev, next, p, solidsegment1);
}

static void dashedsegmentw(
    GrxPoint p1, GrxPoint p2, const GrxPoint *prev, const GrxPoint *next, linepatt *p)
{
    dashedsegment(p1, p2, prev, next, p, solidsegmentw);
}

void _GrDrawCustomPolygon(int n, const GrxPoint *pt, const GrxLineOptions *lp,
    GrFiller *f, GrFillArg c, int doClose, int circle)
{
#define x1 start.x
#define y1 start.y
#define x2 end.x
#define y2 end.y
    int i;
    GrxPoint start, end;
    void (*doseg)(GrxPoint, GrxPoint, const GrxPoint *, const GrxPoint *, linepatt *);
    linepatt p;
    GrxContext preclip;
    if (n < 2)
        return;
    /* set up working pattern */
    p.f = f;
    p.c = c;
    p.w = imax((lp->width - 1), 0);
    p.ppos = 0;
    p.patt = &lp->dash_pattern0;
    p.psegs = imin(imax(lp->n_dash_patterns, 0), 8);
    p.plength = 0;
    for (i = 0; i < p.psegs; i++) {
        /*          if(!p.patt[i]) { p.plength = 0; break; } */
        p.plength += p.patt[i];
    }
    if (p.plength)
        doseg = p.w ? dashedsegmentw : dashedsegment1;
    else {
        if (p.psegs && p.patt[0] == 0)
            return; /* nothing to do */
        doseg = p.w ? solidsegmentw : solidsegment1;
    }
    /* preclip */
    x1 = x2 = pt[0].x;
    y1 = y2 = pt[0].y;
    for (i = 1; i < n; i++) {
        GrxPoint ppt = pt[i];
        if (x1 > ppt.x)
            x1 = ppt.x;
        if (x2 < ppt.x)
            x2 = ppt.x;
        if (y1 > ppt.y)
            y1 = ppt.y;
        if (y2 < ppt.y)
            y2 = ppt.y;
    }
    sttcopy(&preclip, CURC);
    preclip.x_clip_low -= p.w;
    preclip.y_clip_low -= p.w;
    preclip.x_clip_high += p.w;
    preclip.y_clip_high += p.w;
    clip_ordbox((&preclip), x1, y1, x2, y2);
    mouse_block(CURC, x1, y1, x2, y2);
    /* do the polygon segments */
    if (doClose) {
        GrxPoint p1 = pt[0], p2 = pt[n - 1];
        if ((n > 1) && (p1.x == p2.x) && (p1.y == p2.y))
            n--;
        if (n < 3)
            doClose = FALSE;
    }
    for (i = 0; i < n; i++) {
        int clipped, xmajor, length;
        GrxPoint p1, p2;
        const GrxPoint *prev, *next;

        if (!(i + doClose))
            continue;
        p1 = pt[(i + n - 1) % n];
        p2 = pt[i];
        prev = ((i > 1) || doClose) ? &pt[(i + n - 2) % n] : NULL;
        next = ((i < (n - 1)) || doClose) ? &pt[(i + 1) % n] : NULL;
        x1 = p1.x;
        y1 = p1.y;
        x2 = p2.x;
        y2 = p2.y;
        clipped = 0;
        xmajor = iabs(x1 - x2);
        length = iabs(y1 - y2);
        if (xmajor > length) {
            length = xmajor;
            xmajor = 1;
        }
        else
            xmajor = 0;
        clip_line_((&preclip), x1, y1, x2, y2, goto outside, clipped = p.plength);
        if (clipped) {
            clipped = xmajor ? iabs(p1.x - x1) : iabs(p1.y - y1);
            p.ppos += clipped;
            length -= clipped;
        }
        (*doseg)(start, end, prev, next, &p);
    outside:
        p.ppos += length;
    }
    mouse_unblock();
#undef x1
#undef y1
#undef x2
#undef y2
}
