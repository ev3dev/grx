#include <stdlib.h>
#include <stdio.h>
#include <grgui.h>

int main()
{
    GUIContext *gctx1, *gctx2, *gctx3;
    GrEvent ev;

    GrSetMode(GR_default_graphics);
    GUIInit(1, 0);

    GrClearScreen(GrAllocColor(0, 100, 0));
    gctx1 = GUIContextCreate(100, 100, 200, 200, 1);
    gctx2 = GUIContextCreate(150, 150, 250, 250, 1);
    gctx3 = GUIContextCreate(200,  50, 400, 300, 1);
    if (gctx1 == NULL || gctx2 == NULL || gctx3 == NULL) exit(1);

    GrTextXY(10, 10, "Test GUIContexts, press any key to continue",
             GrWhite(), GrNOCOLOR);

    GUIContextSaveUnder(gctx1);
    GrSetContext(gctx1->c);
    GrClearContext(GrAllocColor(100, 0, 0));
    GrEventWaitKeyOrClick(&ev);

    GUIContextSaveUnder(gctx2);
    GrSetContext(gctx2->c);
    GrClearContext(GrAllocColor(0, 0, 100));
    GrEventWaitKeyOrClick(&ev);

    GUIContextSaveUnder(gctx3);
    GrSetContext(gctx3->c);
    GrClearContext(GrAllocColor(200, 200, 0));
    GrEventWaitKeyOrClick(&ev);

    GUIContextRestoreUnder(gctx3);
    GUIContextDestroy(gctx3);
    GrEventWaitKeyOrClick(&ev);

    GUIContextRestoreUnder(gctx2);
    GUIContextDestroy(gctx2);
    GrEventWaitKeyOrClick(&ev);

    GUIContextRestoreUnder(gctx1);
    GUIContextDestroy(gctx1);
    GrEventWaitKeyOrClick(&ev);

    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
