#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <grgui.h>
#include <mgrxcolr.h>


#define COMMAND_EXIT         1
#define COMMAND_SWITCH_L1    2
#define COMMAND_SWITCH_L2    3
#define COMMAND_SWITCH_L3    4
#define COMMAND_SWITCH_L4    5
#define COMMAND_GET_DATA     6

char *listopt[5] = { "Primera opción", "Segunda opción", 
    "Tercera opción", "Cuarta opción", "Quinta opción"};
char bdtline1[81], bdtline2[81], bdtline3[81], bdtline4[81];
char *buf_test[4] = {bdtline1, bdtline2, bdtline3, bdtline4};

void add_line_to_buf_test(char *s)
{
    int i;
    
    for (i=0; i<3; i++)
        memcpy(buf_test[i], buf_test[i+1], 81);
    
    strncpy(buf_test[3], s, 80);
    buf_test[3][80] = '\0';
}

int process_go_event(GUIGroup *go, GrEvent *ev)
{
    char aux[81];
    char *s, *sonoff[4];
    char *son = "On";
    char *soff = "Off";
    int i, status;

    if (ev->type == GREV_COMMAND) {
        switch (ev->p1) {
            case COMMAND_EXIT :
                return -1;
            case COMMAND_SWITCH_L1 :
                GUIGroupSetOn(go, 0, 1);
                return 1;
            case COMMAND_SWITCH_L2 :
                GUIGroupSetOn(go, 1, 1);
                return 1;
            case COMMAND_SWITCH_L3 :
                GUIGroupSetOn(go, 2, 1);
                return 1;
            case COMMAND_SWITCH_L4 :
                GUIGroupSetOn(go, 3, 1);
                return 1;
            case COMMAND_GET_DATA :
                for (i=0; i<4; i++) {
                    status = GUIGroupGetOn(go, i);
                    sonoff[i] = status ? son : soff;
                }
                sprintf(aux, "Lights status 1:%s 2:%s 3:%s 4:%s",
                        sonoff[0], sonoff[1], sonoff[2], sonoff[3]);
                add_line_to_buf_test(aux);
                s = GUIGroupGetText(go, 5, GR_UTF8_TEXT);
                sprintf(aux, "Entry: %s", s);
                add_line_to_buf_test(aux);
                free(s);
                GUIGroupRePaintObject(go, 15);
                return 1;
        }
    }
    if (ev->type == GREV_FCHANGE) {
        sprintf(aux, "Field changed, p1=%ld, p2=%ld", ev->p1, ev->p2);
        add_line_to_buf_test(aux);
        GUIGroupRePaintObject(go, 15);
    }

    return GUIGroupProcessEvent(go, ev);
}

int main()
{
    GUIGroup *go;
    GrEvent ev;

    GrSetMode(GR_default_graphics);
    GrSetUserEncoding(GRENC_UTF_8);
    GUIInit(1, 0);
    GrGenEgaColorTable();
    GrClearContext(EGAC_DARKGRAY);

    GUIObjectsSetColors(EGAC_BLACK, EGAC_LIGHTGRAY, EGAC_DARKGRAY);
    go = GUIGroupCreate(17, 160, 70);
    if (go == NULL) exit(1);
    GUIObjectSetLight(&(go->o[0]), 0,   0, 0, 80, 32, EGAC_LIGHTGREEN, EGAC_BLACK, "Light 1", 0);
    GUIObjectSetLight(&(go->o[1]), 1,  80, 0, 80, 32, EGAC_LIGHTGREEN, EGAC_BLACK, "Light 2", 1);
    GUIObjectSetLight(&(go->o[2]), 2, 160, 0, 80, 32, EGAC_LIGHTRED, EGAC_BLACK, "Light 3", 0);
    GUIObjectSetLight(&(go->o[3]), 3, 240, 0, 80, 32, EGAC_LIGHTCYAN, EGAC_BLACK, "Light 4", 1);
    GUIObjectSetLabel(&(go->o[4]), 4,   0, 40, 160, 30, GrNOCOLOR, EGAC_WHITE, "Editable field");
    GUIObjectSetEntry(&(go->o[5]), 5, 160, 40, 160, 30, EGAC_WHITE, EGAC_BLACK, 30, "entry field");
    GUIObjectSetLabel(&(go->o[6]), 6,   0, 80, 160, 30, GrNOCOLOR, EGAC_WHITE, "List field #1");
    GUIObjectSetList(&(go->o[7]),  7, 160, 80, 160, 30, EGAC_WHITE, EGAC_BLACK, (void **)listopt, 5, 3, 1);
    GUIObjectSetLabel(&(go->o[8]), 8,   0, 120, 160, 30, GrNOCOLOR, EGAC_WHITE, "List field #2");
    GUIObjectSetList(&(go->o[9]),  9, 160, 120, 160, 30, EGAC_WHITE, EGAC_BLACK, (void **)listopt, 5, 5, 2);
    GUIObjectSetButton(&(go->o[10]), 10,   0, 160, 40, 40, EGAC_CYAN, EGAC_WHITE, "L1", COMMAND_SWITCH_L1, 0, 0);
    GUIObjectSetButton(&(go->o[11]), 11,  40, 160, 40, 40, EGAC_CYAN, EGAC_WHITE, "L2", COMMAND_SWITCH_L2, 0, 0);
    GUIObjectSetButton(&(go->o[12]), 12,  80, 160, 40, 40, EGAC_CYAN, EGAC_WHITE, "L3", COMMAND_SWITCH_L3, 0, 0);
    GUIObjectSetButton(&(go->o[13]), 13, 120, 160, 40, 40, EGAC_CYAN, EGAC_WHITE, "L4", COMMAND_SWITCH_L4, 0, 0);
    GUIObjectSetButton(&(go->o[14]), 14, 160, 160, 160, 40, EGAC_CYAN, EGAC_WHITE, "Get data", COMMAND_GET_DATA, 0, 0);
    GUIObjectSetText(&(go->o[15]), 15, 0, 210, 320, 80, EGAC_LIGHTGRAY, EGAC_BLACK, (void **)buf_test, 4, GR_ALIGN_LEFT, NULL);
    GUIObjectSetButton(&(go->o[16]), 16, 80, 300, 160, 40, EGAC_GREEN, EGAC_WHITE, "Exit", COMMAND_EXIT, 0, 0);
    GUIGroupSetSelected(go, 16, 0);

    GUIGroupPaint(go);

    while(1) {
        GrEventRead(&ev);
        if ((ev.type == GREV_KEY) && (ev.p1 == GrKey_Escape)) break;
        if (process_go_event(go, &ev) < 0) break;
    }

    GUIGroupDestroy(go);
    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}
