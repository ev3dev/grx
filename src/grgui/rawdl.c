/**
 ** rawdl.c ---- Mini GUI for MGRX, raw data list
 **
 ** Copyright (C) 2020 Mariano Alvarez Fernandez
 ** [e-mail: malfer at telefonica dot net]
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
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grguip.h"

typedef struct _guiRawDataList {
    int type;             // RDL type
    void **reg;           // string list
    int nreg;             // num of strings in list
    int vreg;             // max num of strings visible (gen)
    int sreg;             // selected reg
    int freg;             // first reg in screen (gen)
    int x, y;             // position
    int dx, dy;           // latest displacement requested to paint
    int width, height;    // dimensions
    int ewidth, eheight;  // effective dimensions (without scb) (gen)
    GrColor bg, fg;       // background, foreground colors
    int lineheight;       // height of a line (gen)
    GUIScrollbar *vscb;   // vertical scrollbar (can be NULL)
    int selected;         // the whole rdl has the focus
    int pressed;          // left mouse pressed
} GUIRawDataList;

static int check_freg(GUIRawDataList *rdl);
static void paint_selected_box(GUIRawDataList *rdl, int blt);
static void show_rdl_item(GUIRawDataList *rdl, int ind);

/***************************/

GUIRawDataList * GUIRDLCreate(int type, int x, int y, int width, int height,
                              GrColor bg, GrColor fg, void **reg, int nreg, int sreg)
{
    GUIRawDataList *rdl;
    int xscb;

    rdl = malloc(sizeof(GUIRawDataList));
    if (rdl == NULL) return NULL;

    rdl->type = type;
    rdl->x = x;
    rdl->y = y;
    rdl->width = width;
    rdl->height = height;
    rdl->bg = bg;
    rdl->fg = fg;
    rdl->reg = reg;
    rdl->nreg = nreg;
    rdl->sreg = sreg;

    rdl->dx = 0;
    rdl->dy = 0;
    rdl->selected = 0;
    rdl->pressed = 0;
    rdl->vscb = NULL;
    rdl->freg = 0;
    rdl->lineheight = _objectfont->h.height;
    rdl->vreg = (rdl->height - 6) / rdl->lineheight;
    if (rdl->vreg >= rdl->nreg) {
        rdl->vreg = rdl->nreg;
        rdl->ewidth = rdl->width;
        rdl->eheight = rdl->height;
    } else {
        xscb = rdl->x + rdl->width - GUI_SB_DEFAULT_THICK - 2;
        rdl->vscb = GUIScrollbarCreate(GUI_TSB_VERTICAL, xscb, rdl->y+2,
                                       GUI_SB_DEFAULT_THICK, rdl->height-4);
        if (rdl->vscb == NULL) {
            free(rdl);
            return NULL;
        }
        // we know freg = 0, so only check sreg >= vreg
        if (rdl->sreg >= rdl->vreg) rdl->freg = rdl->sreg - rdl->vreg + 1;
        GUIScrollbarSetLimits(rdl->vscb, 0, rdl->nreg-1,
                              rdl->freg, rdl->freg+rdl->vreg-1);
        rdl->ewidth = rdl->width - GUI_SB_DEFAULT_THICK - 1;
        rdl->eheight = rdl->height;
    }
 
    return rdl;
}

/***************************/

void GUIRDLDestroy(GUIRawDataList *rdl)
{
    if (rdl->vscb)
        free(rdl->vscb);
    free(rdl);
}

/***************************/

void GUIRDLSetSreg(GUIRawDataList *rdl, int sreg)
{
    rdl->sreg = sreg;
    if (rdl->sreg < rdl->freg) rdl->freg = rdl->sreg;
    if (rdl->sreg >= rdl->vreg+rdl->freg)
        rdl->freg = rdl->sreg - rdl->vreg + 1;
}

/***************************/

int GUIRDLGetSreg(GUIRawDataList *rdl)
{
    return rdl->sreg;
}

/***************************/

void GUIRDLPaint(GUIRawDataList *rdl, int dx, int dy, int blt)
{
    int x, y, i;

    rdl->dx = dx;
    rdl->dy = dy;

    x = rdl->x + rdl->dx;
    y = rdl->y + rdl->dy;

    GrBox(x, y, x+rdl->width-1, y+rdl->height-1, _objectlcolor);
    GrFilledBox(x+1, y+1, x+rdl->width-2, y+rdl->height-2, rdl->bg);
    if (rdl->selected) {
        paint_selected_box(rdl, 0);
    }
    for (i=0; i<rdl->vreg; i++)
        show_rdl_item(rdl, i);
    if (rdl->vscb) {
        GUIScrollbarSetLimits(rdl->vscb, 0, rdl->nreg-1,
                              rdl->freg, rdl->freg+rdl->vreg-1);
        GUIScrollbarPaint(rdl->vscb, rdl->dx, rdl->dy, 0);
    }
    if (blt)
        GUIDBCurCtxBltRectToScreen(x, y, x+rdl->width-1, y+rdl->height-1);
}

