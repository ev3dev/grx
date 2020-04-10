/**
 ** i18ntest.c ---- test the MGRX i18n catalogs implementation
 **
 ** Copyright (c) 2020 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
 **
 ** This is a test/demo file of the GRX graphics library.
 ** You can use GRX test/demo files as you want.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **/

#include <stdlib.h>
#include <stdio.h>
#include "mgrx.h"

enum SID {
    STLANG = 0,
    STR1,
    STR2,
    STR3,
    STR4,
    STR5,
    STR6,
    STR7
};

char *senglish[7] = {
    "Language",
    "this is the first string",
    "this is the second string",
    "this is the third string",
    "this is the four string, with no translation",
    "this is the five string",
    "this is the six string"};

char *sespanol1[4] = {
    "Idioma",
    "este es el primer párrafo",
    "este es el segundo párrafo",
    "este es el tercer párrafo"};

char *sespanol2[2] = {
    "este es el quinto párrafo",
    "este es el sexto párrafo"};

char *defaultstr = "This is the default, returned when no string is defined";

void print_strings(int lid)
{
    int i;

    #define _(sid) ((char *)GrI18nGetString(sid))

    GrI18nSetLang(lid);
    printf("%s: %s\n", _(STLANG), (char *)GrI18nGetLabel(lid));
    
    // becuase STR7 isn't defined, we expext to have the default string there
    for (i=STR1; i<=STR7; i++) {
        printf("%s\n", _(i));
    }
}

int main()
{
    if (!GrI18nInit(2, 7, defaultstr)) {
        printf("GrI18 not inited\n");
        return 1;
    }

    GrI18nSetLabel(0, "English");
    GrI18nSetLabel(1, "Español");

    GrI18nAddStrings(0, STLANG, 7, (void **)senglish);

    // we don't add spanish STR4 on purpose
    GrI18nAddStrings(1, STLANG, 4, (void **)sespanol1);
    GrI18nAddStrings(1, STR5, 2, (void **)sespanol2);

    print_strings(0); // English
    print_strings(1); // Español

    GrI18nEnd();
    return 0;
}
