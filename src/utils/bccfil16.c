/**
 ** BCCFIL32.C ---- BCC memory fill operations (16bit)
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 **
 **/

#include "libgrx.h"
#include "mempeek.h"
#include "memfill.h"

#pragma inline
#pragma warn -rvl

#define COLFL(FN,INS) \
  void far * FN (void far *P,int O, unsigned V, unsigned C) {      \
    asm         les     di, P                      ;               \
    asm         mov     cx, C                      ;               \
    asm         or      cx,cx                      ;               \
    asm         jz      short _##FN##3             ;               \
    asm         mov     bx, O                      ;               \
    asm         mov     ax, V                      ;               \
    asm         inc     cx                         ;               \
    asm         shr     cx,1                       ;               \
    asm         jnc     short _##FN##2             ;               \
       _##FN##1:                                                   \
    asm         INS     es:[di],ax                 ;               \
    asm         add     di,bx                      ;               \
       _##FN##2:                                                   \
    asm         INS     es:[di],ax                 ;               \
    asm         add     di,bx                      ;               \
    asm         dec     cx                         ;               \
    asm         jnz     short _##FN##1             ;               \
        _##FN##3:                                                  \
    asm         mov     dx,es                      ;               \
    asm         mov     ax,di                      ;               \
  }

COLFL(_GR_colfill_w_mov,MOV_INS)
COLFL(_GR_colfill_w_xor,XOR_INS)
COLFL(_GR_colfill_w_or,OR_INS)
COLFL(_GR_colfill_w_and,AND_INS)