/***************************/

void GUIRDLSetSelected(GUIRawDataList *rdl, int selected, int paint, int blt)
{
    rdl->selected = selected;
    if (paint)
        paint_selected_box(rdl, blt);
}

/***************************/

static int check_freg(GUIRawDataList *rdl)
{
    if (rdl->sreg < rdl->freg) {
        rdl->freg = rdl->sreg;
        return -1;
    }
    if (rdl->sreg >= rdl->vreg+rdl->freg) {
        rdl->freg = rdl->sreg - rdl->vreg + 1;
        return 1;
    }
    return 0;
}

/***************************/

static void paint_selected_box(GUIRawDataList *rdl, int blt)
{
    GrLineOption glo;
    int x, y;

    x = rdl->x + rdl->dx;
    y = rdl->y + rdl->dy;
 
    if (rdl->selected) {
        glo.lno_color = rdl->fg;
        glo.lno_width = 1;
        glo.lno_pattlen = 2;
        glo.lno_dashpat = (unsigned char *)"\2\1";
        GrCustomBox(x+2, y+2, x+rdl->ewidth-3, y+rdl->height-3, &glo);
    } else {
        GrBox(x+2, y+2, x+rdl->ewidth-3, y+rdl->height-3, rdl->bg);
    }
    if (blt)
        GUIDBCurCtxBltRectToScreen(x+2, y+2, x+rdl->ewidth-3, y+rdl->height-3);
}

/***************************/

static void show_rdl_item(GUIRawDataList *rdl, int ind)
{
    int x, y, selected = 0;

    if (ind >= rdl->vreg) return;
    if (ind < 0) return;
    
    if (ind+rdl->freg == rdl->sreg) selected = 1;
    x = rdl->x + rdl->dx + 3;
    y = rdl->y + rdl->dy + 3 + ind * rdl->lineheight;

    GrFilledBox(x, y, x+rdl->ewidth-7, y+rdl->lineheight-1,
                selected ? rdl->fg : rdl->bg);

    GrSetClipBox(x+1, y, x+rdl->ewidth-8, y+rdl->lineheight-1);
    _objectgenopt.txo_fgcolor = selected ? rdl->bg : rdl->fg;
    GrDrawString(rdl->reg[ind+rdl->freg], 0, x+1, y+rdl->lineheight/2,
                     &_objectgenopt);
    GrResetClipBox();
}

/***************************/

int GUIRDLProcessEvent(GUIRawDataList *rdl, GrEvent *ev)
// returns 1=event consumed, 0=not consumed, 2=element selected by user

