/**
 ** SPEEDTST.C ---- check all available frame drivers speed
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <values.h>
#include <math.h>
#include <time.h>

#include "grx20.h"

#define READPIX_loops  (256*1)
#define DRAWPIX_loops  (128*1)
#define DRAWLIN_loops  (8*1)
#define DRAWHLIN_loops (16*1)
#define DRAWVLIN_loops (8*1)
#define DRAWBLK_loops  (1*1)

typedef struct {
    GrFrameMode fm;
    int    w,h,bpp;
    int    measured;
    double readpix;
    double drawpix;
    double drawlin;
    double drawhlin;
    double drawvlin;
    double drawblk;
    double rp_count;
    double dp_count;
    double dl_count;
    double dh_count;
    double dv_count;
    double db_count;
} gvmode;

gvmode grmodes[200];

#define PAIRS 4096

#define UL(x)  ((unsigned long)(x))
#define DBL(x)  ((double)(x))
#define INT(x) ((int)(x))

#ifndef  CLK_TCK
#define  CLK_TCK    CLOCKS_PER_SEC
#endif

#ifndef min
#define min(a,b) ((a)<(b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a)>(b) ? (a) : (b))
#endif

typedef struct XYpairs {
  int x[PAIRS];
  int y[PAIRS];
  int w, h;
  struct XYpairs *nxt;
} XY_PAIRS;

XY_PAIRS *xyp = NULL;
int measured_any = 0;

#ifdef __GNUC__
#  define Random() random()
#  define SRandom(s) srandom(s)
#else
#  define Random()   random(MAXINT)
#  define SRandom(s) srand(s)
#endif

XY_PAIRS *checkpairs(int w, int h) {
  XY_PAIRS *res = xyp;
  int i;

  while (res != NULL) {
    if (res->w == w && res->h == h)
      return res;
    res = res->nxt;
  }

  SRandom(12345);

  res = malloc(sizeof(XY_PAIRS));
  assert(res != NULL);
  res->w = w--;
  res->h = h--;
  res->nxt = xyp;
  xyp = res;
  for (i=0; i < PAIRS; ++i) {
    int x = DBL(w) * Random() / DBL(MAXINT);
    int y = DBL(h) * Random() / DBL(MAXINT);
    if (x < 0) x = 0; else
    if (x > w) x = w;
    if (y < 0) y = 0; else
    if (y > h) y = h;
    res->x[i] = x;
    res->y[i] = y;
  }
  return res;
}

double SQR(int a, int b) {
  double r = DBL(a-b);
  return r*r;
}

double ABS(int a, int b) {
  double r = DBL(a-b);
  return fabs(r);
}

char *FrameDriverName(GrFrameMode m) {

  switch(m) {
    case GR_frameUndef: return "Undef";
    case GR_frameText : return "Text";
    case GR_frameHERC1: return "HERC1";
    case GR_frameEGAVGA1: return "EGAVGA1";
    case GR_frameEGA4: return "EGA4";
    case GR_frameSVGA4: return "SVGA4";
    case GR_frameSVGA8: return "SVGA8";
    case GR_frameVGA8X: return "VGA8X";
    case GR_frameSVGA16: return "SVGA16";
    case GR_frameSVGA24: return "SVGA24";
    case GR_frameSVGA32L: return "SVGA32L";
    case GR_frameSVGA32H: return "SVGA32H";
    case GR_frameSVGA8_LFB: return "LFB8";
    case GR_frameSVGA16_LFB: return "LFB16";
    case GR_frameSVGA24_LFB: return "LFB24";
    case GR_frameSVGA32L_LFB: return "LFB32L";
    case GR_frameSVGA32H_LFB: return "LFB32H";
    case GR_frameRAM1: return "RAM1";
    case GR_frameRAM4: return "RAM4";
    case GR_frameRAM8: return "RAM8";
    case GR_frameRAM16: return "RAM16";
    case GR_frameRAM24: return "RAM24";
    case GR_frameRAM32L: return "RAM32L";
    case GR_frameRAM32H: return "RAM32H";
    case GR_frameRAM3x8: return "RAM3x8";
  }
  return "UNKNOWN";
}

void printresultheader(FILE *f) {
  fprintf(f, "Driver               readpx  drawp   line    hline   vline   block\n");
}

void printresultline(FILE *f, gvmode * gp) {
  fprintf(f, "%-9s %4dx%4d", FrameDriverName(gp->fm), gp->w, gp->h);
  fprintf(f, "  %2.4f", gp->readpix / (1024.0 * 1024.0));
  fprintf(f, "  %2.4f", gp->drawpix / (1024.0 * 1024.0));
  fprintf(f, "  %2.4f", gp->drawlin / (1024.0 * 1024.0));
  fprintf(f, "  %2.4f", gp->drawhlin / (1024.0 * 1024.0));
  fprintf(f, "  %2.4f", gp->drawvlin / (1024.0 * 1024.0));
  fprintf(f, "  %2.4f", gp->drawblk / (1024.0 * 1024.0));
  fprintf(f, "\n");
}

void readpixeltest(gvmode *gp, XY_PAIRS *pairs) {
  int i, j;
  long t1,t2;
  double seconds;
  int *x = pairs->x;
  int *y = pairs->y;

  if (!gp->measured) {
    gp->readpix  = 0.0;
    gp->rp_count = DBL(PAIRS) * DBL(READPIX_loops);
  }

  t1 = clock();
  for (i=0; i < READPIX_loops; ++i) {
    for (j=PAIRS-1; j >= 0; j--)
       GrPixelNC(x[j],y[j]);
  }
  t2 = clock();
  seconds = DBL(t2 - t1) / DBL(CLK_TCK);
  if (seconds > 0)
    gp->readpix = gp->rp_count / seconds;
}

void drawpixeltest(gvmode *gp, XY_PAIRS *pairs) {
  int i, j;
  long c1 = GrWhite();
  long c2 = GrWhite() | GrXOR;
  long c3 = GrWhite() | GrOR;
  long c4 = GrBlack() | GrAND;
  long t1,t2;
  double seconds;
  int *x = pairs->x;
  int *y = pairs->y;

  if (!gp->measured) {
    gp->drawpix  = 0.0;
    gp->dp_count = DBL(PAIRS) * DBL(DRAWPIX_loops) * 4.0;
  }

  t1 = clock();
  for (i=0; i < DRAWPIX_loops; ++i) {
    for (j=PAIRS-1; j >= 0; j--) GrPlotNC(x[j],y[j],c1);
    for (j=PAIRS-1; j >= 0; j--) GrPlotNC(x[j],y[j],c2);
    for (j=PAIRS-1; j >= 0; j--) GrPlotNC(x[j],y[j],c3);
    for (j=PAIRS-1; j >= 0; j--) GrPlotNC(x[j],y[j],c4);
  }
  t2 = clock();
  seconds = DBL(t2 - t1) / DBL(CLK_TCK);
  if (seconds > 0)
    gp->drawpix = gp->dp_count / seconds;
}

void drawlinetest(gvmode *gp, XY_PAIRS *pairs) {
  int i, j;
  int *x = pairs->x;
  int *y = pairs->y;
  long c1 = GrWhite();
  long c2 = GrWhite() | GrXOR;
  long c3 = GrWhite() | GrOR;
  long c4 = GrBlack() | GrAND;
  long t1,t2;
  double seconds;

  if (!gp->measured) {
    gp->drawlin  = 0.0;
    gp->dl_count = 0.0;
    for (j=0; j < PAIRS; j+=2)
      gp->dl_count += sqrt(SQR(x[j],x[j+1])+SQR(y[j],y[j+1]));
    gp->dl_count *= 4.0;
  }

  t1 = clock();
  for (i=0; i < DRAWLIN_loops; ++i) {
    for (j=PAIRS-2; j >= 0; j-=2)
        GrLineNC(x[j],y[j],x[j+1],y[j+1],c1);
    for (j=PAIRS-2; j >= 0; j-=2)
        GrLineNC(x[j],y[j],x[j+1],y[j+1],c2);
    for (j=PAIRS-2; j >= 0; j-=2)
        GrLineNC(x[j],y[j],x[j+1],y[j+1],c3);
    for (j=PAIRS-2; j >= 0; j-=2)
        GrLineNC(x[j],y[j],x[j+1],y[j+1],c4);
  }
  t2 = clock();
  seconds = DBL(t2 - t1) / DBL(CLK_TCK);
  if (seconds > 0)
    gp->drawlin = gp->dl_count / seconds;
}

void drawhlinetest(gvmode *gp, XY_PAIRS *pairs) {
  int  i, j;
  int *x = pairs->x;
  int *y = pairs->y;
  long c1 = GrWhite();
  long c2 = GrWhite() | GrXOR;
  long c3 = GrWhite() | GrOR;
  long c4 = GrBlack() | GrAND;
  long t1,t2;
  double seconds;

  if (!gp->measured) {
    gp->drawhlin = 0.0;
    gp->dh_count = 0.0;
    for (j=0; j < PAIRS; j+=2)
      gp->dh_count += ABS(x[j],x[j+1]);
    gp->dh_count *= 4.0;
  }

  t1 = clock();
  for (i=0; i < DRAWHLIN_loops; ++i) {
    for (j=PAIRS-2; j >= 0; j-=2)
      GrHLineNC(x[j],x[j+1],y[j],c1);
    for (j=PAIRS-2; j >= 0; j-=2)
      GrHLineNC(x[j],x[j+1],y[j],c2);
    for (j=PAIRS-2; j >= 0; j-=2)
      GrHLineNC(x[j],x[j+1],y[j],c3);
    for (j=PAIRS-2; j >= 0; j-=2)
      GrHLineNC(x[j],x[j+1],y[j],c4);
  }
  t2 = clock();
  seconds = DBL(t2 - t1) / DBL(CLK_TCK);
  if (seconds > 0)
    gp->drawhlin = gp->dh_count / seconds;
}

void drawvlinetest(gvmode *gp, XY_PAIRS *pairs) {
  int i, j;
  int *x = pairs->x;
  int *y = pairs->y;
  long c1 = GrWhite();
  long c2 = GrWhite() | GrXOR;
  long c3 = GrWhite() | GrOR;
  long c4 = GrBlack() | GrAND;
  long t1,t2;
  double seconds;

  if (!gp->measured) {
    gp->drawvlin = 0.0;
    gp->dv_count = 0.0;
    for (j=0; j < PAIRS; j+=2)
      gp->dv_count += ABS(y[j],y[j+1]);
    gp->dv_count *= 4.0;
  }

  t1 = clock();
  for (i=0; i < DRAWVLIN_loops; ++i) {
    for (j=PAIRS-2; j >= 0; j-=2)
       GrVLineNC(x[j],y[j],y[j+1],c1);
    for (j=PAIRS-2; j >= 0; j-=2)
       GrVLineNC(x[j],y[j],y[j+1],c2);
    for (j=PAIRS-2; j >= 0; j-=2)
       GrVLineNC(x[j],y[j],y[j+1],c3);
    for (j=PAIRS-2; j >= 0; j-=2)
       GrVLineNC(x[j],y[j],y[j+1],c4);
  }
  t2 = clock();
  seconds = DBL(t2 - t1) / DBL(CLK_TCK);
  if (seconds > 0)
    gp->drawvlin = gp->dv_count / seconds;
}

void drawblocktest(gvmode *gp, XY_PAIRS *pairs) {
  int i, j;
  long c1 = GrWhite();
  long c2 = GrWhite() | GrXOR;
  long c3 = GrWhite() | GrOR;
  long c4 = GrBlack() | GrAND;
  int xb[PAIRS];
  int yb[PAIRS];
  long t1,t2;
  double seconds;

  for (j=0; j < PAIRS; j+=2) {
    xb[j]   = min(pairs->x[j],pairs->x[j+1]);
    xb[j+1] = max(pairs->x[j],pairs->x[j+1]);
    yb[j]   = min(pairs->y[j],pairs->y[j+1]);
    yb[j+1] = max(pairs->y[j],pairs->y[j+1]);
  }

  if (!gp->measured) {
    gp->drawblk = 0.0;
    gp->db_count = 0.0;
    for (j=0; j < PAIRS; j+=2)
      gp->db_count += ABS(xb[j],xb[j+1]) * ABS(yb[j],yb[j+1]);
    gp->db_count *= 4.0;
  }

  t1 = clock();
  for (i=0; i < DRAWBLK_loops; ++i) {
    for (j=PAIRS-2; j >= 0; j-=2)
      GrFilledBoxNC(xb[j],yb[j],xb[j+1],yb[j+1],c1);
    for (j=PAIRS-2; j >= 0; j-=2)
      GrFilledBoxNC(xb[j],yb[j],xb[j+1],yb[j+1],c2);
    for (j=PAIRS-2; j >= 0; j-=2)
      GrFilledBoxNC(xb[j],yb[j],xb[j+1],yb[j+1],c3);
    for (j=PAIRS-2; j >= 0; j-=2)
      GrFilledBoxNC(xb[j],yb[j],xb[j+1],yb[j+1],c4);
  }
  t2 = clock();
  seconds = DBL(t2 - t1) / DBL(CLK_TCK);
  if (seconds > 0)
    gp->drawblk = gp->db_count / seconds;
}

void speedcheck(gvmode *gp) {
  XY_PAIRS *pairs;
  char m[40];
  static int first = 1;

  if (gp->measured) return;
  if (first) {
    printf(
      "speedtest may take some time to process.\n"
      "Now press <CR> to continue..."
    );
    first = 0;
    fflush(stdout);
    gets(m);
  }

  GrSetMode(
      GR_width_height_color_graphics,
      gp->w, gp->h, 1UL<<gp->bpp
  );

  if ( GrScreenFrameMode() != gp->fm) {
  GrFrameMode act = GrScreenFrameMode();
    GrSetMode(GR_default_text);
    printf("Setup failed : %s != %s\n",
    FrameDriverName(act),
    FrameDriverName(gp->fm));
    getch();
    return;
  }

  pairs = checkpairs(gp->w, gp->h);
  GrFilledBox( 0, 0, gp->w-1, gp->h-1, GrBlack());
  readpixeltest(gp,pairs);
  GrFilledBox( 0, 0, gp->w-1, gp->h-1, GrBlack());
  drawpixeltest(gp,pairs);
  GrFilledBox( 0, 0, gp->w-1, gp->h-1, GrBlack());
  drawlinetest(gp,pairs);
  GrFilledBox( 0, 0, gp->w-1, gp->h-1, GrBlack());
  drawhlinetest(gp,pairs);
  GrFilledBox( 0, 0, gp->w-1, gp->h-1, GrBlack());
  drawvlinetest(gp,pairs);
  GrFilledBox( 0, 0, gp->w-1, gp->h-1, GrBlack());
  drawblocktest(gp,pairs);
  GrFilledBox( 0, 0, gp->w-1, gp->h-1, GrBlack());
  gp->measured = 1;
  measured_any = 1;
  GrSetMode(GR_default_text);
  printf("Results: \n");
  printresultheader(stdout);
  printresultline(stdout, gp);
  kbhit();    /* this is here to flush in the X version 8-) */
  gets(m);
}

