/**
 ** fontdemo.c ---- demonstrate a font
 **
 ** Copyright (C) 2002 Dimitar Zhekov
 ** E-Mail: jimmy@is-vn.bg
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

static GrTextOption opt;
static int curx = 0, cury = 0;
/* deltax and deltay are the additional columns/lines between characters */
static int deltax = 0, deltay = 0;
/* border margins */
static int marginx = 2, marginy = 2;

static int tcursor_status = -1;
static GrColor tcursor_color;

static void ini_tcursor(void);
static void draw_tcursor(GrColor fg);
static void show_tcursor(void);
static void hide_tcursor(void);
static void gnewl(void);
static void gputc(long c);
static void gputs(char *s);
static void revert(void);
static void info1(void);
static void info2(void);

static void ini_tcursor(void)
{
    tcursor_status = -1;
    tcursor_color = GrAllocColor(255, 255, 0);
    show_tcursor();
}

static void draw_tcursor(GrColor fg)
{
    GrColor aux;

    aux = opt.txo_fgcolor;
    opt.txo_fgcolor = fg;
    GrDrawChar('_', curx, cury, &opt);
    opt.txo_fgcolor = aux;
}

static void show_tcursor(void)
{
    tcursor_status++;
    
    if (tcursor_status == 0) {
      draw_tcursor(tcursor_color);
    }
}

static void hide_tcursor(void)
{
    tcursor_status--;
    
    if (tcursor_status == -1) {
      draw_tcursor(opt.txo_bgcolor);
    }
}

static void gnewl(void)
{
    int linehigh = GrCharHeight('A', &opt) + deltay;

    hide_tcursor();
    cury += linehigh;
    curx = 0;
    if(cury + linehigh > GrSizeY()) {
        GrBitBlt(NULL, 0, 0, NULL, 0, linehigh, GrMaxX(), GrMaxY(), GrWRITE);
        GrFilledBox(0, GrMaxY()-linehigh, GrMaxX(), GrMaxY(), opt.txo_bgcolor);
        cury -= linehigh;
    }
    show_tcursor();
}

/* all control characters are displayed 1:1 */
static void gputc(long c)
{
    hide_tcursor();
    if(curx + GrCharWidth(c, &opt) + deltax > GrSizeX()) gnewl();
    GrDrawChar(c, curx, cury, &opt);
    curx += GrCharWidth(c, &opt) + deltax;
    show_tcursor();
}

static void gputs(char *s)
{
    unsigned short *buf;
    int ulen, i;
    
    hide_tcursor();
    if (opt.txo_chrtype == GR_UTF8_TEXT) {
        opt.txo_chrtype = GR_UCS2_TEXT;
        buf = GrUTF8StrToUCS2Str((unsigned char *)s, &ulen, 0);
        for (i=0; i<ulen; i++) gputc(buf[i]);
        free(buf);
        opt.txo_chrtype = GR_UTF8_TEXT;
    } else {
        while(*s != '\0') gputc((unsigned char) *s++);
    }
    gnewl();
    show_tcursor();
}

static void revert(void)
{
    GrColor color1, color2, under;

    under = opt.txo_fgcolor & GR_UNDERLINE_TEXT;
    color1 = opt.txo_fgcolor & GrCVALUEMASK;
    color2 = opt.txo_bgcolor & GrCVALUEMASK;
    opt.txo_fgcolor = color2 | under;
    opt.txo_bgcolor = color1;

    GrFloodSpillC2(NULL, 0, 0, GrScreenX()-1, GrScreenY()-1,
                   color1, color2, color2, color1);
//    GrFloodSpill2(0, 0, GrMaxX(), GrMaxY(),
//                   color1, color2, color2, color1);
}

static void info1(void)
{
    char buffer[210];
    GrFontHeader *hdr;
    int c;

    hdr = &opt.txo_font->h;
    sprintf(buffer, "Font name: %s %dx%d", hdr->name, GrCharWidth('A', &opt),
            GrCharHeight('A', &opt));
    if (hdr->proportional)
        strcat(buffer, " proportional");
    else
        strcat(buffer, " fixed");
    gputs(buffer);
    sprintf(buffer, "Font family: %s  Encoding: %d (%s)",
            hdr->family, hdr->encoding, GrStrFontEncoding(hdr->encoding));
    gputs(buffer);
    sprintf(buffer, "Min char, num chars: %d, %d", hdr->minchar,hdr->numchars);
    gputs(buffer);
    sprintf(buffer, "Screen res: %dx%d %d bpp",
            GrScreenX(), GrScreenY(), GrCurrentVideoMode()->bpp);
    gputs(buffer);
    gnewl();

    gputs("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG ÑOÑO ÁCÉNTÓS");
    gputs("the quick brown fox jumps over the lazy dog ñoño ácéntós");
    gnewl();

    /* ascii table, or to be precise, a full table of the current font */
    opt.txo_chrtype = GR_WORD_TEXT;
    for(c = 0; c < hdr->numchars; c++) {
        gputc(hdr->minchar + c);
        if(c % 0x20 == 0x1F) gnewl();
    }
    gnewl();
    if(c % 0x20 != 0) gnewl();
    opt.txo_chrtype = GrGetChrtypeForUserEncoding();
}

static void info2(void)
{
    gputs("F5-toggle reverse, F6-toggle underline, F7-restart, F10-exit");
    gputs("Enter text:");
    gnewl();
}

