/**
 ** rawdl2.c ---- Mini GUI for MGRX, raw data list 2
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

typedef struct _guiRawDataList2 {
    GUIRDL2Intf *f;       // interfaz
    int sreg;             // selected reg
    int vreg;             // max num of regs visible (gen)
    int freg;             // first reg in screen (gen)
    int dx, dy;           // latest displacement requested to paint
    int ewidth, eheight;  // effective dimensions (without scb) (gen)
    GUIScrollbar *vscb;   // vertical scrollbar (can be NULL)
    int selected;         // the whole rdl has the focus
    int pressed;          // left mouse pressed
} GUIRawDataList2;

static int check_freg(GUIRawDataList2 *rdl2);
static void paint_selected_box(GUIRawDataList2 *rdl2, int blt);
static void show_rdl2_item(GUIRawDataList2 *rdl2, int ind);

/***************************/

GUIRawDataList2 * GUIRDL2Create(GUIRDL2Intf *f, int sreg)
{
    GUIRawDataList2 *rdl2;
    int xscb;

    rdl2 = malloc(sizeof(GUIRawDataList2));
    if (rdl2 == NULL) return NULL;

    rdl2->f = f;
    rdl2->dx = 0;
    rdl2->dy = 0;
    rdl2->selected = 0;
    rdl2->pressed = 0;
    rdl2->vscb = NULL;
    rdl2->sreg = sreg;
    rdl2->freg = 0;
    rdl2->vreg = (rdl2->f->height - 6) / rdl2->f->regheight;
    if (rdl2->vreg >= rdl2->f->nreg) {
        rdl2->vreg = rdl2->f->nreg;
        rdl2->ewidth = rdl2->f->width;
        rdl2->eheight = rdl2->f->height;
    } else {
        xscb = rdl2->f->x + rdl2->f->width - GUI_SB_DEFAULT_THICK - 2;
        rdl2->vscb = GUIScrollbarCreate(GUI_TSB_VERTICAL, xscb, rdl2->f->y+2,
                                        GUI_SB_DEFAULT_THICK, rdl2->f->height-4);
        if (rdl2->vscb == NULL) {
            free(rdl2);
            return NULL;
        }
        // we know freg = 0, so only check sreg >= vreg
        if (rdl2->sreg >= rdl2->vreg) rdl2->freg = rdl2->sreg - rdl2->vreg + 1;
        GUIScrollbarSetLimits(rdl2->vscb, 0, rdl2->f->nreg-1,
                              rdl2->freg, rdl2->freg+rdl2->vreg-1);
        rdl2->ewidth = rdl2->f->width - GUI_SB_DEFAULT_THICK - 1;
        rdl2->eheight = rdl2->f->height;
    }
 
    return rdl2;
}

/***************************/

void GUIRDL2Destroy(GUIRawDataList2 *rdl2)
{
    if (rdl2->vscb)
        free(rdl2->vscb);
    free(rdl2);
}

/***************************/

void GUIRDL2SetSreg(GUIRawDataList2 *rdl2, int sreg)
{
    rdl2->sreg = sreg;
    if (rdl2->sreg < rdl2->freg) rdl2->freg = rdl2->sreg;
    if (rdl2->sreg >= rdl2->vreg+rdl2->freg)
        rdl2->freg = rdl2->sreg - rdl2->vreg + 1;
}

/***************************/

int GUIRDL2GetSreg(GUIRawDataList2 *rdl2)
{
    return rdl2->sreg;
}

/***************************/

void GUIRDL2Paint(GUIRawDataList2 *rdl2, int dx, int dy, int blt)
{
    int x, y, i;

    rdl2->dx = dx;
    rdl2->dy = dy;

    x = rdl2->f->x + rdl2->dx;
    y = rdl2->f->y + rdl2->dy;

    GrBox(x, y, x+rdl2->f->width-1, y+rdl2->f->height-1, _objectlcolor);
    GrFilledBox(x+1, y+1, x+rdl2->f->width-2, y+rdl2->f->height-2, rdl2->f->bg);
    if (rdl2->selected) {
        paint_selected_box(rdl2, 0);
    }
    for (i=0; i<rdl2->vreg; i++)
        show_rdl2_item(rdl2, i);
    if (rdl2->vscb) {
        GUIScrollbarSetLimits(rdl2->vscb, 0, rdl2->f->nreg-1,
                              rdl2->freg, rdl2->freg+rdl2->vreg-1);
        GUIScrollbarPaint(rdl2->vscb, rdl2->dx, rdl2->dy, 0);
    }
    if (blt)
        GUIDBCurCtxBltRectToScreen(x, y, x+rdl2->f->width-1, y+rdl2->f->height-1);
}