{
    long c;
    int ind, old, pos;
    int change = 0;
    int move = 0;
    int res = 0;
    int j, x, y;

    ind = rdl->sreg - rdl->freg;

    switch (ev->type) {

        case GREV_MOUSE :
            c = ev->p1;
            x = ev->p2 - rdl->dx;
            y = ev->p3 - rdl->dy;
            switch (c) {
                case GRMOUSE_LB_PRESSED :
                    if (GrCheckCoordInto(x, y, rdl->x, rdl->y,
                        rdl->ewidth, rdl->eheight)) {
                        rdl->pressed = 1;
                        pos = (y - rdl->y -3) / rdl->lineheight;
                        if ((pos >= 0) && (pos < rdl->vreg)) {
                            if (ind != pos) {
                                old = ind;
                                ind = pos;
                                rdl->sreg = rdl->freg + ind;
                                change = 1;
                            }
                        res = 1;
                        }
                    } else if (rdl->vscb) {
                        return GUIScrollbarProcessEvent(rdl->vscb, ev);
                    }
                    break;
                case GRMOUSE_LB_RELEASED :
                    if (rdl->pressed) {
                        rdl->pressed = 0;
                        pos = (y - rdl->y -3) / rdl->lineheight;
                        if (pos == ind) return 2;
                        res = 1;
                    } else if (rdl->vscb) {
                        return GUIScrollbarProcessEvent(rdl->vscb, ev);
                    }
                    break;
                case GRMOUSE_B4_RELEASED :
                    if (rdl->freg > 0) {
                        rdl->freg--;
                        change = move = 1;
                    }
                    res = 1;
                    break;
                case GRMOUSE_B5_RELEASED :
                    if (rdl->freg < rdl->nreg-rdl->vreg) {
                        rdl->freg++;
                        change = move = 1;
                    }
                    res = 1;
                    break;
            }
            break;

        case GREV_MMOVE :
            y = ev->p3 - rdl->dy;
            if (rdl->pressed) {
                if (ev->p1 & GRMOUSE_LB_STATUS) {
                    pos = (y - rdl->y -3) / rdl->lineheight;
                    if ((pos >= 0) && (pos < rdl->vreg)) {
                        if (ind != pos) {
                            old = ind;
                            ind = pos;
                            change = 1;
                            rdl->sreg = rdl->freg + ind;
                        }
                    }
                }
                res = 1;
            } else if (rdl->vscb) {
                return GUIScrollbarProcessEvent(rdl->vscb, ev);
            }
            break;

        case GREV_KEY :
            c = ev->p1;
            switch (c) {
                case GrKey_Up :
                    if (rdl->sreg > 0) {
                        rdl->sreg--;
                        old = ind;
                        ind = rdl->sreg - rdl->freg;
                        change = 1;
                        if (check_freg(rdl) != 0)
                            move = 1;
                    }
                    res = 1;
                    break;
                case GrKey_Down :
                    if (rdl->sreg < rdl->nreg-1) {
                        rdl->sreg++;
                        old = ind;
                        ind = rdl->sreg - rdl->freg;
                        change = 1;
                        if (check_freg(rdl) != 0)
                            move = 1;
                    }
                    res = 1;
                    break;
                case GrKey_PageUp :
                    if (check_freg(rdl) != 0) {
                        change = move = 1;
                    } else if (rdl->freg == 0) {
                        if (rdl->sreg > 0) {
                            rdl->sreg = 0;
                            old = ind;
                            ind = rdl->sreg - rdl->freg;
                            change = 1;
                        }
                    } else if (rdl->freg > 0) {
                        rdl->freg -= rdl->vreg - 1;
                        rdl->sreg -= rdl->vreg - 1;
                        if (rdl->freg < 0) rdl->freg = 0;
                        if (rdl->sreg < 0) rdl->sreg = 0;
                        change = move = 1;
                    }
                    res = 1;
                    break;
                case GrKey_PageDown :
                    if (check_freg(rdl) != 0) {
                        change = move = 1;
                    } else if (rdl->freg == rdl->nreg - rdl->vreg) {
                        if (rdl->sreg < rdl->nreg-1) {
                            rdl->sreg = rdl->nreg - 1;
                            old = ind;
                            ind = rdl->sreg - rdl->freg;
                            change = 1;
                        }
                    } else if (rdl->freg < rdl->nreg - rdl->vreg) {
                        rdl->freg += rdl->vreg - 1;
                        rdl->sreg += rdl->vreg - 1;
                        if (rdl->freg > rdl->nreg-rdl->vreg)
                            rdl->freg = rdl->nreg - rdl->vreg;
                        if (rdl->sreg >= rdl->nreg)
                            rdl->sreg = rdl->nreg - 1;
                        change = move = 1;
                    }
                    res = 1;
                    break;
                case GrKey_Return :
                    if (check_freg(rdl) != 0) {
                        change = move = 1;
                    }
                    res = 2;
            }
            break;

        case GREV_NULL :
            if (rdl->vscb) {
                return GUIScrollbarProcessEvent(rdl->vscb, ev);
            }
            break;

        case GREV_SCBVERT:
            if (rdl->vscb && GUIScrollbarGetAid(rdl->vscb) == ev->p2) {
                if (rdl->freg != ev->p1) {
                    rdl->freg = ev->p1;
                    change = 1;
                    move = 1;
                }
                res = 1;
            }
            break;
    }

    if (change) {
        if (move) {
            for(j=0; j<rdl->vreg; j++)
                show_rdl_item(rdl, j);
            if (rdl->vscb) {
                GUIScrollbarSetLimits(rdl->vscb, 0, rdl->nreg-1,
                                      rdl->freg, rdl->freg+rdl->vreg-1);
                GUIScrollbarPaint(rdl->vscb, rdl->dx, rdl->dy, 0);
            }
        } else {
            show_rdl_item(rdl, old);
            show_rdl_item(rdl, ind);
        }
        x = rdl->x + rdl->dx;
        y = rdl->y + rdl->dy;
        GUIDBCurCtxBltRectToScreen(x, y, x+rdl->width-1, y+rdl->height-1);
    }

    return res;
}
