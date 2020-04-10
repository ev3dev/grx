/**
 ** gr18n.c - A catalog implementation to help internationalize programs
 **
 ** Copyright (C) 2020 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#include <stdlib.h>
#include <stdio.h>
#include "libgrx.h"

typedef struct {
    void *label;    // languaje label
    void *str[1];   // strings, not the actual dimension
} Language;

static int lg_inited = 0;
static int numlg = 0;
static int numstr = 0;
static int actlg = 0;
static void *defaultstr = NULL;

static Language *lg[GR_MAX_I18N_LANGS];

int GrI18nInit(int nlg, int nstr, void *defstr)
{
    int i, j;

    if (lg_inited) return 0;

    numlg = nlg;
    if (numlg > GR_MAX_I18N_LANGS) return 0;

    numstr = nstr;
    defaultstr = defstr;

    for (i=0; i<numlg; i++) {
        lg[i] = calloc(1, sizeof(Language)+sizeof(void *)*numstr);
        if (lg[i] == NULL) {
            for (j=0; j<i; j++) free(lg[j]);
            return 0;
        }
    }

    lg_inited = 1;
    return 1;
}

void GrI18nEnd(void)
{
    int i;

    for (i=0; i<numlg; i++) {
        free(lg[i]);
    }

    lg_inited = 0;
    numlg = 0;
    numstr = 0;
    actlg = 0;
    defaultstr = NULL;
}

int GrI18nSetLabel(int lid, void *label)
{
    if (!lg_inited) return 0;
    if (lid >= numlg) return 0;
    lg[lid]->label = label;
    return 1;
}

void * GrI18nGetLabel(int lid)
{
    if (!lg_inited) return 0;
    if (lid >= numlg) return 0;
    return lg[lid]->label;
}

int GrI18nSetLang(int lid)
{
    if (!lg_inited) return 0;
    if (lid >= numlg) return 0;
    actlg = lid;
    return 1;
}

void GrI18nAddStrings(int lid, int fsid, int nums, void **str)
{
    int i;

    if (!lg_inited) return;
    if (lid >= numlg) return;

    for (i=fsid; i<fsid+nums; i++) {
        if (i >= numstr) return;
        lg[lid]->str[i] = str[i-fsid];
    }
}

void * GrI18nGetString(int sid)
{
    void *ret;

    if (!lg_inited) return NULL;
    if (sid >= numstr) return defaultstr;

    ret = lg[actlg]->str[sid];
    if (ret == NULL) ret = lg[0]->str[sid];

    return ret;
}
