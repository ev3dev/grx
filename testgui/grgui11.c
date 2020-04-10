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
    GrContext *globctx, *imgctx;
    int imgloaded = 0, imgwidth, imgheight;

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
        if (GrQueryJpeg(JPGIMGBG, &imgwidth, &imgheight) == 0) {
            imgctx = GrCreateContext(imgwidth, imgheight, NULL, NULL);
            if (imgctx != NULL) {
                if (GrLoadContextFromJpeg(imgctx, JPGIMGBG, 1) == 0) {
                    GrStretchBlt(NULL, 0, 0, GrMaxX(), GrMaxY(),
                                imgctx, 0, 0, imgwidth-1, imgheight-1, GrWRITE);
                    imgloaded = 1;
                }
            GrDestroyContext(imgctx);
            }
        }
    }
    if (!imgloaded) {
        GrFilledBox(0, 0, GrMaxX(), GrMaxY(), WEBC_GOLDENROD);
    }
    GUIDBCurCtxBltToScreen();

    GUICDialogInfo("Hello GrGUI", (void **)abouttext, 4, "Okey");

    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
