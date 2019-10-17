#include <stdlib.h>
#include <stdio.h>
#include <grgui.h>
#include "mgrxcolr.h"

#if defined(__MSDOS__) || defined(__WIN32__)
#define JPGIMGBG  "..\\testimg\\jpeg4.jpg"
#else
#define JPGIMGBG  "../testimg/jpeg4.jpg"
#endif

int main()
{
    char *abouttext[4] = {
        "Welcome to MGRX and GrGUI",
        "MGRX is a small C 2D graphics library",
        "and GrGUI a miniGUI on top of MGRX",
        "visit mgrx.fgrim.com for more info"};
    GrContext *globctx;

    GrSetMode(GR_width_height_bpp_graphics, 640, 480, 32);
    GrGenWebColorTable();
    GrSetFontPath("../fonts/;./");
    GUIInit(1, 1);

    GUIObjectsSetColors(WEBC_KHAKI, WEBC_PERU, WEBC_SIENNA);
    GUIObjectsSetFontByName("tmgrx16b.fnt");
    GUIDialogsSetColors(WEBC_BLACK, WEBC_ORANGE, WEBC_MAROON,
                        WEBC_ANTIQUEWHITE);
    GUICDialogsSetColors(WEBC_TAN, WEBC_BLACK);
    GUIDialogsSetTitleFontByName("ncen40bi.fnt");
    GUICDialogsSetFontByName("tmgrx18b.fnt");

    globctx = GUIGetGlobalContext();
    GrSetContext(globctx);

    if (GrNumColors() > 256 && GrJpegSupport()) {
        GrLoadContextFromJpeg(NULL, JPGIMGBG, 1);
    } else {
        GrFilledBox(0, 0, GrMaxX(), GrMaxY(), WEBC_GOLDENROD);
    }
    GUIGlobalBltRectToScreen(0, 0, GrScreenX()-1, GrScreenY()-1);

    GUICDialogInfo("Hello GrGUI", (void **)abouttext, 4, "Okey");

    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
