#include <stdlib.h>
#include <stdio.h>
#include <grgui.h>

#define COMMAND_OPTION1     1
#define COMMAND_OPTION2     2
#define COMMAND_OPTION3     3
#define COMMAND_OPTION4     4
#define COMMAND_OPTION5     5
#define COMMAND_OPTION6     6
#define COMMAND_EXIT        7

#define ID_MENU1            1
#define ID_MENU2            2
#define ID_MENU3            3

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
    static GUIMenuItem itemsm1[6] = {
        {GUI_MI_OPER, 1, "Option &1", '1', NULL, 0, COMMAND_OPTION1, 0},
        {GUI_MI_OPER, 1, "Option &2", '2', NULL, 0, COMMAND_OPTION2, 0},
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_MENU, 1, "&Submenu", 'S', NULL, 0, ID_MENU2, 0}, 
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_OPER, 1, "E&xit", 'X', "Ctrl+X", GrKey_Control_X, COMMAND_EXIT, 0}};
    static GUIMenu menu1 = {ID_MENU1, 6, 0, itemsm1};

    static GUIMenuItem itemsm2[2] = {
        {GUI_MI_OPER, 1, "Option &3", '3', NULL, 0, COMMAND_OPTION3, 0},
        {GUI_MI_OPER, 1, "Option &4", '4', NULL, 0, COMMAND_OPTION4, 0}};
    static GUIMenu menu2 = {ID_MENU2, 2, 0, itemsm2};

    static GUIMenuItem itemsm3[4] = {
        {GUI_MI_OPER, 1, "Option &5", '5', NULL, 0, COMMAND_OPTION5, 0},
        {GUI_MI_OPER, 1, "Option &6", '6', NULL, 0, COMMAND_OPTION6, 0},
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_MENU, 1, "&Submenu", 'S', NULL, 0, ID_MENU2, 0}};
    static GUIMenu menu3 = {ID_MENU3, 4, 0, itemsm3};

    static GUIMenuBarItem mbitems[2] = {
        {"&First_menu", 1, GrKey_Alt_F, ID_MENU1}, 
        {"&Second_menu", 1, GrKey_Alt_S, ID_MENU3}};
    static GUIMenuBar menubar = {2 ,0, mbitems};

    GrEvent ev;
    char s[81];
    GrContext *ctx;

    GrSetMode(GR_default_graphics);
    GUIInit(1, 0);

    GUIMenuRegister(&menu1);
    GUIMenuRegister(&menu2);
    GUIMenuRegister(&menu3);
    GUIMenuBarSet(&menubar);
    GUIMenuBarShow();
    ctx = GrCreateSubContext(0, GUIMenuBarGetHeight(),
                             GrMaxX(), GrMaxY(), NULL, NULL);
    GrSetContext(ctx);

    print_line("Try the MenuBar above");
    while(1) {
        GrEventWait(&ev);
        if (ev.type == GREV_KEY) {
            if (ev.p1 == GrKey_Escape) break;
        }
        if (ev.type == GREV_COMMAND) {
            if (ev.p1 == COMMAND_EXIT) {
                print_line("Received COMMAND_EXIT event, exiting in 3 seconds");
                GrMouseEraseCursor();
                GrSleep(3000);
                break;
            }
            sprintf(s,"Received COMMAND event %ld", ev.p1);
            print_line(s);
        }
    }

    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