/***************************/

void GUIRDL2SetSelected(GUIRawDataList2 *rdl2, int selected, int paint, int blt)
{
    rdl2->selected = selected;
    if (paint)
        paint_selected_box(rdl2, blt);
}

/***************************/

static int check_freg(GUIRawDataList2 *rdl2)
{
    if (rdl2->sreg < rdl2->freg) {
        rdl2->freg = rdl2->sreg;
        return -1;
    }
    if (rdl2->sreg >= rdl2->vreg+rdl2->freg) {
        rdl2->freg = rdl2->sreg - rdl2->vreg + 1;
        return 1;
    }
    return 0;
}

/***************************/

static void paint_selected_box(GUIRawDataList2 *rdl2, int blt)
{
    GrLineOption glo;
    int x, y;

    x = rdl2->f->x + rdl2->dx;
    y = rdl2->f->y + rdl2->dy;
 
    if (rdl2->selected) {
        glo.lno_color = rdl2->f->fg;
        glo.lno_width = 1;
        glo.lno_pattlen = 2;
        glo.lno_dashpat = (unsigned char *)"\2\1";
        GrCustomBox(x+2, y+2, x+rdl2->ewidth-3, y+rdl2->eheight-3, &glo);
    } else {
        GrBox(x+2, y+2, x+rdl2->ewidth-3, y+rdl2->eheight-3, rdl2->f->bg);
    }
    if (blt)
        GUIDBCurCtxBltRectToScreen(x+2, y+2, x+rdl2->ewidth-3, y+rdl2->eheight-3);
}

/***************************/

static void show_rdl2_item(GUIRawDataList2 *rdl2, int ind)
{
    int x, y, selected = 0;

    if (ind >= rdl2->vreg) return;
    if (ind < 0) return;
    
    if (ind+rdl2->freg == rdl2->sreg) selected = 1;
    x = rdl2->f->x + rdl2->dx + 3;
    y = rdl2->f->y + rdl2->dy + 3 + ind * rdl2->f->regheight;

    GrFilledBox(x, y, x+rdl2->ewidth-7, y+rdl2->f->regheight-1,
                selected ? rdl2->f->fg : rdl2->f->bg);

    GrSetClipBox(x+1, y, x+rdl2->ewidth-8, y+rdl2->f->regheight-1);

    (rdl2->f->paint_reg)(x+1, y, ind+rdl2->freg, selected,
                         rdl2->f->fg, rdl2->f->bg);

    GrResetClipBox();
}

/***************************/

int GUIRD2LProcessEvent(GUIRawDataList2 *rdl2, GrEvent *ev)
// returns 1=event consumed, 0=not consumed, 2=element selected by user

