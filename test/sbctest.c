/**
 * * sbctest.c ---- test subcontext
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

#include <string.h>
#include "test.h"

/***/

static void drawpf( int dx, int dy, GrPattern *pat )
{
    int pt1[4][2] = {{130,200},{140,240},{150,250},{160,180}};
    int pt2[4][2] = {{230,200},{235,240},{246,250},{258,180}};
    int ptaux[4][2];
    int i;
    
    GrPatternFilledBox( 0+dx,0+dy,93+dx,93+dy,pat );
    GrPatternFilledCircle( 139+dx,46+dy,45,pat );
    GrPatternFilledEllipse( 232+dx,46+dy,45,35,pat );
    GrPatternFilledCircleArc( 46+dx,139+dy,45,-300,600,
                              GR_ARC_STYLE_CLOSE2,pat );
    GrPatternFilledEllipseArc( 139+dx,139+dy,45,35,-700,400,
                               GR_ARC_STYLE_CLOSE2,pat );
    GrPatternFilledLine( 188+dx,139+dy,278+dx,139+dy,pat );
    GrPatternFilledPlot( 47+dx,228+dy,pat );
    GrPatternFilledPlot( 47+dx,229+dy,pat );
    GrPatternFilledPlot( 47+dx,230+dy,pat );
    GrPatternFilledPlot( 47+dx,231+dy,pat );
    GrPatternFilledPlot( 47+dx,232+dy,pat );
    for( i=0; i<4; i++ ) {
        ptaux[i][0] = pt1[i][0] + dx;
        ptaux[i][1] = pt1[i][1] + dy;
    }
    GrPatternFilledPolygon( 4,ptaux,pat );
    for( i=0; i<4; i++ ) {
        ptaux[i][0] = pt2[i][0] + dx;
        ptaux[i][1] = pt2[i][1] + dy;
    }
    GrPatternFilledConvexPolygon( 4,ptaux,pat );
}

/***/

static void drawp( int dx, int dy, GrLinePattern *grlp )
{
    int pt1[4][2] = {{130,200},{140,240},{150,250},{160,180}};
    int pt2[4][2] = {{230,200},{235,240},{246,250},{258,180}};
    int ptaux[4][2];
    int i;
    
    GrPatternedBox( 0+dx,0+dy,93+dx,93+dy,grlp );
    GrPatternedCircle( 139+dx,46+dy,45,grlp );
    GrPatternedEllipse( 232+dx,46+dy,45,35,grlp );
    GrPatternedCircleArc( 46+dx,139+dy,45,-300,600,
                          GR_ARC_STYLE_CLOSE2,grlp );
    GrPatternedEllipseArc( 139+dx,139+dy,45,35,-700,400,
                           GR_ARC_STYLE_CLOSE2,grlp );
    GrPatternedLine( 188+dx,139+dy,278+dx,139+dy,grlp );
    for( i=0; i<4; i++ ) {
        ptaux[i][0] = pt1[i][0] + dx;
        ptaux[i][1] = pt1[i][1] + dy;
    }
    GrPatternedPolygon( 4,ptaux,grlp );
    for( i=0; i<4; i++ ) {
        ptaux[i][0] = pt2[i][0] + dx;
        ptaux[i][1] = pt2[i][1] + dy;
    }
    GrPatternedPolyLine( 4,ptaux,grlp );
}