int main(int argc, char **argv)
{
    int i, n;
    char *s;
    char *bad = NULL;
    int c;

    int width = 0, height = 0, bpp = 8, gray = 192, attributes = 0;

    char *name, *testname;
    GrFontHeader *hdr;
    FILE *f;
    GrEvent ev;
    long key;
    GrContext *grc;

    /* unfortunately not all systems support getopt() */
    for(i = 1; i < argc; i++) {
        s = argv[i];
        if(*s != '-' || ((c = *++s) == '\0') || *++s != '\0') break;
        if(c == '-') {
            i++;
            break;
        }
        if(++i == argc) {
            fprintf(stderr, "-%c: argument required\n", c);
            return(1);
        }
        if(sscanf(argv[i], "%d", &n) != 1 || n < 0) {
            fprintf(stderr, "%s: invalid argument\n", argv[i]);
            exit(1);
        }
        switch(c) {
            case 'x' : width = n; break;
            case 'y' : height = n; break;
            case 'b' : if((bpp = n) < 2 || bpp > 32) bad = "bpp"; break;
            case 'g' : if((gray = n) > 255) bad = "gray"; break;
            case 'X' : if((deltax = n) > 31) bad = "deltax"; break;
            case 'Y' : if((deltay = n) > 31) bad = "deltay"; break;
            case 'a' : if((attributes = n) > 3) bad = "attributes"; break;
            default  : {
                fprintf(stderr, "-%c: invalid option\n", c);
                return(1);
            }
        }
        if(bad) {
            fprintf(stderr, "%d: %s out of range\n", n, bad);
            return(1);
        }
    }

    if(i == argc) {
        printf("usage:\tfontdemo [-x WIDTH] [-y HEIGHT] [-b BPP] [-g COMPONENT]\n"
               "\t[-X DELTAX] [-Y DELTAY] [-a ATTRIBUTES] FONT [FILE...]\n");
        return(0);
    }

    name = argv[i++];
    opt.txo_font = GrLoadFont(name);
    if(opt.txo_font == NULL && (testname = malloc(strlen(name) + 10)) != NULL) {
        /* try again, this is a test and the path can not been set yet */
#if defined(__MSDOS__) || defined(__WIN32__)
        sprintf( testname,"..\\fonts\\%s",name );
#else
        sprintf( testname,"../fonts/%s",name );
#endif
        opt.txo_font = GrLoadFont(testname);
        free(testname);
    }
    if(opt.txo_font == NULL) {
        fprintf(stderr, "%s: load error\n", name);
        return(1);
    }
    hdr = &opt.txo_font->h;

    if(height == 0) {
        if(width == 0) {
            if (hdr->height <= 14) height = 400;
            else if (hdr->height <= 16) height = 480;
            else if (hdr->height <= 20) height = 600;
            else if (hdr->height <= 24) height = 768;
            else height = 1024;
        }
        else height = width * 3 / 4;
    }
    if(width == 0) width = (height == 400) ? 640 : height * 4 / 3;

    GrSetMode(GR_width_height_bpp_graphics, width, height, bpp);
    GrSetUserEncoding(GRENC_UTF_8);
    grc = GrCreateSubContext(marginx, marginy, GrMaxX()-marginx,
                             GrMaxY()-marginy, NULL, NULL);
    GrSetContext(grc);
    
    if(!gray || (opt.txo_fgcolor = GrAllocColor(gray, gray, gray)) == GrNOCOLOR)
        opt.txo_fgcolor = GrWhite();
    if(attributes & 0x02)
        opt.txo_fgcolor |= GR_UNDERLINE_TEXT;
    opt.txo_bgcolor = GrBlack();
    if(attributes & 0x01)
        revert();
    opt.txo_chrtype = GrGetChrtypeForUserEncoding();
    opt.txo_direct = GR_TEXT_RIGHT;
    opt.txo_xalign = GR_ALIGN_LEFT;
    opt.txo_yalign = GR_ALIGN_TOP;
    ini_tcursor();

    hide_tcursor();
    
    GrClearScreen(opt.txo_bgcolor);
    info1();
    
    while(i < argc) {
        name = argv[i++];
        if((f = fopen(name, "r")) == NULL) {
            perror(name);
            return(1);
        }
        while((c = getc(f)) != EOF) if(c != '\n') gputc(c); else gnewl();
        if(ferror(f) != 0 || fclose(f) != 0) {
            perror(name);
            return(1);
        }
    }

    info2();

    show_tcursor();
    
    GrEventInit();
    while(1) {
        GrEventWait(&ev);
        if(ev.type == GREV_KEY) {
            key = ev.p1;
            if (ev.p2 == GRKEY_KEYCODE) {
                if( key == GrKey_F10)
                    break;
                if(key == GrKey_F1)
                    gnewl();
                else if(key == GrKey_F5)
                    revert();
                else if(key == GrKey_F6)
                    opt.txo_fgcolor ^= GR_UNDERLINE_TEXT;
                else if(key == GrKey_F7) {
                    GrClearScreen(opt.txo_bgcolor);
                    curx = cury = 0;
                    ini_tcursor();
                    hide_tcursor();
                    info1(); info2();
                    show_tcursor();
                }
            }
            else if (key == '\r')
                gnewl();
            else
                gputc(key);
        }
    }
    GrEventUnInit();

    GrUnloadFont(opt.txo_font);

    return(0);
}
