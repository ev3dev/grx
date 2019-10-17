#include <stdlib.h>
#include <stdio.h>
#include <grgui.h>

typedef struct {
    GUIPanel *gp;
    GrColor fg;
    GrColor bg;
} UserData;

void paint_panel1(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GrClearContext(ud->bg);
    GrTextXY(10, 10, "This is a simple panel with 1 px border", ud->fg, ud->bg);
    GrTextXY(10, 26, "Press R to reverse colors, C to continue", ud->fg, ud->bg);
}

void paint_panel2(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GrClearContext(ud->bg);
    GrTextXY(10, 10, "This is a panel with 4 px border,", ud->fg, ud->bg);
    GrTextXY(10, 26, "title and scroll bars", ud->fg, ud->bg);
    GrTextXY(10, 42, "Press R to reverse colors, C to continue", ud->fg, ud->bg);
}

void paint_panel2_title(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GrClearContext(ud->fg);
    GrTextXY(2, 2, "This is the panel 2 title", ud->bg, ud->fg);
    GrHLine(0, GrMaxX(), GrMaxY(), ud->bg);
}

int process_panel_event(void *data, GrEvent *ev)
{
    UserData *ud;
    GrColor aux;

    ud = (UserData *)data;
    if (ev->type == GREV_KEY) {
        if (ev->p1 == 'r' || ev->p1 == 'R') {
            aux = ud->fg;
            ud->fg = ud->bg;
            ud->bg = aux;
            ud->gp->paintcl(data);
            return 1;
        }
        if (ev->p1 == 'c' || ev->p1 == 'C') {
            return -1;
        }
    }
    return 0;
}

int main()
{
    GUIPanel *gp1, *gp2;
    GrEvent ev;
    UserData ud;
    int ret;

    GrSetMode(GR_default_graphics);
    GUIInit(1, 0);

    GrClearScreen(GrAllocColor(0, 100, 0));
    gp1 = GUIPanelCreate(100, 100, GrMaxX()-200, GrSizeY()-200,
                         GUI_PCAPB_SU, 1, 0);
    gp2 = GUIPanelCreate(100, 100, GrMaxX()-200, GrSizeY()-200,
                         GUI_PCAPB_SU|GUI_PCAPB_VSCB|GUI_PCAPB_HSCB, 4, 20);
    if (gp1 == NULL || gp2 == NULL) exit(1);

    ud.gp = gp1;
    ud.fg = GrBlack();
    ud.bg = GrWhite();

    GUIPanelSetClCallBacks(gp1, paint_panel1, process_panel_event);
    GUIPanelSetUserData(gp1, (void *)&ud);
    
    GUIPanelPaint(gp1, GrBlack(), GrWhite());
    while(1) {
        GrEventWait(&ev);
        ret = GUIPanelProcessEvent(gp1, &ev);
        if (ret == -1) break;
    }
    GUIContextRestoreUnder(gp1->gc);
    GUIPanelDestroy(gp1);
    
    ud.gp = gp2;
    ud.fg = GrAllocColor2(0x555555);
    ud.bg = GrAllocColor2(0x55FFFF);

    GUIPanelSetClCallBacks(gp2, paint_panel2, process_panel_event);
    GUIPanelSetTlCallBack(gp2, paint_panel2_title);
    GUIPanelSetUserData(gp2, (void *)&ud);
    
    GUIPanelPaint(gp2, GrBlack(), GrWhite());
    while(1) {
        GrEventWait(&ev);
        ret = GUIPanelProcessEvent(gp2, &ev);
        if (ret == -1) break;
    }
    GUIContextRestoreUnder(gp2->gc);
    GUIPanelDestroy(gp2);

    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
