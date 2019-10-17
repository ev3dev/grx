#include <stdlib.h>
#include <stdio.h>
#include <grgui.h>

int main()
{
    char *abouttext[4] = {
        "Welcome to MGRX and GrGUI",
        "MGRX is a small C 2D graphics library",
        "and GrGUI a miniGUI on top of MGRX",
        "visit mgrx.fgrim.com for more info"};

    GrSetMode(GR_default_graphics);
    GUIInit(1, 0);

    GUICDialogInfo("Hello GrGUI", (void **)abouttext, 4, "Ok");

    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
