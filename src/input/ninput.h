/**
 ** ninput.h ---- MGRX events internals
 **
 ** Copyright (C) 2005 Mariano Alvarez Fernandez
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

/* functions to be provided in the input driver */

int _GrEventInit(void);
void _GrEventUnInit(void);
int _GrReadInputs(void);

int _GrMouseDetect(void);
void _GrInitMouseCursor(void);

int _GrGetKbSysEncoding(void);

/* intl support, recode functions */

int GrRecodeEvent(GrEvent *ev, int srcenc, int desenc);