gvmode *collectmodes(const GrVideoDriver *drv,gvmode *gp)
{
        GrFrameMode fm;
        const GrVideoMode *mp;
        for(fm =GR_firstGraphicsFrameMode;
              fm <= GR_lastGraphicsFrameMode; fm++) {
            for(mp = GrFirstVideoMode(fm); mp; mp = GrNextVideoMode(mp)) {
                gp->fm  = fm;
                gp->w   = mp->width;
                gp->h   = mp->height;
                gp->bpp = mp->bpp;
                gp->measured = 0;
                gp++;
            }
        }
        return(gp);
}

int vmcmp(const void *m1,const void *m2)
{
        gvmode *md1 = (gvmode *)m1;
        gvmode *md2 = (gvmode *)m2;
        if(md1->bpp != md2->bpp) return(md1->bpp - md2->bpp);
        if(md1->w   != md2->w  ) return(md1->w   - md2->w  );
        if(md1->h   != md2->h  ) return(md1->h   - md2->h  );
        return(0);
}

int  nmodes = 0;

#define LINES   20
#define COLUMNS 80

void ModeText(int i, int shrt,char *mdtxt) {
        switch (shrt) {
          case 2 : sprintf(mdtxt,"%2d) %dx%d ", i+1, grmodes[i].w, grmodes[i].h);
                   break;
          case 1 : sprintf(mdtxt,"%2d) %4dx%-4d ", i+1, grmodes[i].w, grmodes[i].h);
                   break;
          default: sprintf(mdtxt,"  %2d)  %4dx%-4d ", i+1, grmodes[i].w, grmodes[i].h);
                   break;
        }
        mdtxt += strlen(mdtxt);

        if (grmodes[i].bpp > 20)
          sprintf(mdtxt, "%ldM", 1L << (grmodes[i].bpp-20));
        else  if (grmodes[i].bpp > 10)
          sprintf(mdtxt, "%ldk", 1L << (grmodes[i].bpp-10));
        else
          sprintf(mdtxt, "%ld", 1L << grmodes[i].bpp);
        switch (shrt) {
          case 2 : break;
          case 1 : strcat(mdtxt, " col"); break;
          default: strcat(mdtxt, " colors"); break;
        }
}

