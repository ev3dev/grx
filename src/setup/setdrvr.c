/*
 * setdrvr.c ---- video driver setup
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <ctype.h>
#include <string.h>

#include "globals.h"
#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "memcopy.h"
#include "memfill.h"
#include "util.h"

static char *nxtoken(char *p,char *token)
{
        while(*p == ' ') p++;
        while(*p && (*p != ' ')) *token++ = *p++;
        *token = '\0';
        return(p);
}

int grx_set_driver(char *drvspec)
{
        static int firsttime = TRUE;
        GrxVideoDriver *drv = NULL;
        char options[100];
        if(!drvspec) {
            drvspec = getenv("GRX20DRV");
            DBGPRINTF(DBG_DRIVER,("Checking GRX20DRV: %s\n",drvspec));
        }
        options[0] = '\0';
        if(drvspec) {
            char t[100],name[100],*p = drvspec;
            name[0] = '\0';
            while(p = nxtoken(p,t),t[0] != '\0') {
                if(strlen(t) == 2) {
#                   define CH16(c1,c2)  (((c1) << 8) | (c2))
                    void *oaddr = NULL;
                    int  cf = FALSE;
                    switch(CH16(tolower(t[0]),tolower(t[1]))) {
                        case CH16('n','c'):
                        case CH16('g','c'): oaddr = &DRVINFO->defgc; cf = TRUE; break;
                        case CH16('t','c'): oaddr = &DRVINFO->deftc; cf = TRUE; break;
                        case CH16('g','w'): oaddr = &DRVINFO->defgw; break;
                        case CH16('t','w'): oaddr = &DRVINFO->deftw; break;
                        case CH16('g','h'): oaddr = &DRVINFO->defgh; break;
                        case CH16('t','h'): oaddr = &DRVINFO->defth; break;
                    }
                    if(oaddr) {
                        long optval;
                        p = nxtoken(p,t);
                        if(sscanf(t,"%ld",&optval) > 0) {
                            if(cf) {
                                switch(toupper(t[strlen(t) - 1])) {
                                    case 'K': optval <<= 10; break;
                                    case 'M': optval <<= 20; break;
                                }
                                *((long *)oaddr) = optval;
                                continue;
                            }
                            *((int *)oaddr) = (int)optval;
                        }
                        continue;
                    }
                }
                strcpy(name,t);
            }
            for(p = name; (p = strchr(p,':')) != NULL; p++) {
                if(p[1] == ':') {
                    strcpy(options,&p[2]);
                    *p = '\0';
                    break;
                }
            }
            if(name[0] != '\0') {
                DBGPRINTF(DBG_DRIVER,("Looking for match: %s\n",name));
                int ii = 0,found = FALSE;
                while(!found && ((drv = _GrVideoDriverTable[ii++]) != NULL)) {
                    char *n = name;

                    DBGPRINTF(DBG_DRIVER,("Trying: %s\n",drv->name));
                    for(p = drv->name; ; p++,n++) {
                        if(tolower(*p) != tolower(*n)) break;
                        if(*p == '\0') { found = TRUE; break; }
                    }
                }
                if(!found) return(FALSE);
            }
        }
        if(!drv) {
            DBGPRINTF(DBG_DRIVER,("Trying detect\n"));
            GrxVideoDriver *dp;
            int ii,maxmodes = 0;
            for(ii = 0; (dp = _GrVideoDriverTable[ii]) != NULL; ii++) {
                if(dp->detect && (*dp->detect)()) {
                    int nm = 0;
                    for( ; dp; dp = dp->inherit) nm += dp->n_modes;
                    if(nm > maxmodes) {
                        drv = _GrVideoDriverTable[ii];
                        maxmodes = nm;
                    }
                }
            }
            if(!drv) return(FALSE);
            DBGPRINTF(DBG_DRIVER,("Found %s\n", drv->name));
        }
        _GrCloseVideoDriver();
        if(firsttime) {
            atexit(_GrCloseVideoDriver);
            firsttime = FALSE;
        }
        if(!drv->init || drv->init(options)) {
            DRVINFO->vdriver = drv;
            return(TRUE);
        }
        return(FALSE);
}

void _GrCloseVideoDriver(void)
{
        sttcopy(&DRVINFO->fdriver,&DRVINFO->tdriver);
        if(DRVINFO->vdriver != NULL) {
            if(DRVINFO->vdriver->reset) (*DRVINFO->vdriver->reset)();
            DRVINFO->vdriver = NULL;
        }
}