TESTFUNC(sbctest)
{
    char bits[] = {0, 76, 50, 0, 0, 76, 60, 0};
    GrContext *grc, *grsc;
    GrPattern *pat1, *pat2;
    GrLineOption grl;
    GrLinePattern grlp;
    GrFont *grf;
    GrTextOption grt;
    GrEvent ev;
    
    grc = GrCreateContext( 300,320,NULL,NULL );
    if( grc == NULL ) return;
    grsc = GrCreateSubContext( 10,30,290,310,grc,NULL );
    if( grsc == NULL ) return;
    pat1 = GrBuildPixmapFromBits( bits,8,8,GrWhite(),GrBlack() );
    if( pat1 == NULL ) return;
    pat2 = GrBuildPixmapFromBits( bits,8,8,GrBlack(),GrWhite() );
    if( pat2 == NULL ) return;
    grf = GrLoadFont( "lucb40b.fnt" );
    if( grf == NULL ){
        grf = GrLoadFont( "../fonts/lucb40b.fnt" );
        if( grf == NULL ) return;
    }
    
    GrBox( 19,39,320,360,GrWhite() );
    
    GrTextXY( 0,0,"White drawing on context       ",GrWhite(),GrBlack() );
    GrSetContext( grc );
    GrClearContext( GrBlack() );
    drawing( 10,30,280,280,GrWhite(),GrNOCOLOR );
    GrSetContext( NULL );
    GrBitBlt( NULL,20,40,grc,0,0,299,319,GrWRITE );
    GrEventWaitKeyOrClick(&ev);
    
    GrTextXY( 0,0,"Black drawing on subcontext    ",GrWhite(),GrBlack() );
    GrSetContext( grsc );
    drawing( 0,0,280,280,GrBlack(),GrNOCOLOR );
    GrSetContext( NULL );
    GrBitBlt( NULL,20,40,grc,0,0,299,319,GrWRITE );
    GrEventWaitKeyOrClick(&ev);
    
    GrTextXY( 0,0,"Pattern drawing on context     ",GrWhite(),GrBlack() );
    GrSetContext( grc );
    GrClearContext( GrBlack() );
    drawpf( 10,30,pat1 );
    GrSetContext( NULL );
    GrBitBlt( NULL,20,40,grc,0,0,299,319,GrWRITE );
    GrEventWaitKeyOrClick(&ev);
    
    GrTextXY( 0,0,"Pattern drawing on subcontext  ",GrWhite(),GrBlack() );
    GrSetContext( grsc );
    GrClearContext( GrBlack() );
    drawpf( 0,0,pat2 );
    GrSetContext( NULL );
    GrBitBlt( NULL,20,40,grc,0,0,299,319,GrXOR );
    GrEventWaitKeyOrClick(&ev);
    
    grl.lno_color = GrWhite();
    grl.lno_width = 3;
    grl.lno_pattlen = 0;
    grlp.lnp_pattern = pat1;
    grlp.lnp_option = &grl;
    
    GrTextXY( 0,0,"Patterned drawing on context   ",GrWhite(),GrBlack() );
    GrSetContext( grc );
    GrClearContext( GrBlack() );
    grlp.lnp_pattern = pat1;
    drawp( 10,30,&grlp );
    GrSetContext( NULL );
    GrBitBlt( NULL,20,40,grc,0,0,299,319,GrWRITE );
    GrEventWaitKeyOrClick(&ev);
    
    GrTextXY( 0,0,"Patterned drawing on subcontext",GrWhite(),GrBlack() );
    GrSetContext( grsc );
    GrClearContext( GrBlack() );
    grlp.lnp_pattern = pat2;
    drawp( 0,0,&grlp );
    GrSetContext( NULL );
    GrBitBlt( NULL,20,40,grc,0,0,299,319,GrXOR );
    GrEventWaitKeyOrClick(&ev);
    
    grt.txo_fgcolor = GrWhite();
    grt.txo_bgcolor = GrNOCOLOR;
    grt.txo_font = grf;
    grt.txo_direct = GR_TEXT_RIGHT;
    grt.txo_xalign = GR_ALIGN_LEFT;
    grt.txo_yalign = GR_ALIGN_CENTER;
    grt.txo_chrtype = GR_BYTE_TEXT;
    
    GrTextXY( 0,0,"Patterned text on context      ",GrWhite(),GrBlack() );
    GrSetContext( grc );
    GrClearContext( GrBlack() );
    GrPatternDrawString( "Hello all",9,20,60,&grt,pat1 );
    GrPatternDrawChar( 'G',20,120,&grt,pat1 );
    GrPatternDrawStringExt( "Hola a todos",12,20,180,&grt,pat1 );
    GrPatternDrawCharExt( 'M',20,240,&grt,pat1 );
    GrSetContext( NULL );
    GrBitBlt( NULL,20,40,grc,0,0,299,319,GrWRITE );
    GrEventWaitKeyOrClick(&ev);
    
    GrTextXY( 0,0,"Patterned text on subcontext   ",GrWhite(),GrBlack() );
    GrSetContext( grsc );
    GrClearContext( GrBlack() );
    GrPatternDrawString( "Hello all",9,10,30,&grt,pat2 );
    GrPatternDrawChar( 'G',10,90,&grt,pat2 );
    GrPatternDrawStringExt( "Hola a todos",12,10,150,&grt,pat2 );
    GrPatternDrawCharExt( 'M',10,210,&grt,pat2 );
    GrSetContext( NULL );
    GrBitBlt( NULL,20,40,grc,0,0,299,319,GrXOR );
    GrEventWaitKeyOrClick(&ev);
    
    GrUnloadFont( grf );
    GrDestroyPattern( pat2 );
    GrDestroyPattern( pat1 );
    GrDestroyContext( grsc );
    GrDestroyContext( grc );
}
