#include <stdlib.h>
#include <stdio.h>
#include <grgui.h>

void print_line(char *s)
{
    #define LINE_HIGH 16
    static int ypos = 10;
    
    if (ypos >= GrMaxY() - LINE_HIGH) {
        GrClearContext(GrBlack());
        ypos = 10;
    }
    GrTextXY(10, ypos, s, GrWhite(), GrBlack());
    ypos += LINE_HIGH;
}
    
int main()
{
    char *bodytext[2] = {
        "This is a GrGUI common dialog",
        "select one option"};
    GrEvent ev;
    char s[81];
    int result;

    GrSetMode(GR_default_graphics);
    GUIInit(1, 0);

    print_line("Press 1 to run dialog Yes/No");
    print_line("Press 2 to run dialog Yes/No/Cancel");
    print_line("Esc to quit");

    while(1) {
        GrEventWait(&ev);
        if (ev.type == GREV_KEY) {
            if (ev.p1 == GrKey_Escape) break;
            if (ev.p1 == '1') {
                result = GUICDialogYesNo("Test Yes/No",
                            (void **)bodytext, 2, "Yes", "No");
                sprintf(s,"Dialog Yes/No returned %d", result);
                print_line(s);
            }
            if (ev.p1 == '2') {
                result = GUICDialogYesNoCancel("Test Yes/No/Cancel",
                            (void **)bodytext, 2, "Yes", "No", "Cancel");
                sprintf(s,"Dialog Yes/No returned %d", result);
                print_line(s);
            }
        }
    }

    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
