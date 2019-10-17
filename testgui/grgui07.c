#include <stdlib.h>
#include <stdio.h>
#include <grgui.h>

typedef struct {
    GUITile *gt;
    GrColor fg;
    GrColor bg;
} UserData;

void paint_tile1(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GrClearContext(ud->bg);
    GrTextXY(10, 16, "This is a passive tile with 1 px border", ud->fg, ud->bg);
}

void paint_tile2(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GrClearContext(ud->bg);
    GrTextXY(10, 10, "This is an active tile", ud->fg, ud->bg);
    GrTextXY(10, 26, "When selected:", ud->fg, ud->bg);
    GrTextXY(10, 42, "  Press R to reverse colors", ud->fg, ud->bg);
    GrTextXY(10, 58, "  Esc to finish", ud->fg, ud->bg);
}

void paint_tile3(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GrClearContext(ud->bg);
    GrTextXY(10, 10, "This is a second active tile,", ud->fg, ud->bg);
    GrTextXY(10, 26, "When selected:", ud->fg, ud->bg);
    GrTextXY(10, 42, "  Press R to reverse colors", ud->fg, ud->bg);
    GrTextXY(10, 58, "  Esc to finish", ud->fg, ud->bg);
}

void paint_tile4(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GrClearContext(ud->bg);
    GrTextXY(10, 16, "This is a passive tile borderless", ud->fg, ud->bg);
}

int process_tile_event(void *data, GrEvent *ev)
{
    UserData *ud;
    GrColor aux;

    ud = (UserData *)data;
    if (ev->type == GREV_KEY) {
        if (ev->p1 == 'r' || ev->p1 == 'R') {
            aux = ud->fg;
            ud->fg = ud->bg;
            ud->bg = aux;
            ud->gt->p->paintcl(data);
            return 1;
        }
        if (ev->p1 == GrKey_Escape) {
            return -1;
        }
    }
    return 0;
}

int main()
{
    #define IDT1 1
    #define IDT2 2
    #define IDT3 3
    #define IDT4 4

    GUITile *gt1, *gt2, *gt3, *gt4;
    UserData ud1, ud2, ud3, ud4;
    GrEvent ev;
    int ret;

    GrSetMode(GR_default_graphics);
    GUIInit(1, 0);

    //GrClearScreen(GrAllocColor(0, 100, 0));
    gt1 = GUITileCreate(IDT1, GUI_TT_STATICBORDER, 0, 0,
                        GrSizeX(), 50);
    gt2 = GUITileCreate(IDT2, GUI_TT_ACTIVEBORDER, 0, 50,
                        GrSizeX()/2, GrSizeY()-100);
    gt3 = GUITileCreate(IDT3, GUI_TT_ACTIVEBORDER, GrSizeX()/2, 50,
                        GrSizeX()/2, GrSizeY()-100);
    gt4 = GUITileCreate(IDT4, GUI_TT_BORDERLESS, 0,
                        GrSizeY()-50, GrSizeX(), 50);
    if (gt1 == NULL || gt2 == NULL || gt3 == NULL || gt4 == NULL) exit(1);
    
    GUIPanelSetClCallBacks(gt1->p, paint_tile1, NULL);
    GUIPanelSetUserData(gt1->p, (void *)&ud1);
    ud1.gt = gt1;
    ud1.fg = GrWhite();
    ud1.bg = GrAllocColor2(0x00AAAA);
    GUIPanelSetClCallBacks(gt2->p, paint_tile2, process_tile_event);
    GUIPanelSetUserData(gt2->p, (void *)&ud2);
    ud2.gt = gt2;
    ud2.fg = GrWhite();
    ud2.bg = GrAllocColor2(0x00AA00);
    GUIPanelSetClCallBacks(gt3->p, paint_tile3, process_tile_event);
    GUIPanelSetUserData(gt3->p, (void *)&ud3);
    ud3.gt = gt3;
    ud3.fg = GrWhite();
    ud3.bg = GrAllocColor2(0x00AA00);
    GUIPanelSetClCallBacks(gt4->p, paint_tile4, NULL);
    GUIPanelSetUserData(gt4->p, (void *)&ud4);
    ud4.gt = gt4;
    ud4.fg = GrWhite();
    ud4.bg = GrAllocColor2(0x555555);

    GUITileRegister(gt1);
    GUITileRegister(gt2);
    GUITileRegister(gt3);
    GUITileRegister(gt4);

    GUITilePaint(IDT1);
    GUITilePaint(IDT2);
    GUITilePaint(IDT3);
    GUITilePaint(IDT4);

    while(1) {
        GrEventWait(&ev);
        ret = GUITilesProcessEvent(&ev);
        if (ret == -1) break;
    }
    
    GUITilesDestroyAll();
    
    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
