#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <grgui.h>
#include <mgrxcolr.h>

#define COMMAND_EXIT         1
#define COMMAND_LOAD         2
#define COMMAND_SAVE         3

typedef struct {
    GUITextArea *ta;
    GUIGroup *go;
} UserData;

void paint_tl1(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GrClearContext(EGAC_LIGHTGRAY);
    GUIGroupPaint(ud->go);
}

void paint_tl2(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GUITAReDraw(ud->ta);
}

int process_tl1_event(void *data, GrEvent *ev)
{
    UserData *ud;
    GUITAStatus tast;
    FILE *fin, *fout;
    char aux[251];
    char *s;
    int len, i;

    ud = (UserData *)data;
    if (ev->type == GREV_COMMAND) {
        switch (ev->p1) {
            case COMMAND_EXIT :
                return -1;
            case COMMAND_LOAD :
                s = GUIGroupGetText(ud->go, 1, GR_UTF8_TEXT);
                fin = fopen(s, "r");
                free(s);
                if (fin == NULL) return 1;
                GUITAHideCursor(ud->ta);
                while (fgets(aux, 250, fin) != NULL) {
                    len = strlen(aux);
                    if (len>0 && aux[len-1]=='\n') aux[len-1] = '\0';
                    GUITAPutString(ud->ta, aux, 0, GR_UTF8_TEXT);
                    GUITANewLine(ud->ta);
                }
                GUITAShowCursor(ud->ta);
                fclose(fin);
                return 1;
            case COMMAND_SAVE :
                s = GUIGroupGetText(ud->go, 3, GR_UTF8_TEXT);
                fout = fopen(s, "w");
                free(s);
                if (fout == NULL) return 1;
                GUITAGetStatus(ud->ta, &tast);
                for (i=0; i<tast.nlines; i++) {
                    s = GUITAGetString(ud->ta, i, GR_UTF8_TEXT);
                    if (s == NULL) break;
                    fputs(s, fout);
                    putc( '\n', fout);
                    free(s);
                }
                fclose(fout);
                return 1;
        }
    }

    return GUIGroupProcessEvent(ud->go, ev);
}

int process_tl2_event(void *data, GrEvent *ev)
{
    UserData *ud;

    ud = (UserData *)data;
    return GUITAProcessEvent(ud->ta, ev);
}

int main()
{
    #define IDT1 1
    #define IDT2 2

    GUITile *gt1, *gt2;
    GUITextArea *ta1;
    GUIGroup *go1;
    UserData ud;
    GrEvent ev;
    int ret;

    GrSetMode(GR_default_graphics);
    GUIInit(1, 0);
    GrGenEgaColorTable();
    GUIScrollbarsSetColors(EGAC_LIGHTGRAY, EGAC_DARKGRAY);
    GUITilesSetColors(EGAC_BLACK, EGAC_LIGHTGRAY, EGAC_YELLOW);
    GUIObjectsSetColors(EGAC_BLACK, EGAC_LIGHTGRAY, EGAC_DARKGRAY);

    gt1 = GUITileCreate(IDT1, GUI_TT_ACTIVEBORDER, 0, 0,
                        168, GrSizeY());
    gt2 = GUITileCreate(IDT2, GUI_TT_ACTIVEBWSCB, 168, 0,
                        GrSizeX()-168, GrSizeY());
    if (gt1 == NULL || gt2 == NULL) exit(1);
    
    GUITileRegister(gt1);
    GUITileRegister(gt2);

    go1 = GUIGroupCreate(5, 10, 32);
    GUIObjectSetButton(&(go1->o[0]), 0, 0,   0, 140, 40, EGAC_GREEN, EGAC_WHITE, "Load File", COMMAND_LOAD, 0, 0);
    GUIObjectSetEntry(&(go1->o[1]),  1, 0,  44, 140, 30, EGAC_WHITE, EGAC_BLACK, 30, "inputfile");
    GUIObjectSetButton(&(go1->o[2]), 2, 0,  84, 140, 40, EGAC_GREEN, EGAC_WHITE, "Save File", COMMAND_SAVE, 0, 0);
    GUIObjectSetEntry(&(go1->o[3]),  3, 0, 128, 140, 30, EGAC_WHITE, EGAC_BLACK, 30, "outputfile");
    GUIObjectSetButton(&(go1->o[4]), 4, 0, 168, 140, 40, EGAC_RED, EGAC_WHITE, "Exit", COMMAND_EXIT, 0, 0);
    GUIGroupSetSelected(go1, 0, 0);
    GUIGroupSetPanel(go1, gt1->p);

    ta1 = GUITACreate(gt2->p, NULL, 10000);
    if (ta1 == NULL) exit(1);
    GUITASetBgColor(ta1, EGAC_DARKGRAY);
    GUITASetTextColors(ta1, EGAC_WHITE, EGAC_DARKGRAY);
    GUITASetCursorColor(ta1, EGAC_YELLOW);
    GUITAClear(ta1);

    GUIPanelSetClCallBacks(gt1->p, paint_tl1, process_tl1_event);
    GUIPanelSetClCallBacks(gt2->p, paint_tl2, process_tl2_event);
    GUIPanelSetUserData(gt1->p, (void *)&ud);
    GUIPanelSetUserData(gt2->p, (void *)&ud);
    ud.ta = ta1;
    ud.go = go1;

    GUITilePaint(IDT1);
    GUITilePaint(IDT2);
    GUITAShowCursor(ta1);

    while(1) {
        GrEventRead(&ev);
        ret = GUITilesProcessEvent(&ev);
        if (ret == -1) break;
    }
    
    GUIGroupDestroy(go1);
    GUITADestroy(ta1);
    GUITilesDestroyAll();
    
    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
