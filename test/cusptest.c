/**
 ** cusptest.c ---- test custom polygon
 **
 ** Copyright (C) 2019 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
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
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mgrx.h"
#include "mgrxkeys.h"

/* default mode */

static int gwidth = 1024;
static int gheight = 768;
static int gbpp = 24;

static int po_star[10][2] = {{150,25},{179,111},{269,111},{197,165},{223,251},
                             {150,200},{77,251},{103,165},{31,111},{121,111}};

static int po_starnc[5][2] = {{150,25},{223,251},{31,111},{269,111},{77,251}};

static int po_box[4][2] = {{50,301},{150,301},{150,100},{50,100}};

static int po_pen[5][2] = {{150,25},{269,111},{223,251},{77,251},{31,111}};

static int po_ele[6][2] = {{50,301},{200,301},{200,200},{150,200},{150,100},{50,100}};

static char pat_bits[] = {0, 76, 50, 0, 0, 76, 60, 0};

static int po_aux[10][2];
  
void translate(int x, int y, int np, int po_src[][2], int po_des[][2])
{
    int i;
    
    for (i=0; i<np; i++) {
        po_des[i][0] = po_src[i][0] + x;
        po_des[i][1] = po_src[i][1] + y;
    }
}

void test_custom(GrLineOption *lopt, GrColor cline)
{
    GrCustomBox(50, 301, 150, 100, lopt);
    GrBox(50, 301, 150, 100, cline);
  
    translate(0, 300, 4, po_box, po_aux);
    GrCustomPolygon(4, po_aux, lopt);
    GrPolygon(4, po_aux, cline);

    translate(180, 50, 10, po_star, po_aux);
    GrCustomPolygon(10, po_aux, lopt);
    GrPolygon(10, po_aux, cline);

    translate(180, 350, 10, po_star, po_aux);
    GrCustomPolyLine(10, po_aux, lopt);
    GrPolyLine(10, po_aux, cline);

    GrCustomCircle(600, 200, 100, lopt);
    GrCircle(600, 200, 100, cline);
  
    translate(500, 300, 6, po_ele, po_aux);
    GrCustomPolygon(6, po_aux, lopt);
    GrPolygon(6, po_aux, cline);

    translate(720, 50, 5, po_pen, po_aux);
    GrCustomPolygon(5, po_aux, lopt);
    GrPolygon(5, po_aux, cline);

    translate(720, 350, 5, po_starnc, po_aux);
    GrCustomPolygon(5, po_aux, lopt);
    GrPolygon(5, po_aux, cline);
}

void test_patterned(GrLinePattern *grlp, GrColor cline)
{
    GrPatternedBox(50, 301, 150, 100, grlp);
  
    translate(0, 300, 4, po_box, po_aux);
    GrPatternedPolygon(4, po_aux, grlp);

    translate(180, 50, 10, po_star, po_aux);
    GrPatternedPolygon(10, po_aux, grlp);

    translate(180, 350, 10, po_star, po_aux);
    GrPatternedPolyLine(10, po_aux, grlp);

    GrPatternedCircle(600, 200, 100, grlp);
  
    translate(500, 300, 6, po_ele, po_aux);
    GrPatternedPolygon(6, po_aux, grlp);

    translate(720, 50, 5, po_pen, po_aux);
    GrPatternedPolygon(5, po_aux, grlp);

    translate(720, 350, 5, po_starnc, po_aux);
    GrPatternedPolygon(5, po_aux, grlp);
}

int main(int argc,char **argv)
{
    GrLineOption lopt;
    GrLinePattern grlp;
    GrPattern *pat;
    GrEvent ev;
    GrColor color;

    if (argc >= 4) {
        gwidth = atoi(argv[1]);
        gheight = atoi(argv[2]);
        gbpp = atoi(argv[3]);
    }

    GrSetMode(GR_width_height_bpp_graphics, gwidth, gheight, gbpp);

    GrEventInit();
    GrMouseDisplayCursor();
    GrSetDefaultFont(&GrFont_PC8x16);

    lopt.lno_color = GrWhite();
    lopt.lno_width = 20;
    lopt.lno_pattlen = 0;
    lopt.lno_dashpat = NULL;

    color = GrAllocColor(255,0,0);
    
    GrClearScreen(GrBlack());
    GrTextXY(10, 10, "Solid wide line", GrWhite(), GrBlack() );
    test_custom(&lopt, color);
    GrEventWaitKeyOrClick(&ev);

    lopt.lno_pattlen = 2;
    lopt.lno_dashpat = (unsigned char *)"\x06\x04";
    
    GrClearScreen(GrBlack());
    GrTextXY(10, 10, "Dashed wide line", GrWhite(), GrBlack() );
    test_custom(&lopt, color);
    GrEventWaitKeyOrClick(&ev);

    pat = GrBuildPixmapFromBits(pat_bits, 8, 8, GrWhite(), GrBlack());
    if (pat == NULL) return 1;
    lopt.lno_pattlen = 0;
    lopt.lno_dashpat = NULL;
    grlp.lnp_pattern = pat;
    grlp.lnp_option = &lopt;

    GrClearScreen(GrBlack());
    GrTextXY(10,10, "Patterned wide line", GrWhite(), GrBlack() );
    test_patterned(&grlp, color);
    GrEventWaitKeyOrClick(&ev);

    lopt.lno_pattlen = 2;
    lopt.lno_dashpat = (unsigned char *)"\x06\x04";

    GrClearScreen(GrBlack());
    GrTextXY(10, 10, "Patterned and dashed wide line", GrWhite(), GrBlack() );
    test_patterned(&grlp, color);
    GrEventWaitKeyOrClick(&ev);

    GrEventUnInit();
    GrSetMode(GR_default_text);
    return 0;
}