{
    long c;
    int ind, old, pos;
    int change = 0;
    int move = 0;
    int res = 0;
    int j, x, y;

    ind = rdl2->sreg - rdl2->freg;

    switch (ev->type) {

        case GREV_MOUSE :
            c = ev->p1;
            x = ev->p2 - rdl2->dx;
            y = ev->p3 - rdl2->dy;
            switch (c) {
                case GRMOUSE_LB_PRESSED :
                    if (GrCheckCoordInto(x, y, rdl2->f->x, rdl2->f->y,
                        rdl2->ewidth, rdl2->eheight)) {
                        rdl2->pressed = 1;
                        pos = (y - rdl2->f->y -3) / rdl2->f->regheight;
                        if ((pos >= 0) && (pos < rdl2->vreg)) {
                            if (ind != pos) {
                                old = ind;
                                ind = pos;
                                rdl2->sreg = rdl2->freg + ind;
                                change = 1;
                            }
                        res = 1;
                        }
                    } else if (rdl2->vscb) {
                        return GUIScrollbarProcessEvent(rdl2->vscb, ev);
                    }
                    break;
                case GRMOUSE_LB_RELEASED :
                    if (rdl2->pressed) {
                        rdl2->pressed = 0;
                        pos = (y - rdl2->f->y -3) / rdl2->f->regheight;
                        if (pos == ind) return 2;
                        res = 1;
                    } else if (rdl2->vscb) {
                        return GUIScrollbarProcessEvent(rdl2->vscb, ev);
                    }
                    break;
                case GRMOUSE_B4_RELEASED :
                    if (rdl2->freg > 0) {
                        rdl2->freg--;
                        change = move = 1;
                    }
                    res = 1;
                    break;
                case GRMOUSE_B5_RELEASED :
                    if (rdl2->freg < rdl2->f->nreg-rdl2->vreg) {
                        rdl2->freg++;
                        change = move = 1;
                    }
                    res = 1;
                    break;
            }
            break;

        case GREV_MMOVE :
            y = ev->p3 - rdl2->dy;
            if (rdl2->pressed) {
                if (ev->p1 & GRMOUSE_LB_STATUS) {
                    pos = (y - rdl2->f->y -3) / rdl2->f->regheight;
                    if ((pos >= 0) && (pos < rdl2->vreg)) {
                        if (ind != pos) {
                            old = ind;
                            ind = pos;
                            change = 1;
                            rdl2->sreg = rdl2->freg + ind;
                        }
                    }
                }
                res = 1;
            } else if (rdl2->vscb) {
                return GUIScrollbarProcessEvent(rdl2->vscb, ev);
            }
            break;

        case GREV_KEY :
            c = ev->p1;
            switch (c) {
                case GrKey_Up :
                    if (rdl2->sreg > 0) {
                        rdl2->sreg--;
                        old = ind;
                        ind = rdl2->sreg - rdl2->freg;
                        change = 1;
                        if (check_freg(rdl2) != 0)
                            move = 1;
                    }
                    res = 1;
                    break;
                case GrKey_Down :
                    if (rdl2->sreg < rdl2->f->nreg-1) {
                        rdl2->sreg++;
                        old = ind;
                        ind = rdl2->sreg - rdl2->freg;
                        change = 1;
                        if (check_freg(rdl2) != 0)
                            move = 1;
                    }
                    res = 1;
                    break;
                case GrKey_PageUp :
                    if (check_freg(rdl2) != 0) {
                        change = move = 1;
                    } else if (rdl2->freg == 0) {
                        if (rdl2->sreg > 0) {
                            rdl2->sreg = 0;
                            old = ind;
                            ind = rdl2->sreg - rdl2->freg;
                            change = 1;
                        }
                    } else if (rdl2->freg > 0) {
                        rdl2->freg -= rdl2->vreg - 1;
                        rdl2->sreg -= rdl2->vreg - 1;
                        if (rdl2->freg < 0) rdl2->freg = 0;
                        if (rdl2->sreg < 0) rdl2->sreg = 0;
                        change = move = 1;
                    }
                    res = 1;
                    break;
                case GrKey_PageDown :
                    if (check_freg(rdl2) != 0) {
                        change = move = 1;
                    } else if (rdl2->freg == rdl2->f->nreg - rdl2->vreg) {
                        if (rdl2->sreg < rdl2->f->nreg-1) {
                            rdl2->sreg = rdl2->f->nreg - 1;
                            old = ind;
                            ind = rdl2->sreg - rdl2->freg;
                            change = 1;
                        }
                    } else if (rdl2->freg < rdl2->f->nreg - rdl2->vreg) {
                        rdl2->freg += rdl2->vreg - 1;
                        rdl2->sreg += rdl2->vreg - 1;
                        if (rdl2->freg > rdl2->f->nreg-rdl2->vreg)
                            rdl2->freg = rdl2->f->nreg - rdl2->vreg;
                        if (rdl2->sreg >= rdl2->f->nreg)
                            rdl2->sreg = rdl2->f->nreg - 1;
                        change = move = 1;
                    }
                    res = 1;
                    break;
                case GrKey_Return :
                    if (check_freg(rdl2) != 0) {
                        change = move = 1;
                    }
                    res = 2;
            }
            break;

        case GREV_NULL :
            if (rdl2->vscb) {
                return GUIScrollbarProcessEvent(rdl2->vscb, ev);
            }
            break;

        case GREV_SCBVERT:
            if (rdl2->vscb && GUIScrollbarGetAid(rdl2->vscb) == ev->p2) {
                if (rdl2->freg != ev->p1) {
                    rdl2->freg = ev->p1;
                    change = 1;
                    move = 1;
                }
                res = 1;
            }
            break;
    }

    if (change) {
        if (move) {
            for(j=0; j<rdl2->vreg; j++)
                show_rdl2_item(rdl2, j);
            if (rdl2->vscb) {
                GUIScrollbarSetLimits(rdl2->vscb, 0, rdl2->f->nreg-1,
                                      rdl2->freg, rdl2->freg+rdl2->vreg-1);
                GUIScrollbarPaint(rdl2->vscb, rdl2->dx, rdl2->dy, 0);
            }
        } else {
            show_rdl2_item(rdl2, old);
            show_rdl2_item(rdl2, ind);
        }
        x = rdl2->f->x + rdl2->dx;
        y = rdl2->f->y + rdl2->dy;
        GUIDBCurCtxBltRectToScreen(x, y, x+rdl2->f->width-1, y+rdl2->f->height-1);
    }

    return res;
}