int ColsCheck(int cols, int ml, int sep) {
  int len;

  len = ml * cols + (cols-1) * sep + 1;
  return len <= COLUMNS;
}

void PrintModes(void) {
        char mdtxt[100];
        unsigned int maxlen;
        int i, n, shrt, c, cols;

        cols = (nmodes+LINES-1) / LINES;
        do {
          for (shrt = 0; shrt <= 2; ++shrt) {
            maxlen = 0;
            for (i = 0; i < nmodes; ++i) {
              ModeText(i,shrt,mdtxt);
              if (strlen(mdtxt) > maxlen) maxlen = strlen(mdtxt);
            }
            n = 2;
            if (cols>1 || shrt<2) {
              if (!ColsCheck(cols, maxlen, n)) continue;
              while (ColsCheck(cols, maxlen, n+1) && n < 4) ++n;
            }
            c = 0;
            for (i = 0; i < nmodes; ++i) {
              if (++c == cols) c = 0;
              ModeText(i,shrt,mdtxt);
              printf("%*s%s", (c ? -maxlen-n : -maxlen), mdtxt, (c || (i+1==nmodes) ? "" : "\n") );
            }
            return;
          }
          --cols;
        } while (1);
}

int main(void)
{
        GrSetDriver(NULL);
        if(GrCurrentVideoDriver() == NULL) {
            printf("No graphics driver found\n");
            exit(1);
        }

        nmodes = (int)(collectmodes(GrCurrentVideoDriver(),grmodes) - grmodes);
        if(nmodes == 0) {
            printf("No graphics modes found\n");
            exit(1);
        }
        qsort(grmodes,nmodes,sizeof(grmodes[0]),vmcmp);

        for( ; ; ) {
            int  i;
            char m1[40];
            GrSetMode(GR_default_text);
            printf(
                "Graphics driver: \"%s\"\t"
                "graphics defaults: %dx%d %ld colors\n",
                GrCurrentVideoDriver()->name,
                GrDriverInfo->defgw,
                GrDriverInfo->defgh,
                GrDriverInfo->defgc
            );
            PrintModes();
            printf("\nEnter choice #, or anything else to quit> ");
            fflush(stdout);
            if(!gets(m1) || (sscanf(m1,"%d",&i) != 1) || (i < 1) || (i > nmodes))
                break;
            i--;
            speedcheck(&grmodes[i]);
        }
        if (measured_any) {
            int i;
            FILE *log = fopen("speedtst.log", "a");

            if (!log) exit(1);

            fprintf( log, "\nGraphics driver: \"%s\"\n\n",
                                               GrCurrentVideoDriver()->name);
            printf("Results: \n");
            printresultheader(log);

            for (i=0; i < nmodes; ++i)
              if (grmodes[i].measured)
                printresultline(log, &grmodes[i]);
            fclose(log);
        }
        return(0);
}

