#include <stdlib.h>
#include <stdio.h>
#include <grgui.h>

#define COMMAND_OPTION1     1
#define COMMAND_OPTION2     2
#define COMMAND_OPTION3     3
#define COMMAND_OPTION4     4
#define COMMAND_EXIT        5

#define ID_MENU1            1
#define ID_MENU2            2

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
        {GUI_MI_OPER, 1, "E&xit", 'X', NULL, 0, COMMAND_EXIT, 0}};
    static GUIMenu menu1 = {ID_MENU1, 6, 0, itemsm1};

    static GUIMenuItem itemsm2[2] = {
        {GUI_MI_OPER, 1, "Option &3", '3', NULL, 0, COMMAND_OPTION3, 0},
        {GUI_MI_OPER, 1, "Option &4", '4', NULL, 0, COMMAND_OPTION4, 0}};
    static GUIMenu menu2 = {ID_MENU2, 2, 0, itemsm2};
    
    GrEvent ev;
    int result;
    char s[81];

    GrSetMode(GR_default_graphics);
    GUIInit(1, 0);

    GUIMenuRegister(&menu1);
    GUIMenuRegister(&menu2);

    print_line("Press R to run menu, Esc to quit");
    while(1) {
        GrEventWait(&ev);
        if (ev.type == GREV_KEY) {
            if (ev.p1 == GrKey_Escape) break;
            if (ev.p1 == 'r' || ev.p1 == 'R') {
                result = GUIMenuRun(ID_MENU1, 100, 100, 0);
                sprintf(s,"GUIMenuRun returned %d", result);
                print_line(s);
            }
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
