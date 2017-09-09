/* This example program is in the public domain */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mgrx.h"
#include "mgrxkeys.h"

#define MAX_FONTLIST 400
#define MAX_FONTLOADED 20
#define DIR_FILE "_fonts.dir"

static char *lfont[MAX_FONTLIST];
static int nlfont;

static GrFont *grf[MAX_FONTLOADED];
static int grfp[MAX_FONTLOADED];
static int ngrf;

int loadfont(int n, int clear)
{
    if (clear) ngrf = 0;

    if (ngrf >= MAX_FONTLOADED) return -1;
    grf[ngrf] = GrLoadFont(lfont[n]);
    grfp[ngrf] = n;
    if (grf[ngrf] == NULL) return -1;
    ngrf++;

    return 0;
}

void unloadfonts(void)
{
    int i;

    for (i=0; i<ngrf; i++) {
        GrUnloadFont(grf[i]);
    }
}

int loadfontlist(char *file)
{
    FILE *f;
    char s[251];
    char *p;

    nlfont = 0;
    f = fopen(file, "rt");
    if (f == NULL) {
        printf("Error al cargar %s\n", file);
        return -1;
    }

    while (fgets(s, 250, f)) {
        if (s[0] == '#') continue;
        p = strchr(s, ' ');
        if (p && nlfont < MAX_FONTLIST) {
            *p = '\0';
            lfont[nlfont++] = strdup(s);
        }
    }
    return 0;
}

void printline(char *s, int x, int *y, int h, GrTextOption *grt)
{
    GrDrawString(s, strlen(s), x, *y, grt);
    *y += h;
}

void showeveryfont(int listar)
{
    GrTextOption grt;
    int k, x, y, h;
    char message[121];
    GrEvent ev;
    int i,j;

    grt.txo_fgcolor = GrBlack();
    grt.txo_bgcolor = GrWhite();
    grt.txo_direct = GR_TEXT_RIGHT;
    grt.txo_xalign = GR_ALIGN_LEFT;
    grt.txo_yalign = GR_ALIGN_TOP;
    grt.txo_chrtype = GR_BYTE_TEXT;

    k = 0;
    while (1) {
        if (loadfont(k,1) == 0) {
            x = 2;
            y = 2;
            GrClearScreen(GrWhite());
            h = GrFontCharHeight(grf[0], 'M') + 4;
            grt.txo_font = grf[0];

            sprintf(message, "%s: Hello, GRX world!",lfont[grfp[0]]);
            printline(message, x, &y, h, &grt);
            sprintf(message, "%d x %d %s", grf[0]->h.width, grf[0]->h.height, 
                (grf[0]->h.proportional) ? "proportional" : "fixed");
            printline(message, x, &y, h, &grt);
            sprintf(message, "Encoding: %d",(int)(grf[0]->h.encoding));
            printline(message, x, &y, h, &grt);
            y += h;
            
            if (listar) {
                printf("%s %s %s ", lfont[grfp[0]], grf[0]->h.name, grf[0]->h.family);
                printf("%d %d ", grf[0]->h.width, grf[0]->h.height);
                printf("%s ", (grf[0]->h.proportional) ? "proportional" : "fixed");
                printf("%d\n", grf[0]->h.encoding);
            }

            j = 0;
            grt.txo_chrtype = GR_WORD_TEXT;
            for (i=grf[0]->h.minchar; i<grf[0]->h.minchar+grf[0]->h.numchars; i++) {
                GrDrawChar(i, x, y, &grt);
                x += GrCharWidth(i, &grt);
                j++;
                if (j >= 32) {
                    j = 0;
                    y += h;
                    x = 2;
                }
            }
            grt.txo_chrtype = GR_BYTE_TEXT;

            while (1) {
                GrEventWait(&ev);
                if (ev.type == GREV_KEY) {
                    if (ev.p2 == GRKEY_KEYCODE) {
                        if (ev.p1 == GrKey_Left && k > 0) {
                            k--;
                            break;
                        } else if (ev.p1 == GrKey_Right && k < (nlfont-1)) {
                            k++;
                            break;
                        }
                    } else {
                        if (ev.p1 == GrKey_Escape) {
                            unloadfonts();
                            return;
                        }
                    }
                } else if (ev.type == GREV_MOUSE) {
                    if (ev.p1 == GRMOUSE_RB_RELEASED && k > 0) {
                        k--;
                        break;
                    } else if (ev.p1 == GRMOUSE_LB_RELEASED && k < (nlfont-1)) {
                        k++;
                        break;
                    }
                }
            }
        } else {
            printf("Error al cargar %s\n",lfont[grfp[0]]);
        }
    }
}

int main(int argc, char **argv)
{
    int listar = 0;
    
    if (argc > 1 && argv[1][0] == 'l') listar = 1;
    
    GrSetMode(GR_width_height_bpp_graphics, 1024, 768, 24);
    GrEventInit();
    GrMouseDisplayCursor();

    if (loadfontlist(DIR_FILE) == 0) {
        showeveryfont(listar);
    }

    GrEventUnInit();
    return 0;
}
